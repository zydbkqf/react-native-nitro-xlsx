#include "HybridXlsxWorksheet.hpp"
#include "XlsxError.hpp"
#include "XlsxJson.hpp"
#include <OpenXLSX/XLCellReference.hpp>
#include <cstring>
#include <cmath>
#include <fstream>
#include <chrono>

namespace margelo::nitro::xlsx {

HybridXlsxWorksheet::HybridXlsxWorksheet(OpenXLSX::XLDocument& doc, OpenXLSX::XLWorksheet worksheet)
    : HybridObject("XlsxWorksheet"), HybridXlsxWorksheetSpec(), _doc(doc), _worksheet(worksheet) {
}

HybridXlsxWorksheet::~HybridXlsxWorksheet() {
}

void HybridXlsxWorksheet::applyFormat(OpenXLSX::XLCell& cell, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  if (!format.has_value()) return;
  
  auto hybridFormat = std::dynamic_pointer_cast<HybridXlsxCellFormat>(*format);
  if (!hybridFormat) return;
  
  auto styleIndex = hybridFormat->applyToDocument(_doc.styles());
  hybridFormat->applyToCell(cell, styleIndex);
}

void HybridXlsxWorksheet::writeString(double row, double col, const std::string& value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
    cell.value() = value;
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::writeNumber(double row, double col, double value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
    cell.value() = value;
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::writeBoolean(double row, double col, bool value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
    cell.value() = value;
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::writeBlank(double row, double col, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::writeFormula(double row, double col, const std::string& formula, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
    cell.formula() = formula;
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::writeFormulaNum(double row, double col, const std::string& formula, double number, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
    cell.formula() = formula;
    cell.value() = number;
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::writeFormulaString(double row, double col, const std::string& formula, const std::string& str, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
    cell.formula() = formula;
    cell.value() = str;
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::writeFormulaBoolean(double row, double col, const std::string& formula, bool boolVal, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
    cell.formula() = formula;
    cell.value() = boolVal;
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::writeArrayFormula(double firstRow, double firstCol, double lastRow, double lastCol, const std::string& formula, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol));
    cell.formula() = formula;
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::writeDatetime(double row, double col, double datetime, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
    cell.value() = datetime;
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::writeURL(double row, double col, const std::string& url, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
    cell.value() = url;
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::setColumn(double firstCol, double lastCol, double width, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  for (unsigned int c = static_cast<unsigned int>(firstCol); c <= static_cast<unsigned int>(lastCol); ++c) {
    _worksheet.column(c).setWidth(static_cast<float>(width));
  }
  if (format.has_value()) {
    // Apply format to entire column range by setting it on cells in the first row
    // (OpenXLSX does not support column-level style; apply to first row cells as a hint)
    auto hybridFormat = std::dynamic_pointer_cast<HybridXlsxCellFormat>(*format);
    if (hybridFormat) {
      auto styleIndex = hybridFormat->applyToDocument(_doc.styles());
      for (unsigned int c = static_cast<unsigned int>(firstCol); c <= static_cast<unsigned int>(lastCol); ++c) {
        OpenXLSX::XLCell cell = _worksheet.cell(1, c);
        hybridFormat->applyToCell(cell, styleIndex);
      }
    }
  }
}

void HybridXlsxWorksheet::setRow(double row, double height, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  _worksheet.row(static_cast<unsigned int>(row)).setHeight(static_cast<float>(height));
  if (format.has_value()) {
    auto hybridFormat = std::dynamic_pointer_cast<HybridXlsxCellFormat>(*format);
    if (hybridFormat) {
      auto styleIndex = hybridFormat->applyToDocument(_doc.styles());
      unsigned int colCount = _worksheet.columnCount();
      for (unsigned int c = 1; c <= colCount; ++c) {
        OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), c);
        hybridFormat->applyToCell(cell, styleIndex);
      }
    }
  }
}

bool HybridXlsxWorksheet::deleteRow(double row) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return _worksheet.deleteRow(static_cast<uint32_t>(row));
  });
}

bool HybridXlsxWorksheet::deleteColumn(double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    unsigned int targetCol = static_cast<unsigned int>(col);
    if (targetCol < 1) {
      throw XlsxError(xlsx_error::INVALID_ARGUMENT, "Column index must be >= 1");
    }

    unsigned int lastCol = _worksheet.columnCount();
    unsigned int lastRow = _worksheet.rowCount();
    if (targetCol > lastCol) {
      return false;
    }

    // Shift cell contents left for every row
    for (unsigned int r = 1; r <= lastRow; ++r) {
      for (unsigned int c = targetCol + 1; c <= lastCol; ++c) {
        OpenXLSX::XLCell src = _worksheet.cell(r, c);
        OpenXLSX::XLCell dst = _worksheet.cell(r, c - 1);
        if (!src.empty()) {
          dst.copyFrom(src);
          dst.setCellFormat(src.cellFormat());
        } else {
          dst.clear(0);
        }
      }
      // Clear the trailing column
      OpenXLSX::XLCell trailing = _worksheet.cell(r, lastCol);
      trailing.clear(0);
    }

    // Shift column widths / hidden state left
    for (unsigned int c = targetCol + 1; c <= lastCol; ++c) {
      OpenXLSX::XLColumn srcCol = _worksheet.column(c);
      OpenXLSX::XLColumn dstCol = _worksheet.column(c - 1);
      dstCol.setWidth(srcCol.width());
      dstCol.setHidden(srcCol.isHidden());
    }

    return true;
  });
}

void HybridXlsxWorksheet::mergeRange(double firstRow, double firstCol, double lastRow, double lastCol, const std::string& value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCellReference topLeft(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol));
  OpenXLSX::XLCellReference bottomRight(static_cast<unsigned int>(lastRow), static_cast<unsigned int>(lastCol));
  OpenXLSX::XLCellRange range = _worksheet.range(topLeft, bottomRight);
  _worksheet.mergeCells(range);
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol));
  cell.value() = value;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::mergeRangeNum(double firstRow, double firstCol, double lastRow, double lastCol, double value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCellReference topLeft(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol));
  OpenXLSX::XLCellReference bottomRight(static_cast<unsigned int>(lastRow), static_cast<unsigned int>(lastCol));
  OpenXLSX::XLCellRange range = _worksheet.range(topLeft, bottomRight);
  _worksheet.mergeCells(range);
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol));
  cell.value() = value;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::unmergeCells(double firstRow, double firstCol, double lastRow, double lastCol) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCellReference topLeft(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol));
    OpenXLSX::XLCellReference bottomRight(static_cast<unsigned int>(lastRow), static_cast<unsigned int>(lastCol));
    std::string rangeRef = topLeft.address() + ":" + bottomRight.address();
    _worksheet.unmergeCells(rangeRef);
  });
}

void HybridXlsxWorksheet::hide() {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    _worksheet.setVisibility(OpenXLSX::XLSheetState::Hidden);
  });
}

void HybridXlsxWorksheet::activate() {
  _worksheet.setActive();
}

// ========== Sheet protection ==========

namespace {
  // OpenXLSX defaults `set` to true when the argument is omitted
  bool resolveSet(const std::optional<bool>& set) {
    return set.value_or(true);
  }
}

void HybridXlsxWorksheet::protect(const std::optional<std::string>& password) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (password.has_value() && !password->empty()) {
      if (!_worksheet.setPassword(*password)) {
        throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set sheet password");
      }
    }
    if (!_worksheet.protectSheet()) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to protect sheet");
    }
  });
}

void HybridXlsxWorksheet::protectSheet(std::optional<bool> set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.protectSheet(resolveSet(set))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set protectSheet");
    }
  });
}

void HybridXlsxWorksheet::protectObjects(std::optional<bool> set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.protectObjects(resolveSet(set))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set protectObjects");
    }
  });
}

void HybridXlsxWorksheet::protectScenarios(std::optional<bool> set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.protectScenarios(resolveSet(set))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set protectScenarios");
    }
  });
}

// ========== Password ==========

void HybridXlsxWorksheet::setPassword(const std::string& password) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.setPassword(password)) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set password");
    }
  });
}

void HybridXlsxWorksheet::setPasswordHash(const std::string& hash) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.setPasswordHash(hash)) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set password hash");
    }
  });
}

void HybridXlsxWorksheet::clearPassword() {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.clearPassword()) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to clear password");
    }
  });
}

void HybridXlsxWorksheet::clearSheetProtection() {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.clearSheetProtection()) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to clear sheet protection");
    }
  });
}

// ========== Fine-grained permissions ==========

void HybridXlsxWorksheet::allowInsertColumns(std::optional<bool> set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.allowInsertColumns(resolveSet(set))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set allowInsertColumns");
    }
  });
}

void HybridXlsxWorksheet::allowInsertRows(std::optional<bool> set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.allowInsertRows(resolveSet(set))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set allowInsertRows");
    }
  });
}

void HybridXlsxWorksheet::allowDeleteColumns(std::optional<bool> set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.allowDeleteColumns(resolveSet(set))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set allowDeleteColumns");
    }
  });
}

void HybridXlsxWorksheet::allowDeleteRows(std::optional<bool> set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.allowDeleteRows(resolveSet(set))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set allowDeleteRows");
    }
  });
}

void HybridXlsxWorksheet::allowSelectLockedCells(std::optional<bool> set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.allowSelectLockedCells(resolveSet(set))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set allowSelectLockedCells");
    }
  });
}

void HybridXlsxWorksheet::allowSelectUnlockedCells(std::optional<bool> set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.allowSelectUnlockedCells(resolveSet(set))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set allowSelectUnlockedCells");
    }
  });
}

void HybridXlsxWorksheet::denyInsertColumns() {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.denyInsertColumns()) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to denyInsertColumns");
    }
  });
}

void HybridXlsxWorksheet::denyInsertRows() {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.denyInsertRows()) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to denyInsertRows");
    }
  });
}

void HybridXlsxWorksheet::denyDeleteColumns() {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.denyDeleteColumns()) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to denyDeleteColumns");
    }
  });
}

void HybridXlsxWorksheet::denyDeleteRows() {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.denyDeleteRows()) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to denyDeleteRows");
    }
  });
}

void HybridXlsxWorksheet::denySelectLockedCells() {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.denySelectLockedCells()) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to denySelectLockedCells");
    }
  });
}

void HybridXlsxWorksheet::denySelectUnlockedCells() {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_worksheet.denySelectUnlockedCells()) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to denySelectUnlockedCells");
    }
  });
}

// ========== Protection state ==========

bool HybridXlsxWorksheet::sheetProtected() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.sheetProtected(); });
}

bool HybridXlsxWorksheet::objectsProtected() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.objectsProtected(); });
}

bool HybridXlsxWorksheet::scenariosProtected() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.scenariosProtected(); });
}

bool HybridXlsxWorksheet::insertColumnsAllowed() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.insertColumnsAllowed(); });
}

bool HybridXlsxWorksheet::insertRowsAllowed() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.insertRowsAllowed(); });
}

bool HybridXlsxWorksheet::deleteColumnsAllowed() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.deleteColumnsAllowed(); });
}

bool HybridXlsxWorksheet::deleteRowsAllowed() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.deleteRowsAllowed(); });
}

bool HybridXlsxWorksheet::selectLockedCellsAllowed() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.selectLockedCellsAllowed(); });
}

bool HybridXlsxWorksheet::selectUnlockedCellsAllowed() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.selectUnlockedCellsAllowed(); });
}

bool HybridXlsxWorksheet::passwordIsSet() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.passwordIsSet(); });
}

std::string HybridXlsxWorksheet::passwordHash() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.passwordHash(); });
}

std::string HybridXlsxWorksheet::sheetProtectionSummary() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _worksheet.sheetProtectionSummary(); });
}

void HybridXlsxWorksheet::setColumnHidden(double firstCol, double lastCol, bool hidden) {
  for (unsigned int c = static_cast<unsigned int>(firstCol); c <= static_cast<unsigned int>(lastCol); ++c) {
    _worksheet.column(c).setHidden(hidden);
  }
}

void HybridXlsxWorksheet::setRowHidden(double row, bool hidden) {
  _worksheet.row(static_cast<unsigned int>(row)).setHidden(hidden);
}

std::variant<bool, nitro::NullType, std::string, double> HybridXlsxWorksheet::getCellValue(double row, double col) {
  auto cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  auto type = cell.value().type();
  
  switch (type) {
    case OpenXLSX::XLValueType::String:
      return cell.value().get<std::string>();
    case OpenXLSX::XLValueType::Integer:
      return static_cast<double>(cell.value().get<int64_t>());
    case OpenXLSX::XLValueType::Float:
      return cell.value().get<double>();
    case OpenXLSX::XLValueType::Boolean:
      return cell.value().get<bool>();
    case OpenXLSX::XLValueType::Empty:
    case OpenXLSX::XLValueType::Error:
    default:
      return nitro::NullType{};
  }
}

std::string HybridXlsxWorksheet::getCellString(double row, double col) {
  auto cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  auto type = cell.value().type();

  switch (type) {
    case OpenXLSX::XLValueType::String:
      return cell.value().get<std::string>();
    case OpenXLSX::XLValueType::Integer:
      return std::to_string(cell.value().get<int64_t>());
    case OpenXLSX::XLValueType::Float:
      return std::to_string(cell.value().get<double>());
    case OpenXLSX::XLValueType::Boolean:
      return cell.value().get<bool>() ? "TRUE" : "FALSE";
    case OpenXLSX::XLValueType::Empty:
    case OpenXLSX::XLValueType::Error:
    default:
      return "";
  }
}

std::string HybridXlsxWorksheet::getCellRawValue(double row, double col) {
  auto cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));

  if (cell.hasFormula()) {
    return cell.formula().get();
  }

  auto type = cell.value().type();
  switch (type) {
    case OpenXLSX::XLValueType::String:
      return cell.value().get<std::string>();
    case OpenXLSX::XLValueType::Integer:
      return std::to_string(cell.value().get<int64_t>());
    case OpenXLSX::XLValueType::Float:
      return std::to_string(cell.value().get<double>());
    case OpenXLSX::XLValueType::Boolean:
      return cell.value().get<bool>() ? "TRUE" : "FALSE";
    case OpenXLSX::XLValueType::Empty:
    case OpenXLSX::XLValueType::Error:
    default:
      return "";
  }
}

CellType HybridXlsxWorksheet::getCellType(double row, double col) {
  auto cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  auto type = cell.value().type();

  if (cell.hasFormula()) {
    return CellType::FORMULA;
  }

  switch (type) {
    case OpenXLSX::XLValueType::Empty:
      return CellType::EMPTY;
    case OpenXLSX::XLValueType::String:
      return CellType::STRING;
    case OpenXLSX::XLValueType::Integer:
    case OpenXLSX::XLValueType::Float:
      return CellType::NUMBER;
    case OpenXLSX::XLValueType::Boolean:
      return CellType::BOOLEAN;
    case OpenXLSX::XLValueType::Error:
      return CellType::ERROR;
    default:
      return CellType::EMPTY;
  }
}

std::shared_ptr<HybridXlsxCellFormatSpec> HybridXlsxWorksheet::getCellFormat(double row, double col) {
  auto cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  auto format = std::make_shared<HybridXlsxCellFormat>();
  return format;
}

double HybridXlsxWorksheet::getRowCount() {
  return static_cast<double>(_worksheet.rowCount());
}

double HybridXlsxWorksheet::getColumnCount() {
  return static_cast<double>(_worksheet.columnCount());
}

double HybridXlsxWorksheet::getLastRow() {
  auto last = _worksheet.lastCell();
  return static_cast<double>(last.row());
}

double HybridXlsxWorksheet::getLastColumn() {
  auto last = _worksheet.lastCell();
  return static_cast<double>(last.column());
}

std::string HybridXlsxWorksheet::getName() {
  return _worksheet.name();
}

void HybridXlsxWorksheet::applyRename(const std::string& oldName, const std::string& newName) {
  _worksheet.setName(newName);
  _worksheet.updateSheetName(oldName, newName);
}

void HybridXlsxWorksheet::rewriteSheetNameRef(const std::string& oldName, const std::string& newName) {
  _worksheet.updateSheetName(oldName, newName);
}

// ========== JSON import / export (this worksheet) ==========

std::vector<std::shared_ptr<AnyMap>> HybridXlsxWorksheet::toJSON(const std::optional<std::vector<std::string>>& keys) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return worksheetToRecords(_worksheet, keys);
  });
}

void HybridXlsxWorksheet::fromJSON(const std::vector<std::shared_ptr<AnyMap>>& data) {
  withXlsxError(xlsx_error::INVALID_ARGUMENT, [&] {
    recordsToWorksheet(_worksheet, data);
  });
}

std::string HybridXlsxWorksheet::cellAddress(double row, double col) {
  return OpenXLSX::XLCellReference(static_cast<uint32_t>(row), static_cast<uint16_t>(col)).address();
}

// ========== Comments ==========

void HybridXlsxWorksheet::setComment(double row, double col, const std::string& text, std::optional<double> authorId) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    std::string ref = cellAddress(row, col);
    auto& comments = _worksheet.comments();
    // Excel requires every comment's authorId to reference a real <author> entry.
    // OpenXLSX does not auto-register authors, so ensure at least one exists here;
    // otherwise the generated comments.xml has an empty <authors> + a dangling authorId,
    // which Excel flags as "We found a problem with some content".
    if (comments.authorCount() == 0) {
      comments.addAuthor("Author"); // becomes index 0
    }
    uint16_t author = authorId.has_value() ? static_cast<uint16_t>(*authorId) : 0;
    if (author >= comments.authorCount()) {
      author = 0; // fall back to a valid author
    }
    if (!comments.set(ref, text, author)) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set comment for cell " + ref);
    }
  });
}

std::string HybridXlsxWorksheet::getComment(double row, double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return _worksheet.comments().get(cellAddress(row, col));
  });
}

bool HybridXlsxWorksheet::hasComment(double row, double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    std::string text = _worksheet.comments().get(cellAddress(row, col));
    return !text.empty();
  });
}

bool HybridXlsxWorksheet::deleteComment(double row, double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return _worksheet.comments().deleteComment(cellAddress(row, col));
  });
}

double HybridXlsxWorksheet::getCommentCount() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(_worksheet.comments().count());
  });
}

double HybridXlsxWorksheet::addCommentAuthor(const std::string& author) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(_worksheet.comments().addAuthor(author));
  });
}

std::string HybridXlsxWorksheet::getCommentAuthor(double authorId) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return _worksheet.comments().author(static_cast<uint16_t>(authorId));
  });
}

// ========== Conditional formatting ==========

std::shared_ptr<HybridXlsxConditionalFormatsSpec> HybridXlsxWorksheet::getConditionalFormats() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return std::static_pointer_cast<HybridXlsxConditionalFormatsSpec>(
        std::make_shared<HybridXlsxConditionalFormats>(_worksheet.conditionalFormats()));
  });
}

// ========== Formula / findCell ==========

bool HybridXlsxWorksheet::hasFormula(double row, double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCellAssignable cell = _worksheet.findCell(static_cast<uint32_t>(row), static_cast<uint16_t>(col));
    if (cell.empty()) return false;
    return cell.hasFormula();
  });
}

std::string HybridXlsxWorksheet::formula(double row, double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCellAssignable cell = _worksheet.findCell(static_cast<uint32_t>(row), static_cast<uint16_t>(col));
    if (cell.empty() || !cell.hasFormula()) return std::string();
    return cell.formula().get();
  });
}

bool HybridXlsxWorksheet::findCell(double row, double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCellAssignable cell = _worksheet.findCell(static_cast<uint32_t>(row), static_cast<uint16_t>(col));
    return !cell.empty();
  });
}

}