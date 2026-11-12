#include "XlsxJson.hpp"
#include <algorithm>
#include <charconv>
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
    case OpenXLSX::XLValueType::Float: {
      double d = cell.value().get<double>();
      // Integral values in int64 range render exactly like the Integer branch.
      // Bounds are the int64 range as doubles: [-2^63, 2^63).
      constexpr double kInt64Min = -9223372036854775808.0;
      constexpr double kInt64MaxExclusive = 9223372036854775808.0;
      if (std::isfinite(d) && d == std::trunc(d) && d >= kInt64Min && d < kInt64MaxExclusive) {
        return std::to_string(static_cast<long long>(d));
      }
      // Shortest representation that round-trips exactly: no truncation to int,
      // so headers 3.5 and 3.9 stay distinct ("3.5" / "3.9", not both "3").
      char buf[40];
      auto [end, ec] = std::to_chars(buf, buf + sizeof(buf), d);
      if (ec == std::errc()) return std::string(buf, end);
      return std::to_string(d);
    }
    default:
      return "col" + std::to_string(col);
  }
}

void recordFromRow(OpenXLSX::XLWorksheet& ws,
                   uint32_t row,
                   const std::vector<std::string>& keys,
                   uint16_t lastCol,
                   std::shared_ptr<AnyMap> record) {
  for (size_t c = 0; c < keys.size(); ++c) {
    const std::string& key = keys[c];
    if (c >= static_cast<size_t>(lastCol)) {
      // Column beyond the sheet's width: emit an explicit null instead of dropping the key.
      record->setNull(key);
      continue;
    }
    auto cell = ws.findCell(row, static_cast<uint16_t>(c + 1));
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

  // Keys beyond lastCol can never hold data, so the has-data scan only needs to
  // cover the sheet's columns; recordFromRow still emits every key (null-filled).
  size_t scanCols = std::min(resolvedKeys.size(), static_cast<size_t>(lastCol));
  for (uint32_t r = dataStartRow; r <= lastRow; ++r) {
    bool hasData = false;
    for (size_t c = 1; c <= scanCols; ++c) {
      if (!isCellEmpty(worksheet, r, static_cast<uint16_t>(c))) {
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
