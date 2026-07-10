#include "HybridXlsxFormat.hpp"

namespace margelo::nitro::xlsx {

HybridXlsxFormat::HybridXlsxFormat()
    : HybridObject("XlsxFormat"), HybridXlsxFormatSpec() {
}

HybridXlsxFormat::~HybridXlsxFormat() {
}

void HybridXlsxFormat::applyToCell(OpenXLSX::XLCell& cell) {
  OpenXLSX::XLStyle style = cell.style();
  
  if (!_fontName.empty()) {
    style.font().name(_fontName);
  }
  if (_fontSize > 0) {
    style.font().size(_fontSize);
  }
  if (_fontColor > 0) {
    style.font().color(OpenXLSX::XLColor(_fontColor));
  }
  if (_bold) {
    style.font().isBold(true);
  }
  if (_italic) {
    style.font().isItalic(true);
  }
  if (_underline > 0) {
    style.font().isUnderline(true);
  }
  if (_strikeout) {
    style.font().isStrikethrough(true);
  }
  
  if (_textWrap) {
    style.alignment().wrapText(true);
  }
  if (_rotation != 0) {
    style.alignment().rotation(static_cast<int>(_rotation));
  }
  
  if (!_numFormat.empty()) {
    style.numberFormat().formatCode(_numFormat);
  }
  
  if (_bgColor > 0) {
    style.fill().patternType(OpenXLSX::XLPatternType::Solid);
    style.fill().patternColor(OpenXLSX::XLColor(_bgColor));
  }
  
  cell.style() = style;
}

void HybridXlsxFormat::setFontName(const std::string& name) {
  _fontName = name;
}

void HybridXlsxFormat::setFontSize(double size) {
  _fontSize = size;
}

void HybridXlsxFormat::setFontColor(double color) {
  _fontColor = color;
}

void HybridXlsxFormat::setFontScript(double style) {
  _fontScript = style;
}

void HybridXlsxFormat::setBold() {
  _bold = true;
}

void HybridXlsxFormat::setItalic() {
  _italic = true;
}

void HybridXlsxFormat::setUnderline(std::optional<double> style) {
  _underline = style.has_value() ? *style : 1;
}

void HybridXlsxFormat::setStrikeout() {
  _strikeout = true;
}

void HybridXlsxFormat::setAlign(double align) {
  _align = align;
}

void HybridXlsxFormat::setVerticalAlign(double align) {
  _verticalAlign = align;
}

void HybridXlsxFormat::setTextWrap() {
  _textWrap = true;
}

void HybridXlsxFormat::setRotation(double angle) {
  _rotation = angle;
}

void HybridXlsxFormat::setIndent(double level) {
  _indent = level;
}

void HybridXlsxFormat::setShrink() {
  _shrink = true;
}

void HybridXlsxFormat::setNumFormat(const std::string& format) {
  _numFormat = format;
}

void HybridXlsxFormat::setNumFormatIndex(double index) {
  _numFormatIndex = index;
}

void HybridXlsxFormat::setBgColor(double color) {
  _bgColor = color;
}

void HybridXlsxFormat::setFgColor(double color) {
  _fgColor = color;
}

void HybridXlsxFormat::setPattern(double pattern) {
  _pattern = pattern;
}

void HybridXlsxFormat::setBorder(double style) {
  _border = style;
}

void HybridXlsxFormat::setBorderColor(double color) {
  _borderColor = color;
}

void HybridXlsxFormat::setBottom(double style) {
  _bottom = style;
}

void HybridXlsxFormat::setTop(double style) {
  _top = style;
}

void HybridXlsxFormat::setLeft(double style) {
  _left = style;
}

void HybridXlsxFormat::setRight(double style) {
  _right = style;
}

void HybridXlsxFormat::setBottomColor(double color) {
  _bottomColor = color;
}

void HybridXlsxFormat::setTopColor(double color) {
  _topColor = color;
}

void HybridXlsxFormat::setLeftColor(double color) {
  _leftColor = color;
}

void HybridXlsxFormat::setRightColor(double color) {
  _rightColor = color;
}

void HybridXlsxFormat::setDiagonal(double style) {
  _diagonal = style;
}

void HybridXlsxFormat::setDiagonalColor(double color) {
  _diagonalColor = color;
}

void HybridXlsxFormat::setLocked() {
  _locked = true;
}

void HybridXlsxFormat::setUnlocked() {
  _locked = false;
}

void HybridXlsxFormat::setHyperlink() {
  _hyperlink = true;
}

void HybridXlsxFormat::setFontOnly() {
}

}