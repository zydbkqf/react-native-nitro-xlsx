#pragma once

// Nitro-free implementation of the XlsxCellFormat style state and its
// materialization into a document's styles.xml. HybridXlsxCellFormat wraps this
// class; unit tests exercise it directly (see cpp/tests/format_testable.hpp).
#include <OpenXLSX/XLStyles.hpp>
#include <OpenXLSX/XLCell.hpp>

#include <cctype>
#include <cstdint>
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>

namespace margelo::nitro::xlsx {

// Excel stores colors as ARGB hex strings; the alpha channel is always opaque.
inline std::string doubleToHexColor(double color) {
  uint32_t c = static_cast<uint32_t>(color);
  uint32_t argb = c | 0xFF000000;
  std::stringstream ss;
  ss << std::hex << std::setfill('0') << std::setw(8) << argb;
  std::string result = ss.str();
  for (char& ch : result) {
    ch = std::toupper(ch);
  }
  return result;
}

class XlsxCellStyle {
public:
  XlsxCellStyle() = default;
  ~XlsxCellStyle() = default;

  // Setters (each invalidates the cached style index)
  void setFontName(const std::string& name) { _fontName = name; invalidate(); }
  void setFontSize(double size) { _fontSize = size; invalidate(); }
  void setFontColor(double color) { _fontColor = color; invalidate(); }
  void setFontScript(double style) { _fontScript = style; invalidate(); }
  void setBold() { _bold = true; invalidate(); }
  void setItalic() { _italic = true; invalidate(); }
  void setUnderline(std::optional<double> style) { _underline = style.value_or(1); invalidate(); }
  void setStrikeout() { _strikeout = true; invalidate(); }
  void setAlign(double align) { _align = align; invalidate(); }
  void setVerticalAlign(double align) { _verticalAlign = align; invalidate(); }
  void setTextWrap() { _textWrap = true; invalidate(); }
  void setRotation(double angle) { _rotation = angle; invalidate(); }
  void setIndent(double level) { _indent = level; invalidate(); }
  void setShrink() { _shrink = true; invalidate(); }
  void setNumFormat(const std::string& format) { _numFormat = format; invalidate(); }
  void setNumFormatIndex(double index) { _numFormatIndex = index; invalidate(); }
  void setBgColor(double color) { _bgColor = color; invalidate(); }
  void setFgColor(double color) { _fgColor = color; invalidate(); }
  void setPattern(double pattern) { _pattern = pattern; invalidate(); }
  void setBorder(double style) { _border = style; invalidate(); }
  void setBorderColor(double color) { _borderColor = color; invalidate(); }
  void setBottom(double style) { _bottom = style; invalidate(); }
  void setTop(double style) { _top = style; invalidate(); }
  void setLeft(double style) { _left = style; invalidate(); }
  void setRight(double style) { _right = style; invalidate(); }
  void setBottomColor(double color) { _bottomColor = color; invalidate(); }
  void setTopColor(double color) { _topColor = color; invalidate(); }
  void setLeftColor(double color) { _leftColor = color; invalidate(); }
  void setRightColor(double color) { _rightColor = color; invalidate(); }
  void setDiagonal(double style) { _diagonal = style; invalidate(); }
  void setDiagonalColor(double color) { _diagonalColor = color; invalidate(); }
  void setLocked() { _locked = true; invalidate(); }
  void setUnlocked() { _locked = false; invalidate(); }
  void setHyperlink() { _hyperlink = true; invalidate(); }
  void setFontOnly() { _fontOnly = true; invalidate(); }

  // Getters (unstored colors read back as 0, like the nitro spec)
  std::string getFontName() const { return _fontName; }
  double getFontSize() const { return _fontSize; }
  double getFontColor() const { return _fontColor.value_or(0); }
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
  double getNumFormatIndex() const { return _numFormatIndex.value_or(0); }
  double getBgColor() const { return _bgColor.value_or(0); }
  double getFgColor() const { return _fgColor.value_or(0); }
  double getPattern() const { return _pattern; }
  double getBorder() const { return _border; }
  double getBorderColor() const { return _borderColor.value_or(0); }
  double getBottom() const { return _bottom; }
  double getTop() const { return _top; }
  double getLeft() const { return _left; }
  double getRight() const { return _right; }
  double getBottomColor() const { return _bottomColor.value_or(0); }
  double getTopColor() const { return _topColor.value_or(0); }
  double getLeftColor() const { return _leftColor.value_or(0); }
  double getRightColor() const { return _rightColor.value_or(0); }
  double getDiagonal() const { return _diagonal; }
  double getDiagonalColor() const { return _diagonalColor.value_or(0); }
  bool getIsLocked() const { return _locked.value_or(false); }
  bool getIsHyperlink() const { return _hyperlink; }
  bool getIsFontOnly() const { return _fontOnly; }

  // Materialize the held style state into the document's styles.xml.
  // The resulting cell format index is cached for one document and invalidated
  // whenever a setter runs.
  //
  // The cache is keyed on (styles address, documentGeneration) rather than on
  // the address alone: a destroyed XLDocument's XLStyles is routinely
  // reallocated at the same address for the next document, and a pointer-only
  // check would then hand out a cell format index that does not exist in the
  // new document ("s=" pointing past the end of cellXfs).
  OpenXLSX::XLStyleIndex applyToDocument(OpenXLSX::XLStyles& styles, uint64_t documentGeneration = 0) {
    if (_styleIndex.has_value() && _cachedStyles == &styles && _cachedGeneration == documentGeneration) {
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
    if (!_fontName.empty() || _fontSize > 0 || _fontColor.has_value() || _bold || _italic
        || _underline > 0 || _strikeout || _fontScript > 0) {
      OpenXLSX::XLFont emptyFont;
      OpenXLSX::XLStyleIndex fontIndex = styles.fonts().create(emptyFont);
      OpenXLSX::XLFont font = styles.fonts()[fontIndex];
      if (!_fontName.empty()) font.setFontName(_fontName);
      if (_fontSize > 0) font.setFontSize(static_cast<size_t>(_fontSize));
      if (_fontColor.has_value()) font.setFontColor(OpenXLSX::XLColor(doubleToHexColor(*_fontColor)));
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

    // Number format: a custom format string wins over a built-in format index.
    if (!_numFormat.empty()) {
      // Reuse an existing entry with the same format code instead of adding a
      // duplicate id for it.
      uint32_t numFmtId = 0;
      bool found = false;
      for (size_t i = 0; i < styles.numberFormats().count(); ++i) {
        OpenXLSX::XLNumberFormat existing = styles.numberFormats()[i];
        if (existing.formatCode() == _numFormat) {
          numFmtId = existing.numberFormatId();
          found = true;
          break;
        }
      }
      if (!found) {
        // Custom numFmtIds must be unique within this document; built-in
        // formats occupy ids 0-163, so scan for a free id >= 164 instead of
        // sharing a static counter across documents.
        uint32_t nextId = 164;
        for (size_t i = 0; i < styles.numberFormats().count(); ++i) {
          uint32_t id = styles.numberFormats()[i].numberFormatId();
          if (id >= nextId) nextId = id + 1;
        }
        OpenXLSX::XLNumberFormat emptyNumFormat;
        OpenXLSX::XLStyleIndex numFmtIndex = styles.numberFormats().create(emptyNumFormat);
        OpenXLSX::XLNumberFormat numFormat = styles.numberFormats()[numFmtIndex];
        numFormat.setFormatCode(_numFormat);
        numFormat.setNumberFormatId(nextId);
        numFmtId = nextId;
      }
      cellFormat.setNumberFormatId(numFmtId);
      cellFormat.setApplyNumberFormat(true);
    } else if (_numFormatIndex.has_value()) {
      cellFormat.setNumberFormatId(static_cast<uint32_t>(*_numFormatIndex));
      cellFormat.setApplyNumberFormat(true);
    }

    // Fill
    if (_bgColor.has_value() || _fgColor.has_value() || _pattern > 0) {
      OpenXLSX::XLFill emptyFill;
      OpenXLSX::XLStyleIndex fillIndex = styles.fills().create(emptyFill);
      OpenXLSX::XLFill fill = styles.fills()[fillIndex];
      OpenXLSX::XLPatternType patternType = _pattern > 0
        ? static_cast<OpenXLSX::XLPatternType>(_pattern)
        : OpenXLSX::XLPatternType::XLPatternSolid;
      fill.setPatternType(patternType);
      if (_bgColor.has_value()) fill.setBackgroundColor(OpenXLSX::XLColor(doubleToHexColor(*_bgColor)));
      if (_fgColor.has_value()) fill.setColor(OpenXLSX::XLColor(doubleToHexColor(*_fgColor)));
      // A solid pattern is painted with fgColor; bgColor is only the backdrop
      // for pattern fills. Mirroring a background-only colour into fgColor is
      // what makes setBgColor() actually show up in Excel.
      if (patternType == OpenXLSX::XLPatternType::XLPatternSolid
          && _bgColor.has_value() && !_fgColor.has_value()) {
        fill.setColor(OpenXLSX::XLColor(doubleToHexColor(*_bgColor)));
      }
      cellFormat.setFillIndex(fillIndex);
      cellFormat.setApplyFill(true);
    }

    // Border
    bool anyBorder = _border > 0 || _bottom > 0 || _top > 0 || _left > 0 || _right > 0 || _diagonal > 0
        || _borderColor.has_value() || _bottomColor.has_value() || _topColor.has_value()
        || _leftColor.has_value() || _rightColor.has_value() || _diagonalColor.has_value();
    if (anyBorder) {
      OpenXLSX::XLBorder emptyBorder;
      OpenXLSX::XLStyleIndex borderIndex = styles.borders().create(emptyBorder);
      OpenXLSX::XLBorder border = styles.borders()[borderIndex];

      // A side is written when its style > 0 or its color is set (a side with
      // only a color defaults to a THIN line). Per-side values override the
      // all-sides _border/_borderColor pair.
      using BorderSetter = bool (OpenXLSX::XLBorder::*)(OpenXLSX::XLLineStyle, OpenXLSX::XLColor, double);
      auto writeSide = [&](double sideStyle, const std::optional<double>& sideColor, BorderSetter setter) {
        double style = sideStyle > 0 ? sideStyle : _border;
        std::optional<double> color = sideColor.has_value() ? sideColor : _borderColor;
        if (style <= 0 && !color.has_value()) return;
        OpenXLSX::XLLineStyle lineStyle = style > 0
            ? static_cast<OpenXLSX::XLLineStyle>(static_cast<uint8_t>(style))
            : OpenXLSX::XLLineStyle::XLLineStyleThin;
        OpenXLSX::XLColor lineColor = color.has_value()
            ? OpenXLSX::XLColor(doubleToHexColor(*color))
            : OpenXLSX::XLColor();
        (border.*setter)(lineStyle, lineColor, 0.0);
      };
      writeSide(_left,   _leftColor,   &OpenXLSX::XLBorder::setLeft);
      writeSide(_right,  _rightColor,  &OpenXLSX::XLBorder::setRight);
      writeSide(_top,    _topColor,    &OpenXLSX::XLBorder::setTop);
      writeSide(_bottom, _bottomColor, &OpenXLSX::XLBorder::setBottom);
      if (_diagonal > 0 || _diagonalColor.has_value()) {
        writeSide(_diagonal, _diagonalColor, &OpenXLSX::XLBorder::setDiagonal);
        // The line style is only rendered when a diagonal direction is set;
        // the API has no direction parameter, so draw both diagonals.
        border.setDiagonalUp(true);
        border.setDiagonalDown(true);
      }
      cellFormat.setBorderIndex(borderIndex);
      cellFormat.setApplyBorder(true);
    }

    // Protection: Excel cells are locked by default, so an explicit
    // setUnlocked() must write locked=false + applyProtection.
    if (_locked.has_value()) {
      cellFormat.setLocked(*_locked);
      cellFormat.setApplyProtection(true);
    }

    _styleIndex = cellFormatIndex;
    _cachedStyles = &styles;
    _cachedGeneration = documentGeneration;
    return cellFormatIndex;
  }

  void applyToCell(OpenXLSX::XLCell& cell, OpenXLSX::XLStyleIndex styleIndex) {
    cell.setCellFormat(styleIndex);
  }

  // Reset to defaults
  void reset() {
    _fontName.clear();
    _fontSize = 0;
    _fontColor.reset();
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
    _numFormatIndex.reset();
    _bgColor.reset();
    _fgColor.reset();
    _pattern = 0;
    _border = 0;
    _borderColor.reset();
    _bottom = 0;
    _top = 0;
    _left = 0;
    _right = 0;
    _bottomColor.reset();
    _topColor.reset();
    _leftColor.reset();
    _rightColor.reset();
    _diagonal = 0;
    _diagonalColor.reset();
    _locked.reset();
    _hyperlink = false;
    _fontOnly = false;
    invalidate();
    _cachedStyles = nullptr;
    _cachedGeneration = 0;
  }

private:
  void invalidate() { _styleIndex.reset(); }

  std::optional<OpenXLSX::XLStyleIndex> _styleIndex;
  const OpenXLSX::XLStyles* _cachedStyles = nullptr;
  uint64_t _cachedGeneration = 0;

  std::string _fontName;
  double _fontSize = 0;
  std::optional<double> _fontColor;
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
  std::optional<double> _numFormatIndex;
  std::optional<double> _bgColor;
  std::optional<double> _fgColor;
  double _pattern = 0;
  double _border = 0;
  std::optional<double> _borderColor;
  double _bottom = 0;
  double _top = 0;
  double _left = 0;
  double _right = 0;
  std::optional<double> _bottomColor;
  std::optional<double> _topColor;
  std::optional<double> _leftColor;
  std::optional<double> _rightColor;
  double _diagonal = 0;
  std::optional<double> _diagonalColor;
  std::optional<bool> _locked;
  bool _hyperlink = false;
  bool _fontOnly = false;
};

} // namespace margelo::nitro::xlsx
