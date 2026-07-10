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

void HybridXlsxWorksheet::applyFormat(OpenXLSX::XLCell& cell, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  if (!format.has_value()) return;
  
  auto hybridFormat = std::dynamic_pointer_cast<HybridXlsxFormat>(*format);
  if (!hybridFormat) return;
  
  hybridFormat->applyToCell(cell);
}

void HybridXlsxWorksheet::writeString(double row, double col, const std::string& value, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.value() = value;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeNumber(double row, double col, double value, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.value() = value;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeBoolean(double row, double col, bool value, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.value() = value;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeBlank(double row, double col, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeFormula(double row, double col, const std::string& formula, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.formula() = formula;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeFormulaNum(double row, double col, const std::string& formula, double number, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.formula() = formula;
  cell.value() = number;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeFormulaString(double row, double col, const std::string& formula, const std::string& str, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.formula() = formula;
  cell.value() = str;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeFormulaBoolean(double row, double col, const std::string& formula, bool boolVal, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.formula() = formula;
  cell.value() = boolVal;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeArrayFormula(double firstRow, double firstCol, double lastRow, double lastCol, const std::string& formula, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCellRange range = _worksheet.range(
    static_cast<unsigned int>(firstRow), 
    static_cast<unsigned int>(firstCol), 
    static_cast<unsigned int>(lastRow), 
    static_cast<unsigned int>(lastCol)
  );
  range.formula() = formula;
}

void HybridXlsxWorksheet::writeDatetime(double row, double col, double datetime, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.value() = datetime;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::writeURL(double row, double col, const std::string& url, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
  cell.value() = url;
  cell.hyperlink() = url;
  applyFormat(cell, format);
}

void HybridXlsxWorksheet::setColumn(double firstCol, double lastCol, double width, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  for (unsigned int c = static_cast<unsigned int>(firstCol); c <= static_cast<unsigned int>(lastCol); ++c) {
    _worksheet.column(c).width(width);
  }
}

void HybridXlsxWorksheet::setRow(double row, double height, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  _worksheet.row(static_cast<unsigned int>(row)).height(height);
}

void HybridXlsxWorksheet::mergeRange(double firstRow, double firstCol, double lastRow, double lastCol, const std::string& value, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCellRange range = _worksheet.range(
    static_cast<unsigned int>(firstRow), 
    static_cast<unsigned int>(firstCol), 
    static_cast<unsigned int>(lastRow), 
    static_cast<unsigned int>(lastCol)
  );
  range.merge();
  _worksheet.cell(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol)).value() = value;
}

void HybridXlsxWorksheet::mergeRangeNum(double firstRow, double firstCol, double lastRow, double lastCol, double value, const std::optional<std::shared_ptr<HybridXlsxFormatSpec>>& format) {
  OpenXLSX::XLCellRange range = _worksheet.range(
    static_cast<unsigned int>(firstRow), 
    static_cast<unsigned int>(firstCol), 
    static_cast<unsigned int>(lastRow), 
    static_cast<unsigned int>(lastCol)
  );
  range.merge();
  _worksheet.cell(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol)).value() = value;
}

void HybridXlsxWorksheet::insertImage(double row, double col, const std::string& path, std::optional<double> xOffset, std::optional<double> yOffset, std::optional<double> xScale, std::optional<double> yScale) {
  _worksheet.image(static_cast<unsigned int>(row), static_cast<unsigned int>(col)).loadFromFile(path);
}

void HybridXlsxWorksheet::insertImageBuffer(double row, double col, const std::shared_ptr<ArrayBuffer>& buffer, const std::string& extension, std::optional<double> xOffset, std::optional<double> yOffset, std::optional<double> xScale, std::optional<double> yScale) {
  std::string tempPath = "/tmp/xlsx_image_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + "." + extension;
  std::ofstream file(tempPath, std::ios::binary);
  if (file.is_open()) {
    file.write(reinterpret_cast<const char*>(buffer->data()), buffer->size());
    file.close();
    
    insertImage(row, col, tempPath, xOffset, yOffset, xScale, yScale);
    
    std::remove(tempPath.c_str());
  }
}

void HybridXlsxWorksheet::autofilter(double firstRow, double firstCol, double lastRow, double lastCol) {
  OpenXLSX::XLCellRange range = _worksheet.range(
    static_cast<unsigned int>(firstRow), 
    static_cast<unsigned int>(firstCol), 
    static_cast<unsigned int>(lastRow), 
    static_cast<unsigned int>(lastCol)
  );
  _worksheet.autoFilter(range);
}

void HybridXlsxWorksheet::freezePanes(double row, double col) {
  _worksheet.freezePanes(static_cast<unsigned int>(row), static_cast<unsigned int>(col));
}

void HybridXlsxWorksheet::splitPanes(double row, double col) {
  _worksheet.splitPanes(static_cast<int>(row * 20), static_cast<int>(col * 10));
}

void HybridXlsxWorksheet::setTabColor(double color) {
  _worksheet.tabColor(OpenXLSX::XLColor(color));
}

void HybridXlsxWorksheet::hide() {
  _worksheet.isHidden(true);
}

void HybridXlsxWorksheet::activate() {
  _worksheet.isActive(true);
}

void HybridXlsxWorksheet::setFirstSheet() {
}

void HybridXlsxWorksheet::protect(const std::optional<std::string>& password) {
  _worksheet.protect(password.has_value() ? *password : "");
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
  _worksheet.zoom(static_cast<unsigned int>(scale * 100));
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
    _worksheet.column(c).isHidden(hidden);
  }
}

void HybridXlsxWorksheet::setRowHidden(double row, bool hidden) {
  _worksheet.row(static_cast<unsigned int>(row)).isHidden(hidden);
}

}