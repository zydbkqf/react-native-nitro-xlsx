#include "HybridXlsxWorksheet.hpp"
#include "XlsxError.hpp"
#include "XlsxJson.hpp"
#include <OpenXLSX/XLCellReference.hpp>
#include <cctype>
#include <cstring>
#include <cmath>
#include <fstream>
#include <chrono>
#include <string>

namespace margelo::nitro::xlsx {

HybridXlsxWorksheet::HybridXlsxWorksheet(OpenXLSX::XLDocument& doc, OpenXLSX::XLWorksheet worksheet, uint64_t documentGeneration)
    : HybridObject("XlsxWorksheet"), HybridXlsxWorksheetSpec(), _doc(doc), _worksheet(worksheet), _documentGeneration(documentGeneration) {
}

HybridXlsxWorksheet::~HybridXlsxWorksheet() {
}

void HybridXlsxWorksheet::applyFormat(OpenXLSX::XLCell& cell, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  if (!format.has_value()) return;
  
  auto hybridFormat = std::dynamic_pointer_cast<HybridXlsxCellFormat>(*format);
  if (!hybridFormat) return;
  
  auto styleIndex = hybridFormat->applyToDocument(_doc.styles(), _documentGeneration);
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

void HybridXlsxWorksheet::writeArrayFormula(double /*firstRow*/, double /*firstCol*/, double /*lastRow*/, double /*lastCol*/, const std::string& /*formula*/, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& /*format*/) {
  // OpenXLSX (XLFormula) explicitly does not support array formulas; attempting to
  // write one would silently produce a normal formula covering only the top-left cell.
  throw XlsxError(xlsx_error::UNSUPPORTED,
                  "Array formulas are not supported by the underlying OpenXLSX engine");
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
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    for (unsigned int c = static_cast<unsigned int>(firstCol); c <= static_cast<unsigned int>(lastCol); ++c) {
      _worksheet.column(c).setWidth(static_cast<float>(width));
    }
    if (format.has_value()) {
      // Apply format to entire column range by setting it on cells in the first row
      // (OpenXLSX does not support column-level style; apply to first row cells as a hint)
      auto hybridFormat = std::dynamic_pointer_cast<HybridXlsxCellFormat>(*format);
      if (hybridFormat) {
        auto styleIndex = hybridFormat->applyToDocument(_doc.styles(), _documentGeneration);
        for (unsigned int c = static_cast<unsigned int>(firstCol); c <= static_cast<unsigned int>(lastCol); ++c) {
          OpenXLSX::XLCell cell = _worksheet.cell(1, c);
          hybridFormat->applyToCell(cell, styleIndex);
        }
      }
    }
  });
}

void HybridXlsxWorksheet::setRow(double row, double height, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    _worksheet.row(static_cast<unsigned int>(row)).setHeight(static_cast<float>(height));
    if (format.has_value()) {
      auto hybridFormat = std::dynamic_pointer_cast<HybridXlsxCellFormat>(*format);
      if (hybridFormat) {
        auto styleIndex = hybridFormat->applyToDocument(_doc.styles(), _documentGeneration);
        unsigned int colCount = _worksheet.columnCount();
        for (unsigned int c = 1; c <= colCount; ++c) {
          OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(row), c);
          hybridFormat->applyToCell(cell, styleIndex);
        }
      }
    }
  });
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

    // Shift cell contents left for every row. Use findCell for reads so rows/cells
    // that do not exist are never materialized just by shifting.
    for (unsigned int r = 1; r <= lastRow; ++r) {
      for (unsigned int c = targetCol + 1; c <= lastCol; ++c) {
        OpenXLSX::XLCellAssignable src = _worksheet.findCell(r, c);
        if (!src.empty()) {
          OpenXLSX::XLCell dst = _worksheet.cell(r, c - 1);
          dst.copyFrom(src);
          dst.setCellFormat(src.cellFormat());
        } else {
          // Only clear an existing destination cell; never create one.
          OpenXLSX::XLCellAssignable dst = _worksheet.findCell(r, c - 1);
          if (!dst.empty()) {
            dst.clear(0);
          }
        }
      }
      // Clear the trailing column (only if the cell exists)
      OpenXLSX::XLCellAssignable trailing = _worksheet.findCell(r, lastCol);
      if (!trailing.empty()) {
        trailing.clear(0);
      }
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
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCellReference topLeft(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol));
    OpenXLSX::XLCellReference bottomRight(static_cast<unsigned int>(lastRow), static_cast<unsigned int>(lastCol));
    OpenXLSX::XLCellRange range = _worksheet.range(topLeft, bottomRight);
    _worksheet.mergeCells(range);
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol));
    cell.value() = value;
    applyFormat(cell, format);
  });
}

void HybridXlsxWorksheet::mergeRangeNum(double firstRow, double firstCol, double lastRow, double lastCol, double value, const std::optional<std::shared_ptr<HybridXlsxCellFormatSpec>>& format) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCellReference topLeft(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol));
    OpenXLSX::XLCellReference bottomRight(static_cast<unsigned int>(lastRow), static_cast<unsigned int>(lastCol));
    OpenXLSX::XLCellRange range = _worksheet.range(topLeft, bottomRight);
    _worksheet.mergeCells(range);
    OpenXLSX::XLCell cell = _worksheet.cell(static_cast<unsigned int>(firstRow), static_cast<unsigned int>(firstCol));
    cell.value() = value;
    applyFormat(cell, format);
  });
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
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    _worksheet.setActive();
  });
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
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    for (unsigned int c = static_cast<unsigned int>(firstCol); c <= static_cast<unsigned int>(lastCol); ++c) {
      _worksheet.column(c).setHidden(hidden);
    }
  });
}

void HybridXlsxWorksheet::setRowHidden(double row, bool hidden) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    _worksheet.row(static_cast<unsigned int>(row)).setHidden(hidden);
  });
}

// ========== Read helpers ==========

namespace {

  // Reverse of HybridXlsxCellFormat's color encoding: XLColor hex "AARRGGBB" ->
  // packed RGB double (0xRRGGBB) used by the public color setters.
  double xlColorToDouble(const OpenXLSX::XLColor& color) {
    std::string hex = color.hex();
    if (hex.empty()) return 0.0;
    uint32_t argb = static_cast<uint32_t>(std::stoul(hex, nullptr, 16));
    return static_cast<double>(argb & 0x00FFFFFFu);
  }

  // Built-in number format codes (numFmtId 0-49, ECMA-376). nullptr when unknown.
  const char* builtinNumFmtCode(uint32_t numFmtId) {
    switch (numFmtId) {
      case 0:  return "General";
      case 1:  return "0";
      case 2:  return "0.00";
      case 3:  return "#,##0";
      case 4:  return "#,##0.00";
      case 5:  return "_($* #,##0_);_($* (#,##0);_($* \"-\"_);_(@_)";
      case 6:  return "_($* #,##0.00_);_($* (#,##0.00);_($* \"-\"??_);_(@_)";
      case 7:  return "_(#,##0_);(#,##0)";
      case 8:  return "_(#,##0.00_);(#,##0.00)";
      case 9:  return "0%";
      case 10: return "0.00%";
      case 11: return "0.00E+00";
      case 12: return "# ?/?";
      case 13: return "# ?\?/??";
      case 14: return "mm-dd-yy";
      case 15: return "d-mmm-yy";
      case 16: return "d-mmm";
      case 17: return "mmm-yy";
      case 18: return "h:mm AM/PM";
      case 19: return "h:mm:ss AM/PM";
      case 20: return "h:mm";
      case 21: return "h:mm:ss";
      case 22: return "m/d/yy h:mm";
      case 37: return "#,##0 ;(#,##0)";
      case 38: return "#,##0 ;[Red](#,##0)";
      case 39: return "#,##0.00;(#,##0.00)";
      case 40: return "#,##0.00;[Red](#,##0.00)";
      case 45: return "mm:ss";
      case 46: return "[h]:mm:ss";
      case 47: return "mmss.0";
      case 48: return "##0.0E+0";
      case 49: return "@";
      default: return nullptr;
    }
  }

  // Resolve a numFmtId to its format code: custom entries in styles.xml first,
  // then the built-in table.
  std::string lookupNumFmtCode(OpenXLSX::XLStyles& styles, uint32_t numFmtId) {
    try {
      return styles.numberFormats().numberFormatById(numFmtId).formatCode();
    } catch (const std::exception&) {
      const char* builtin = builtinNumFmtCode(numFmtId);
      return builtin ? std::string(builtin) : std::string();
    }
  }

  // Does this format code describe a date/time? Strips quoted literals, escapes
  // and [..] sections, then looks for date/time tokens (y, d, hh, ss, m/d).
  bool formatCodeIsDateTime(const std::string& fmt) {
    std::string s;
    s.reserve(fmt.size());
    bool inQuote = false;
    for (size_t i = 0; i < fmt.size(); ++i) {
      char ch = fmt[i];
      if (inQuote) {
        if (ch == '"') inQuote = false;
        continue;
      }
      if (ch == '"') { inQuote = true; continue; }
      if (ch == '\\' && i + 1 < fmt.size()) { ++i; continue; }
      if (ch == '[') {
        size_t end = fmt.find(']', i);
        if (end != std::string::npos) {
          std::string section = fmt.substr(i + 1, end - i - 1);
          // Elapsed-time sections like [h], [mm], [ss] are date/time.
          if (section == "h" || section == "m" || section == "s" ||
              section == "hh" || section == "mm" || section == "ss") {
            return true;
          }
          i = end;
          continue;
        }
      }
      s += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    if (s.find('y') != std::string::npos) return true;
    if (s.find('d') != std::string::npos) return true;
    // Single-letter h covers custom time-only codes such as "h:mm"; the
    // "hh"/"ss" forms are covered by this and the check below.
    if (s.find('h') != std::string::npos) return true;
    if (s.find("ss") != std::string::npos) return true;
    if (s.find("m/") != std::string::npos || s.find("/m") != std::string::npos) return true;
    return false;
  }

  // Date/time detection per the number format applied to a cell style:
  // built-in numFmtId 14-22 are date/time; custom ids >= 164 are inspected by code.
  bool numFmtIsDateTime(OpenXLSX::XLStyles& styles, OpenXLSX::XLStyleIndex styleIndex) {
    try {
      uint32_t numFmtId = styles.cellFormats()[styleIndex].numberFormatId();
      if (numFmtId >= 14 && numFmtId <= 22) return true;
      if (numFmtId >= 164) {
        try {
          return formatCodeIsDateTime(styles.numberFormats().numberFormatById(numFmtId).formatCode());
        } catch (const std::exception&) {
          return false;
        }
      }
      return false;
    } catch (const std::exception&) {
      return false;
    }
  }

} // anonymous namespace

std::variant<bool, nitro::NullType, std::string, double> HybridXlsxWorksheet::getCellValue(double row, double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&]() -> std::variant<bool, nitro::NullType, std::string, double> {
    OpenXLSX::XLCellAssignable cell = _worksheet.findCell(static_cast<uint32_t>(row), static_cast<uint16_t>(col));
    if (cell.empty()) return nitro::NullType{};
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
  });
}

std::string HybridXlsxWorksheet::getCellString(double row, double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCellAssignable cell = _worksheet.findCell(static_cast<uint32_t>(row), static_cast<uint16_t>(col));
    if (cell.empty()) return std::string();
    auto type = cell.value().type();

    switch (type) {
      case OpenXLSX::XLValueType::String:
        return cell.value().get<std::string>();
      case OpenXLSX::XLValueType::Integer:
        return std::to_string(cell.value().get<int64_t>());
      case OpenXLSX::XLValueType::Float:
        return std::to_string(cell.value().get<double>());
      case OpenXLSX::XLValueType::Boolean:
        return cell.value().get<bool>() ? std::string("TRUE") : std::string("FALSE");
      case OpenXLSX::XLValueType::Empty:
      case OpenXLSX::XLValueType::Error:
      default:
        return std::string();
    }
  });
}

std::string HybridXlsxWorksheet::getCellRawValue(double row, double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCellAssignable cell = _worksheet.findCell(static_cast<uint32_t>(row), static_cast<uint16_t>(col));
    if (cell.empty()) return std::string();

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
        return cell.value().get<bool>() ? std::string("TRUE") : std::string("FALSE");
      case OpenXLSX::XLValueType::Empty:
      case OpenXLSX::XLValueType::Error:
      default:
        return std::string();
    }
  });
}

CellType HybridXlsxWorksheet::getCellType(double row, double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    OpenXLSX::XLCellAssignable cell = _worksheet.findCell(static_cast<uint32_t>(row), static_cast<uint16_t>(col));
    if (cell.empty()) return CellType::EMPTY;

    if (cell.hasFormula()) {
      return CellType::FORMULA;
    }

    auto type = cell.value().type();
    switch (type) {
      case OpenXLSX::XLValueType::Empty:
        return CellType::BLANK;
      case OpenXLSX::XLValueType::String:
        return CellType::STRING;
      case OpenXLSX::XLValueType::Integer:
      case OpenXLSX::XLValueType::Float:
        if (numFmtIsDateTime(_doc.styles(), cell.cellFormat())) {
          return CellType::DATE;
        }
        return CellType::NUMBER;
      case OpenXLSX::XLValueType::Boolean:
        return CellType::BOOLEAN;
      case OpenXLSX::XLValueType::Error:
        return CellType::ERROR;
      default:
        return CellType::EMPTY;
    }
  });
}

std::shared_ptr<HybridXlsxCellFormatSpec> HybridXlsxWorksheet::getCellFormat(double row, double col) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&]() -> std::shared_ptr<HybridXlsxCellFormatSpec> {
    auto format = std::make_shared<HybridXlsxCellFormat>();
    OpenXLSX::XLCellAssignable cell = _worksheet.findCell(static_cast<uint32_t>(row), static_cast<uint16_t>(col));
    if (cell.empty()) return format;

    auto& styles = _doc.styles();
    OpenXLSX::XLCellFormat cellFormat = styles.cellFormats()[cell.cellFormat()];

    // Number format
    std::string numFmtCode = lookupNumFmtCode(styles, cellFormat.numberFormatId());
    if (!numFmtCode.empty() && numFmtCode != "General") {
      format->setNumFormat(numFmtCode);
    }

    // Font
    OpenXLSX::XLFont font = styles.fonts()[cellFormat.fontIndex()];
    if (!font.fontName().empty()) format->setFontName(font.fontName());
    if (font.fontSize() > 0) format->setFontSize(static_cast<double>(font.fontSize()));
    try {
      double fontColor = xlColorToDouble(font.fontColor());
      if (fontColor > 0) format->setFontColor(fontColor);
    } catch (const std::exception&) {}
    if (font.bold()) format->setBold();
    if (font.italic()) format->setItalic();
    if (font.strikethrough()) format->setStrikeout();
    {
      auto underline = font.underline();
      if (underline != OpenXLSX::XLUnderlineNone && underline != OpenXLSX::XLUnderlineInvalid) {
        format->setUnderline(static_cast<double>(underline));
      }
    }
    {
      auto vertAlign = font.vertAlign();
      if (vertAlign != OpenXLSX::XLBaseline && vertAlign != OpenXLSX::XLVerticalAlignRunInvalid) {
        format->setFontScript(static_cast<double>(vertAlign));
      }
    }

    // Alignment
    OpenXLSX::XLAlignment alignment = cellFormat.alignment(OpenXLSX::XLDoNotCreate);
    {
      auto horizontal = alignment.horizontal();
      if (horizontal != OpenXLSX::XLAlignGeneral && horizontal != OpenXLSX::XLAlignInvalid) {
        format->setAlign(static_cast<double>(horizontal));
      }
      auto vertical = alignment.vertical();
      if (vertical != OpenXLSX::XLAlignGeneral && vertical != OpenXLSX::XLAlignInvalid) {
        format->setVerticalAlign(static_cast<double>(vertical));
      }
      if (alignment.wrapText()) format->setTextWrap();
      if (alignment.textRotation() != 0) format->setRotation(static_cast<double>(alignment.textRotation()));
      if (alignment.indent() != 0) format->setIndent(static_cast<double>(alignment.indent()));
      if (alignment.shrinkToFit()) format->setShrink();
    }

    // Fill (pattern + fg/bg colors)
    OpenXLSX::XLFill fill = styles.fills()[cellFormat.fillIndex()];
    if (fill.fillType() == OpenXLSX::XLPatternFill) {
      auto pattern = fill.patternType();
      if (pattern != OpenXLSX::XLPatternNone && pattern != OpenXLSX::XLPatternTypeInvalid) {
        format->setPattern(static_cast<double>(pattern));
      }
      try {
        double fg = xlColorToDouble(fill.color());
        if (fg > 0) format->setFgColor(fg);
      } catch (const std::exception&) {}
      try {
        double bg = xlColorToDouble(fill.backgroundColor());
        if (bg > 0) format->setBgColor(bg);
      } catch (const std::exception&) {}
    }

    // Border (per-side styles and colors)
    OpenXLSX::XLBorder border = styles.borders()[cellFormat.borderIndex()];
    double leftStyle = 0, rightStyle = 0, topStyle = 0, bottomStyle = 0;
    double leftColor = 0, rightColor = 0, topColor = 0, bottomColor = 0;
    auto readLine = [](const OpenXLSX::XLLine& line, double& styleOut, double& colorOut) {
      if (!line) return;
      auto style = line.style();
      if (style != OpenXLSX::XLLineStyleNone && style != OpenXLSX::XLLineStyleInvalid) {
        styleOut = static_cast<double>(style);
      }
      try {
        double c = xlColorToDouble(line.color().rgb());
        if (c > 0) colorOut = c;
      } catch (const std::exception&) {}
    };
    readLine(border.left(), leftStyle, leftColor);
    readLine(border.right(), rightStyle, rightColor);
    readLine(border.top(), topStyle, topColor);
    readLine(border.bottom(), bottomStyle, bottomColor);
    if (leftStyle > 0) format->setLeft(leftStyle);
    if (rightStyle > 0) format->setRight(rightStyle);
    if (topStyle > 0) format->setTop(topStyle);
    if (bottomStyle > 0) format->setBottom(bottomStyle);
    if (leftColor > 0) format->setLeftColor(leftColor);
    if (rightColor > 0) format->setRightColor(rightColor);
    if (topColor > 0) format->setTopColor(topColor);
    if (bottomColor > 0) format->setBottomColor(bottomColor);
    if (leftStyle > 0 && leftStyle == rightStyle && leftStyle == topStyle && leftStyle == bottomStyle) {
      format->setBorder(leftStyle);
      if (leftColor > 0 && leftColor == rightColor && leftColor == topColor && leftColor == bottomColor) {
        format->setBorderColor(leftColor);
      }
    }

    // Protection
    if (cellFormat.applyProtection()) {
      if (cellFormat.locked()) format->setLocked();
      else format->setUnlocked();
    }

    return format;
  });
}

double HybridXlsxWorksheet::getRowCount() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(_worksheet.rowCount());
  });
}

double HybridXlsxWorksheet::getColumnCount() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(_worksheet.columnCount());
  });
}

double HybridXlsxWorksheet::getLastRow() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (_worksheet.rowCount() == 0) return 0.0;
    return static_cast<double>(_worksheet.lastCell().row());
  });
}

double HybridXlsxWorksheet::getLastColumn() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (_worksheet.rowCount() == 0) return 0.0;
    return static_cast<double>(_worksheet.lastCell().column());
  });
}

std::string HybridXlsxWorksheet::getName() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return _worksheet.name();
  });
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