#include "HybridXlsxCellFormat.hpp"
#include <sstream>
#include <iomanip>
#include <cctype>

namespace margelo::nitro::xlsx {

static std::string doubleToHexColor(double color) {
  uint32_t c = static_cast<uint32_t>(color);
  // Excel uses ARGB format, prepend alpha channel (FF)
  uint32_t argb = c | 0xFF000000;
  std::stringstream ss;
  ss << std::hex << std::setfill('0') << std::setw(8) << argb;
  std::string result = ss.str();
  // Convert to uppercase (OpenXLSX expects uppercase hex)
  for (char& ch : result) {
    ch = std::toupper(ch);
  }
  return result;
}

HybridXlsxCellFormat::HybridXlsxCellFormat()
    : HybridObject("XlsxCellFormat"), HybridXlsxCellFormatSpec() {
}

HybridXlsxCellFormat::~HybridXlsxCellFormat() {
}

OpenXLSX::XLStyleIndex HybridXlsxCellFormat::applyToDocument(OpenXLSX::XLStyles& styles) {
  if (_styleIndex.has_value()) {
    return *_styleIndex;
  }

  // Create cell format in document first to get a valid XML node.
  // OpenXLSX default-constructed style objects have empty XML nodes;
  // setters on empty nodes silently fail. We must create the node in
  // the document first, then modify the live node via operator[].
  OpenXLSX::XLCellFormat emptyCellFormat;
  OpenXLSX::XLStyleIndex cellFormatIndex = styles.cellFormats().create(emptyCellFormat);
  OpenXLSX::XLCellFormat cellFormat = styles.cellFormats()[cellFormatIndex];

  // Font
  if (!_fontName.empty() || _fontSize > 0 || _fontColor > 0 || _bold || _italic || _underline > 0 || _strikeout || _fontScript > 0) {
    OpenXLSX::XLFont emptyFont;
    OpenXLSX::XLStyleIndex fontIndex = styles.fonts().create(emptyFont);
    OpenXLSX::XLFont font = styles.fonts()[fontIndex];
    if (!_fontName.empty()) font.setFontName(_fontName);
    if (_fontSize > 0) font.setFontSize(static_cast<size_t>(_fontSize));
    if (_fontColor > 0) font.setFontColor(OpenXLSX::XLColor(doubleToHexColor(_fontColor)));
    if (_bold) font.setBold();
    if (_italic) font.setItalic();
    if (_underline > 0) font.setUnderline(static_cast<OpenXLSX::XLUnderlineStyle>(_underline));
    if (_strikeout) font.setStrikethrough();
    if (_fontScript > 0) font.setVertAlign(static_cast<OpenXLSX::XLVerticalAlignRunStyle>(_fontScript));
    cellFormat.setFontIndex(fontIndex);
    cellFormat.setApplyFont(true);
  }

  // Alignment
  if (_textWrap || _rotation != 0 || _indent > 0 || _align > 0 || _verticalAlign > 0 || _shrink) {
    if (_align > 0) cellFormat.alignment(true).setHorizontal(static_cast<OpenXLSX::XLAlignmentStyle>(_align));
    if (_verticalAlign > 0) cellFormat.alignment(true).setVertical(static_cast<OpenXLSX::XLAlignmentStyle>(_verticalAlign));
    if (_textWrap) cellFormat.alignment(true).setWrapText();
    if (_rotation != 0) cellFormat.alignment(true).setTextRotation(static_cast<uint16_t>(_rotation));
    if (_indent > 0) cellFormat.alignment(true).setIndent(static_cast<uint32_t>(_indent));
    if (_shrink) cellFormat.alignment(true).setShrinkToFit();
    cellFormat.setApplyAlignment(true);
  }

  // Number format
  if (!_numFormat.empty()) {
    OpenXLSX::XLNumberFormat emptyNumFormat;
    OpenXLSX::XLStyleIndex numFmtIndex = styles.numberFormats().create(emptyNumFormat);
    OpenXLSX::XLNumberFormat numFormat = styles.numberFormats()[numFmtIndex];
    numFormat.setFormatCode(_numFormat);
    // Assign a unique custom numFmtId (>= 164 for user-defined formats)
    static uint32_t nextCustomNumFmtId = 164;
    numFormat.setNumberFormatId(nextCustomNumFmtId++);
    cellFormat.setNumberFormatId(numFormat.numberFormatId());
    cellFormat.setApplyNumberFormat(true);
  }

  // Fill
  if (_bgColor > 0 || _fgColor > 0 || _pattern > 0) {
    OpenXLSX::XLFill emptyFill;
    OpenXLSX::XLStyleIndex fillIndex = styles.fills().create(emptyFill);
    OpenXLSX::XLFill fill = styles.fills()[fillIndex];
    fill.setPatternType(_pattern > 0
      ? static_cast<OpenXLSX::XLPatternType>(_pattern)
      : OpenXLSX::XLPatternType::XLPatternSolid);
    if (_bgColor > 0) fill.setBackgroundColor(OpenXLSX::XLColor(doubleToHexColor(_bgColor)));
    if (_fgColor > 0) fill.setColor(OpenXLSX::XLColor(doubleToHexColor(_fgColor)));
    cellFormat.setFillIndex(fillIndex);
    cellFormat.setApplyFill(true);
  }

  // Border
  if (_border > 0 || _bottom > 0 || _top > 0 || _left > 0 || _right > 0 || _diagonal > 0) {
    OpenXLSX::XLBorder emptyBorder;
    OpenXLSX::XLStyleIndex borderIndex = styles.borders().create(emptyBorder);
    OpenXLSX::XLBorder border = styles.borders()[borderIndex];
    // TODO: Implement border styling with OpenXLSX v0.5.1 API
    cellFormat.setBorderIndex(borderIndex);
    cellFormat.setApplyBorder(true);
  }

  // Protection
  if (_locked) {
    cellFormat.setLocked(true);
    cellFormat.setApplyProtection(true);
  }

  _styleIndex = cellFormatIndex;
  return cellFormatIndex;
}

void HybridXlsxCellFormat::applyToCell(OpenXLSX::XLCell& cell, OpenXLSX::XLStyleIndex styleIndex) {
  cell.setCellFormat(styleIndex);
}

void HybridXlsxCellFormat::setFontName(const std::string& name) {
  _fontName = name;
}

void HybridXlsxCellFormat::setFontSize(double size) {
  _fontSize = size;
}

void HybridXlsxCellFormat::setFontColor(double color) {
  _fontColor = color;
}

void HybridXlsxCellFormat::setFontScript(double style) {
  _fontScript = style;
}

void HybridXlsxCellFormat::setBold() {
  _bold = true;
}

void HybridXlsxCellFormat::setItalic() {
  _italic = true;
}

void HybridXlsxCellFormat::setUnderline(std::optional<double> style) {
  _underline = style.has_value() ? *style : 1;
}

void HybridXlsxCellFormat::setStrikeout() {
  _strikeout = true;
}

void HybridXlsxCellFormat::setAlign(double align) {
  _align = align;
}

void HybridXlsxCellFormat::setVerticalAlign(double align) {
  _verticalAlign = align;
}

void HybridXlsxCellFormat::setTextWrap() {
  _textWrap = true;
}

void HybridXlsxCellFormat::setRotation(double angle) {
  _rotation = angle;
}

void HybridXlsxCellFormat::setIndent(double level) {
  _indent = level;
}

void HybridXlsxCellFormat::setShrink() {
  _shrink = true;
}

void HybridXlsxCellFormat::setNumFormat(const std::string& format) {
  _numFormat = format;
}

void HybridXlsxCellFormat::setNumFormatIndex(double index) {
  _numFormatIndex = index;
}

void HybridXlsxCellFormat::setBgColor(double color) {
  _bgColor = color;
}

void HybridXlsxCellFormat::setFgColor(double color) {
  _fgColor = color;
}

void HybridXlsxCellFormat::setPattern(double pattern) {
  _pattern = pattern;
}

void HybridXlsxCellFormat::setBorder(double style) {
  _border = style;
}

void HybridXlsxCellFormat::setBorderColor(double color) {
  _borderColor = color;
}

void HybridXlsxCellFormat::setBottom(double style) {
  _bottom = style;
}

void HybridXlsxCellFormat::setTop(double style) {
  _top = style;
}

void HybridXlsxCellFormat::setLeft(double style) {
  _left = style;
}

void HybridXlsxCellFormat::setRight(double style) {
  _right = style;
}

void HybridXlsxCellFormat::setBottomColor(double color) {
  _bottomColor = color;
}

void HybridXlsxCellFormat::setTopColor(double color) {
  _topColor = color;
}

void HybridXlsxCellFormat::setLeftColor(double color) {
  _leftColor = color;
}

void HybridXlsxCellFormat::setRightColor(double color) {
  _rightColor = color;
}

void HybridXlsxCellFormat::setDiagonal(double style) {
  _diagonal = style;
}

void HybridXlsxCellFormat::setDiagonalColor(double color) {
  _diagonalColor = color;
}

void HybridXlsxCellFormat::setLocked() {
  _locked = true;
}

void HybridXlsxCellFormat::setUnlocked() {
  _locked = false;
}

void HybridXlsxCellFormat::setHyperlink() {
  _hyperlink = true;
}

void HybridXlsxCellFormat::setFontOnly() {
}

std::string HybridXlsxCellFormat::getFontName() {
  return _fontName;
}

double HybridXlsxCellFormat::getFontSize() {
  return _fontSize;
}

double HybridXlsxCellFormat::getFontColor() {
  return _fontColor;
}

bool HybridXlsxCellFormat::getIsBold() {
  return _bold;
}

bool HybridXlsxCellFormat::getIsItalic() {
  return _italic;
}

std::string HybridXlsxCellFormat::getNumFormat() {
  return _numFormat;
}

double HybridXlsxCellFormat::getBgColor() {
  return _bgColor;
}

}