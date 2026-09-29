#include "XlsxJson.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace margelo::nitro::xlsx {

namespace {

bool isCellEmpty(OpenXLSX::XLWorksheet& ws, uint32_t row, uint16_t col) {
  return ws.findCell(row, col).empty() || ws.findCell(row, col).value().type() == OpenXLSX::XLValueType::Empty;
}

std::string keyFromCellValue(OpenXLSX::XLWorksheet& ws, uint32_t row, uint16_t col) {
  auto cell = ws.findCell(row, col);
  if (cell.empty()) return "col" + std::to_string(col);
  switch (cell.value().type()) {
    case OpenXLSX::XLValueType::String:
      return cell.value().get<std::string>();
    case OpenXLSX::XLValueType::Integer:
      return std::to_string(static_cast<long long>(cell.value().get<int64_t>()));
    case OpenXLSX::XLValueType::Float:
      return std::to_string(static_cast<int>(cell.value().get<double>()));
    default:
      return "col" + std::to_string(col);
  }
}

void recordFromRow(OpenXLSX::XLWorksheet& ws,
                   uint32_t row,
                   const std::vector<std::string>& keys,
                   uint16_t lastCol,
                   std::shared_ptr<AnyMap> record) {
  uint16_t keyCount = std::min(static_cast<uint16_t>(keys.size()), lastCol);
  for (uint16_t c = 0; c < keyCount; ++c) {
    const std::string& key = keys[c];
    auto cell = ws.findCell(row, c + 1);
    if (cell.empty()) {
      record->setNull(key);
      continue;
    }
    switch (cell.value().type()) {
      case OpenXLSX::XLValueType::String:
        record->setString(key, cell.value().get<std::string>());
        break;
      case OpenXLSX::XLValueType::Integer:
        record->setDouble(key, static_cast<double>(cell.value().get<int64_t>()));
        break;
      case OpenXLSX::XLValueType::Float: {
        double d = cell.value().get<double>();
        if (!std::isnan(d) && !std::isinf(d)) {
          record->setDouble(key, d);
        } else {
          record->setNull(key);
        }
        break;
      }
      case OpenXLSX::XLValueType::Boolean:
        record->setBoolean(key, cell.value().get<bool>());
        break;
      default:
        record->setNull(key);
        break;
    }
  }
}

void writeValue(OpenXLSX::XLWorksheet& ws,
                uint32_t row,
                uint16_t col,
                const std::shared_ptr<AnyMap>& map,
                const std::string& key) {
  if (!map->contains(key) || map->isNull(key)) return;
  auto cell = ws.cell(row, col);
  if (map->isString(key)) {
    cell.value() = map->getString(key);
  } else if (map->isDouble(key)) {
    cell.value() = map->getDouble(key);
  } else if (map->isInt64(key)) {
    // store as number so get<double> round-trips
    cell.value() = static_cast<double>(map->getInt64(key));
  } else if (map->isBoolean(key)) {
    cell.value() = map->getBoolean(key);
  }
  // arrays / objects are not representable as a single cell — skipped
}

// AnyMap is backed by std::unordered_map — sort each row's keys so column
// order is deterministic, then keep first-seen order across rows.
std::vector<std::string> collectKeys(const std::vector<std::shared_ptr<AnyMap>>& data) {
  std::vector<std::string> keys;
  for (const auto& row : data) {
    if (!row) continue;
    auto rowKeys = row->getAllKeys();
    std::sort(rowKeys.begin(), rowKeys.end());
    for (const auto& key : rowKeys) {
      bool known = false;
      for (const auto& existing : keys) {
        if (existing == key) { known = true; break; }
      }
      if (!known) keys.push_back(key);
    }
  }
  return keys;
}

} // anonymous namespace

std::vector<std::shared_ptr<AnyMap>> worksheetToRecords(
    OpenXLSX::XLWorksheet& worksheet,
    const std::optional<std::vector<std::string>>& keys) {
  std::vector<std::shared_ptr<AnyMap>> result;
  uint32_t lastRow = worksheet.rowCount();
  uint16_t lastCol = worksheet.columnCount();
  if (lastRow == 0 || lastCol == 0) return result;

  uint32_t dataStartRow = 1;
  std::vector<std::string> resolvedKeys;

  if (keys.has_value()) {
    resolvedKeys = *keys;
  } else {
    dataStartRow = 2;
    for (uint16_t c = 1; c <= lastCol; ++c) {
      resolvedKeys.push_back(keyFromCellValue(worksheet, 1, c));
    }
  }
  if (resolvedKeys.empty()) return result;

  uint16_t keyCount = std::min(static_cast<uint16_t>(resolvedKeys.size()), lastCol);
  for (uint32_t r = dataStartRow; r <= lastRow; ++r) {
    bool hasData = false;
    for (uint16_t c = 1; c <= keyCount; ++c) {
      if (!isCellEmpty(worksheet, r, c)) {
        hasData = true;
        break;
      }
    }
    if (!hasData) continue;

    auto record = AnyMap::make();
    recordFromRow(worksheet, r, resolvedKeys, lastCol, record);
    result.push_back(record);
  }
  return result;
}

void recordsToWorksheet(
    OpenXLSX::XLWorksheet& worksheet,
    const std::vector<std::shared_ptr<AnyMap>>& data) {
  if (data.empty()) return;

  // Column order: first-seen keys across rows; each row's keys sorted (AnyMap
  // is unordered, so sorting keeps the layout deterministic).
  std::vector<std::string> keys = collectKeys(data);

  // Header row
  for (size_t c = 0; c < keys.size(); ++c) {
    worksheet.cell(1, static_cast<uint16_t>(c + 1)).value() = keys[c];
  }

  // Data rows
  for (size_t r = 0; r < data.size(); ++r) {
    const auto& row = data[r];
    if (!row) continue;
    uint32_t rowNum = static_cast<uint32_t>(r + 2);
    for (size_t c = 0; c < keys.size(); ++c) {
      writeValue(worksheet, rowNum, static_cast<uint16_t>(c + 1), row, keys[c]);
    }
  }
}

std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>> workbookToRecords(
    OpenXLSX::XLWorkbook& workbook) {
  std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>> result;
  uint16_t count = static_cast<uint16_t>(workbook.worksheetCount());
  for (uint16_t i = 1; i <= count; ++i) {
    auto ws = workbook.worksheet(i);
    result[ws.name()] = worksheetToRecords(ws);
  }
  return result;
}

} // namespace margelo::nitro::xlsx
