#pragma once

#include "HybridXlsxWorksheetSpec.hpp"
#include <OpenXLSX/XLDocument.hpp>
#include <OpenXLSX/XLSheet.hpp>
#include <string>
#include <memory>
#include "HybridXlsxCellFormat.hpp"

namespace margelo::nitro::xlsx {

class HybridXlsxWorksheet : public HybridXlsxWorksheetSpec {
public:
  HybridXlsxWorksheet(OpenXLSX::XLDocument& doc, OpenXLSX::XLWorksheet worksheet);
  ~HybridXlsxWorksheet() override;

  void writeString(double row, double col, const std::string& value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void writeNumber(double row, double col, double value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void writeBoolean(double row, double col, bool value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void writeBlank(double row, double col, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  
  void writeFormula(double row, double col, const std::string& formula, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void writeFormulaNum(double row, double col, const std::string& formula, double number, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void writeFormulaString(double row, double col, const std::string& formula, const std::string& str, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void writeFormulaBoolean(double row, double col, const std::string& formula, bool boolVal, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void writeArrayFormula(double firstRow, double firstCol, double lastRow, double lastCol, const std::string& formula, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  
  void writeDatetime(double row, double col, double datetime, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void writeURL(double row, double col, const std::string& url, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  
  void setColumn(double firstCol, double lastCol, double width, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void setRow(double row, double height, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  
  void mergeRange(double firstRow, double firstCol, double lastRow, double lastCol, const std::string& value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void mergeRangeNum(double firstRow, double firstCol, double lastRow, double lastCol, double value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  
  void insertImage(double row, double col, const std::string& path, std::optional<double> xOffset, std::optional<double> yOffset, std::optional<double> xScale, std::optional<double> yScale) override;
  void insertImageBuffer(double row, double col, const std::shared_ptr<ArrayBuffer>& buffer, const std::string& extension, std::optional<double> xOffset, std::optional<double> yOffset, std::optional<double> xScale, std::optional<double> yScale) override;
  
  void autofilter(double firstRow, double firstCol, double lastRow, double lastCol) override;
  
  void freezePanes(double row, double col) override;
  void splitPanes(double row, double col) override;
  
  void setTabColor(double color) override;
  void hide() override;
  void activate() override;
  void setFirstSheet() override;
  void protect(const std::optional<std::string>& password) override;
  
  void setPortrait() override;
  void setLandscape() override;
  void setPaper(double paperType) override;
  void setHeader(const std::string& header) override;
  void setFooter(const std::string& footer) override;
  void setPrintArea(double firstRow, double firstCol, double lastRow, double lastCol) override;
  void fitToPages(double width, double height) override;
  void setZoom(double scale) override;
  void setGridlines(double option) override;
  void centerHorizontally() override;
  void centerVertically() override;
  
  void setOutline(double level) override;
  void setColumnHidden(double firstCol, double lastCol, bool hidden) override;
  void setRowHidden(double row, bool hidden) override;

  // Read methods
  std::variant<bool, nitro::NullType, std::string, double> getCellValue(double row, double col) override;
  std::string getCellString(double row, double col) override;
  std::string getCellRawValue(double row, double col) override;
  CellType getCellType(double row, double col) override;
  std::shared_ptr<HybridXlsxCellFormatSpec> getCellFormat(double row, double col) override;
  double getRowCount() override;
  double getColumnCount() override;
  double getLastRow() override;
  double getLastColumn() override;
  std::string getName() override;

private:
  OpenXLSX::XLDocument& _doc;
  OpenXLSX::XLWorksheet _worksheet;

  void applyFormat(OpenXLSX::XLCell& cell, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format);
};

}