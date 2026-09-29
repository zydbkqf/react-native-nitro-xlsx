#pragma once

#include "HybridXlsxWorksheetSpec.hpp"
#include <OpenXLSX/XLDocument.hpp>
#include <OpenXLSX/XLSheet.hpp>
#include <OpenXLSX/XLComments.hpp>
#include <string>
#include <memory>
#include "HybridXlsxCellFormat.hpp"
#include "HybridXlsxConditionalFormats.hpp"

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
  bool deleteRow(double row) override;
  bool deleteColumn(double col) override;

  void mergeRange(double firstRow, double firstCol, double lastRow, double lastCol, const std::string& value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void mergeRangeNum(double firstRow, double firstCol, double lastRow, double lastCol, double value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) override;
  void unmergeCells(double firstRow, double firstCol, double lastRow, double lastCol) override;

  void hide() override;
  void activate() override;

  // Sheet protection
  void protect(const std::optional<std::string>& password) override;
  void protectSheet(std::optional<bool> set) override;
  void protectObjects(std::optional<bool> set) override;
  void protectScenarios(std::optional<bool> set) override;

  // Password
  void setPassword(const std::string& password) override;
  void setPasswordHash(const std::string& hash) override;
  void clearPassword() override;
  void clearSheetProtection() override;

  // Fine-grained permissions
  void allowInsertColumns(std::optional<bool> set) override;
  void allowInsertRows(std::optional<bool> set) override;
  void allowDeleteColumns(std::optional<bool> set) override;
  void allowDeleteRows(std::optional<bool> set) override;
  void allowSelectLockedCells(std::optional<bool> set) override;
  void allowSelectUnlockedCells(std::optional<bool> set) override;

  void denyInsertColumns() override;
  void denyInsertRows() override;
  void denyDeleteColumns() override;
  void denyDeleteRows() override;
  void denySelectLockedCells() override;
  void denySelectUnlockedCells() override;

  // Protection state
  bool sheetProtected() override;
  bool objectsProtected() override;
  bool scenariosProtected() override;
  bool insertColumnsAllowed() override;
  bool insertRowsAllowed() override;
  bool deleteColumnsAllowed() override;
  bool deleteRowsAllowed() override;
  bool selectLockedCellsAllowed() override;
  bool selectUnlockedCellsAllowed() override;
  bool passwordIsSet() override;
  std::string passwordHash() override;
  std::string sheetProtectionSummary() override;

  void setColumnHidden(double firstCol, double lastCol, bool hidden) override;
  void setRowHidden(double row, bool hidden) override;

  // Comments (XLComments)
  void setComment(double row, double col, const std::string& text, std::optional<double> authorId) override;
  std::string getComment(double row, double col) override;
  bool hasComment(double row, double col) override;
  bool deleteComment(double row, double col) override;
  double getCommentCount() override;
  double addCommentAuthor(const std::string& author) override;
  std::string getCommentAuthor(double authorId) override;

  // Conditional formatting
  std::shared_ptr<HybridXlsxConditionalFormatsSpec> getConditionalFormats() override;

  // Read methods
  std::variant<bool, nitro::NullType, std::string, double> getCellValue(double row, double col) override;
  std::string getCellString(double row, double col) override;
  std::string getCellRawValue(double row, double col) override;
  CellType getCellType(double row, double col) override;
  std::shared_ptr<HybridXlsxCellFormatSpec> getCellFormat(double row, double col) override;
  bool hasFormula(double row, double col) override;
  std::string formula(double row, double col) override;
  bool findCell(double row, double col) override;
  double getRowCount() override;
  double getColumnCount() override;
  double getLastRow() override;
  double getLastColumn() override;
  std::string getName() override;

  // JSON import / export for this worksheet
  std::vector<std::shared_ptr<AnyMap>> toJSON(const std::optional<std::vector<std::string>>& keys) override;
  void fromJSON(const std::vector<std::shared_ptr<AnyMap>>& data) override;

  // Internal: rename this sheet and rewrite formula references
  void applyRename(const std::string& oldName, const std::string& newName);
  // Internal: rewrite formula references to a renamed sheet (no actual rename)
  void rewriteSheetNameRef(const std::string& oldName, const std::string& newName);

private:
  OpenXLSX::XLDocument& _doc;
  OpenXLSX::XLWorksheet _worksheet;

  void applyFormat(OpenXLSX::XLCell& cell, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format);
  std::string cellAddress(double row, double col);
};

}