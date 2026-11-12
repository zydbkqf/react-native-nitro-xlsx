#pragma once

// Test-build stand-in for nitrogen's HybridXlsxWorksheetSpec.hpp.
// Mirrors the pure-virtual interface (signatures must stay in sync) without the
// Nitro/JSI hybrid-registration machinery.

#include <NitroModules/HybridObject.hpp>
#include <NitroModules/Null.hpp>
#include <NitroModules/AnyMap.hpp>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include "CellType.hpp"

namespace margelo::nitro::xlsx {

  class HybridXlsxCellFormatSpec;
  class HybridXlsxConditionalFormatsSpec;

  using namespace margelo::nitro;

  class HybridXlsxWorksheetSpec: public virtual HybridObject {
    public:
      explicit HybridXlsxWorksheetSpec(): HybridObject(TAG) { }
      ~HybridXlsxWorksheetSpec() override = default;

    public:
      virtual void writeString(double row, double col, const std::string& value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void writeNumber(double row, double col, double value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void writeBoolean(double row, double col, bool value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void writeBlank(double row, double col, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void writeFormula(double row, double col, const std::string& formula, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void writeFormulaNum(double row, double col, const std::string& formula, double number, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void writeFormulaString(double row, double col, const std::string& formula, const std::string& str, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void writeFormulaBoolean(double row, double col, const std::string& formula, bool boolVal, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void writeArrayFormula(double firstRow, double firstCol, double lastRow, double lastCol, const std::string& formula, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void writeDatetime(double row, double col, double datetime, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void writeURL(double row, double col, const std::string& url, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void setColumn(double firstCol, double lastCol, double width, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void setRow(double row, double height, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual bool deleteRow(double row) = 0;
      virtual bool deleteColumn(double col) = 0;
      virtual void mergeRange(double firstRow, double firstCol, double lastRow, double lastCol, const std::string& value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void mergeRangeNum(double firstRow, double firstCol, double lastRow, double lastCol, double value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) = 0;
      virtual void unmergeCells(double firstRow, double firstCol, double lastRow, double lastCol) = 0;
      virtual void hide() = 0;
      virtual void activate() = 0;
      virtual void protect(const std::optional<std::string>& password) = 0;
      virtual void protectSheet(std::optional<bool> set) = 0;
      virtual void protectObjects(std::optional<bool> set) = 0;
      virtual void protectScenarios(std::optional<bool> set) = 0;
      virtual void setPassword(const std::string& password) = 0;
      virtual void setPasswordHash(const std::string& hash) = 0;
      virtual void clearPassword() = 0;
      virtual void clearSheetProtection() = 0;
      virtual void allowInsertColumns(std::optional<bool> set) = 0;
      virtual void allowInsertRows(std::optional<bool> set) = 0;
      virtual void allowDeleteColumns(std::optional<bool> set) = 0;
      virtual void allowDeleteRows(std::optional<bool> set) = 0;
      virtual void allowSelectLockedCells(std::optional<bool> set) = 0;
      virtual void allowSelectUnlockedCells(std::optional<bool> set) = 0;
      virtual void denyInsertColumns() = 0;
      virtual void denyInsertRows() = 0;
      virtual void denyDeleteColumns() = 0;
      virtual void denyDeleteRows() = 0;
      virtual void denySelectLockedCells() = 0;
      virtual void denySelectUnlockedCells() = 0;
      virtual bool sheetProtected() = 0;
      virtual bool objectsProtected() = 0;
      virtual bool scenariosProtected() = 0;
      virtual bool insertColumnsAllowed() = 0;
      virtual bool insertRowsAllowed() = 0;
      virtual bool deleteColumnsAllowed() = 0;
      virtual bool deleteRowsAllowed() = 0;
      virtual bool selectLockedCellsAllowed() = 0;
      virtual bool selectUnlockedCellsAllowed() = 0;
      virtual bool passwordIsSet() = 0;
      virtual std::string passwordHash() = 0;
      virtual std::string sheetProtectionSummary() = 0;
      virtual void setColumnHidden(double firstCol, double lastCol, bool hidden) = 0;
      virtual void setRowHidden(double row, bool hidden) = 0;
      virtual void setComment(double row, double col, const std::string& text, std::optional<double> authorId) = 0;
      virtual std::string getComment(double row, double col) = 0;
      virtual bool hasComment(double row, double col) = 0;
      virtual bool deleteComment(double row, double col) = 0;
      virtual double getCommentCount() = 0;
      virtual double addCommentAuthor(const std::string& author) = 0;
      virtual std::string getCommentAuthor(double authorId) = 0;
      virtual std::shared_ptr<HybridXlsxConditionalFormatsSpec> getConditionalFormats() = 0;
      virtual std::variant<bool, nitro::NullType, std::string, double> getCellValue(double row, double col) = 0;
      virtual std::string getCellString(double row, double col) = 0;
      virtual std::string getCellRawValue(double row, double col) = 0;
      virtual CellType getCellType(double row, double col) = 0;
      virtual std::shared_ptr<HybridXlsxCellFormatSpec> getCellFormat(double row, double col) = 0;
      virtual bool hasFormula(double row, double col) = 0;
      virtual std::string formula(double row, double col) = 0;
      virtual bool findCell(double row, double col) = 0;
      virtual double getRowCount() = 0;
      virtual double getColumnCount() = 0;
      virtual double getLastRow() = 0;
      virtual double getLastColumn() = 0;
      virtual std::string getName() = 0;
      virtual std::vector<std::shared_ptr<AnyMap>> toJSON(const std::optional<std::vector<std::string>>& keys) = 0;
      virtual void fromJSON(const std::vector<std::shared_ptr<AnyMap>>& data) = 0;

    protected:
      void loadHybridMethods() override {}

    protected:
      static constexpr auto TAG = "XlsxWorksheet";
  };

} // namespace margelo::nitro::xlsx
