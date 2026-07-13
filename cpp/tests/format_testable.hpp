#pragma once

// Standalone header for gtest that doesn't depend on Nitro Modules
// This extracts the testable logic from HybridXlsxFormat
#include <string>
#include <memory>
#include <optional>

// Forward declarations to avoid pulling in OpenXLSX headers in this stub
namespace OpenXLSX {
  class XLCell;
  class XLStyle;
}

namespace margelo::nitro::xlsx::test {

// Standalone Format class for unit testing
// This is a simplified version that doesn't depend on HybridXlsxFormatSpec
class TestableFormat {
public:
  TestableFormat() = default;
  ~TestableFormat() = default;

  // Setters
  void setFontName(const std::string& name) { _fontName = name; }
  void setFontSize(double size) { _fontSize = size; }
  void setFontColor(double color) { _fontColor = color; }
  void setFontScript(double style) { _fontScript = style; }
  void setBold() { _bold = true; }
  void setItalic() { _italic = true; }
  void setUnderline(std::optional<double> style) { _underline = style.value_or(1); }
  void setStrikeout() { _strikeout = true; }
  void setAlign(double align) { _align = align; }
  void setVerticalAlign(double align) { _verticalAlign = align; }
  void setTextWrap() { _textWrap = true; }
  void setRotation(double angle) { _rotation = angle; }
  void setIndent(double level) { _indent = level; }
  void setShrink() { _shrink = true; }
  void setNumFormat(const std::string& format) { _numFormat = format; }
  void setNumFormatIndex(double index) { _numFormatIndex = index; }
  void setBgColor(double color) { _bgColor = color; }
  void setFgColor(double color) { _fgColor = color; }
  void setPattern(double pattern) { _pattern = pattern; }
  void setBorder(double style) { _border = style; }
  void setBorderColor(double color) { _borderColor = color; }
  void setBottom(double style) { _bottom = style; }
  void setTop(double style) { _top = style; }
  void setLeft(double style) { _left = style; }
  void setRight(double style) { _right = style; }
  void setBottomColor(double color) { _bottomColor = color; }
  void setTopColor(double color) { _topColor = color; }
  void setLeftColor(double color) { _leftColor = color; }
  void setRightColor(double color) { _rightColor = color; }
  void setDiagonal(double style) { _diagonal = style; }
  void setDiagonalColor(double color) { _diagonalColor = color; }
  void setLocked() { _locked = true; }
  void setUnlocked() { _locked = false; }
  void setHyperlink() { _hyperlink = true; }
  void setFontOnly() { _fontOnly = true; }

  // Getters
  std::string getFontName() const { return _fontName; }
  double getFontSize() const { return _fontSize; }
  double getFontColor() const { return _fontColor; }
  double getFontScript() const { return _fontScript; }
  bool getIsBold() const { return _bold; }
  bool getIsItalic() const { return _italic; }
  double getUnderline() const { return _underline; }
  bool getIsStrikeout() const { return _strikeout; }
  double getAlign() const { return _align; }
  double getVerticalAlign() const { return _verticalAlign; }
  bool getIsTextWrap() const { return _textWrap; }
  double getRotation() const { return _rotation; }
  double getIndent() const { return _indent; }
  bool getIsShrink() const { return _shrink; }
  std::string getNumFormat() const { return _numFormat; }
  double getNumFormatIndex() const { return _numFormatIndex; }
  double getBgColor() const { return _bgColor; }
  double getFgColor() const { return _fgColor; }
  double getPattern() const { return _pattern; }
  double getBorder() const { return _border; }
  double getBorderColor() const { return _borderColor; }
  double getBottom() const { return _bottom; }
  double getTop() const { return _top; }
  double getLeft() const { return _left; }
  double getRight() const { return _right; }
  double getBottomColor() const { return _bottomColor; }
  double getTopColor() const { return _topColor; }
  double getLeftColor() const { return _leftColor; }
  double getRightColor() const { return _rightColor; }
  double getDiagonal() const { return _diagonal; }
  double getDiagonalColor() const { return _diagonalColor; }
  bool getIsLocked() const { return _locked; }
  bool getIsHyperlink() const { return _hyperlink; }
  bool getIsFontOnly() const { return _fontOnly; }

  // Reset to defaults
  void reset() {
    _fontName.clear();
    _fontSize = 0;
    _fontColor = 0;
    _fontScript = 0;
    _bold = false;
    _italic = false;
    _underline = 0;
    _strikeout = false;
    _align = 0;
    _verticalAlign = 0;
    _textWrap = false;
    _rotation = 0;
    _indent = 0;
    _shrink = false;
    _numFormat.clear();
    _numFormatIndex = 0;
    _bgColor = 0;
    _fgColor = 0;
    _pattern = 0;
    _border = 0;
    _borderColor = 0;
    _bottom = 0;
    _top = 0;
    _left = 0;
    _right = 0;
    _bottomColor = 0;
    _topColor = 0;
    _leftColor = 0;
    _rightColor = 0;
    _diagonal = 0;
    _diagonalColor = 0;
    _locked = false;
    _hyperlink = false;
    _fontOnly = false;
  }

private:
  std::string _fontName;
  double _fontSize = 0;
  double _fontColor = 0;
  double _fontScript = 0;
  bool _bold = false;
  bool _italic = false;
  double _underline = 0;
  bool _strikeout = false;
  double _align = 0;
  double _verticalAlign = 0;
  bool _textWrap = false;
  double _rotation = 0;
  double _indent = 0;
  bool _shrink = false;
  std::string _numFormat;
  double _numFormatIndex = 0;
  double _bgColor = 0;
  double _fgColor = 0;
  double _pattern = 0;
  double _border = 0;
  double _borderColor = 0;
  double _bottom = 0;
  double _top = 0;
  double _left = 0;
  double _right = 0;
  double _bottomColor = 0;
  double _topColor = 0;
  double _leftColor = 0;
  double _rightColor = 0;
  double _diagonal = 0;
  double _diagonalColor = 0;
  bool _locked = false;
  bool _hyperlink = false;
  bool _fontOnly = false;
};

} // namespace margelo::nitro::xlsx::test