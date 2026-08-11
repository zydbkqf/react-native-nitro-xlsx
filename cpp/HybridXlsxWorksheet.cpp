#include "HybridXlsxWorksheet.hpp"
#include <cstring>
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
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.value() = value;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeNumber(double row, double col, double value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.value() = value;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeBoolean(double row, double col, bool value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.value() = value;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeBlank(double row, double col, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeFormula(double row, double col, const std::string& formula, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.formula() = formula;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeFormulaNum(double row, double col, const std::string& formula, double number, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.formula() = formula;
  cell.value() = number;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeFormulaString(double row, double col, const std::string& formula, const std::string& str, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.formula() = formula;
  cell.value() = str;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeFormulaBoolean(double row, double col, const std::string& formula, bool boolVal, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.formula() = formula;
  cell.value() = boolVal;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeArrayFormula(double firstRow, double firstCol, double lastRow, double lastCol, const std::string& formula, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol));
  cell.formula() = formula;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeDatetime(double row, double col, double datetime, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.value() = datetime;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeURL(double row, double col, const std::string& url, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.value() = url;
  applyFormat(cell, format);
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

void HybridXlsxWorksheet::insertImage(double row, double col, const std::string& path, std::optional<double> xOffset, std::optional<double> yOffset, std::optional<double> xScale, std::optional<double> yScale) {
}

void HybridXlsxWorksheet::insertImageBuffer(double row, double col, const std::shared_ptr<ArrayBuffer>& buffer, const std::string& extension, std::optional<double> xOffset, std::optional<double> yOffset, std::optional<double> xScale, std::optional<double> yScale) {
}

void HybridXlsxWorksheet::autofilter(double firstRow, double firstCol, double lastRow, double lastCol) {
}

void HybridXlsxWorksheet::freezePanes(double row, double col) {
}

void HybridXlsxWorksheet::splitPanes(double row, double col) {
}

void HybridXlsxWorksheet::setTabColor(double color) {
}

void HybridXlsxWorksheet::hide() {
}

void HybridXlsxWorksheet::activate() {
  _worksheet.setActive();
}

void HybridXlsxWorksheet::setFirstSheet() {
}

void HybridXlsxWorksheet::protect(const std::optional<std::string>& password) {
  if (password.has_value() && !password->empty()) {
    _worksheet.setPassword(*password);
  }
  _worksheet.protectSheet();
}

void HybridXlsxWorksheet::setPortrait() {
}

void HybridXlsxWorksheet::setLandscape() {
}

void HybridXlsxWorksheet::setPaper(double paperType) {
}

void HybridXlsxWorksheet::setHeader(const std::string& header) {
}

void HybridXlsxWorksheet::setFooter(const std::string& footer) {
}

void HybridXlsxWorksheet::setPrintArea(double firstRow, double firstCol, double lastRow, double lastCol) {
}

void HybridXlsxWorksheet::fitToPages(double width, double height) {
}

void HybridXlsxWorksheet::setZoom(double scale) {
}

void HybridXlsxWorksheet::setGridlines(double option) {
}

void HybridXlsxWorksheet::centerHorizontally() {
}

void HybridXlsxWorksheet::centerVertically() {
}

void HybridXlsxWorksheet::setOutline(double level) {
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

}