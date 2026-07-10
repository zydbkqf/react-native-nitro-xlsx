#pragma once

#include "HybridXlsxFormatSpec.hpp"
#include <OpenXLSX/XLDocument.hpp>
#include <OpenXLSX/XLStyles.hpp>
#include <string>
#include <memory>

namespace margelo::nitro::xlsx {

class HybridXlsxFormat : public HybridXlsxFormatSpec {
public:
  HybridXlsxFormat();
  ~HybridXlsxFormat() override;

  void applyToCell(OpenXLSX::XLCell& cell);

  void setFontName(const std::string& name) override;
  void setFontSize(double size) override;
  void setFontColor(double color) override;
  void setFontScript(double style) override;
  void setBold() override;
  void setItalic() override;
  void setUnderline(std::optional<double> style) override;
  void setStrikeout() override;
  
  void setAlign(double align) override;
  void setVerticalAlign(double align) override;
  void setTextWrap() override;
  void setRotation(double angle) override;
  void setIndent(double level) override;
  void setShrink() override;
  
  void setNumFormat(const std::string& format) override;
  void setNumFormatIndex(double index) override;
  
  void setBgColor(double color) override;
  void setFgColor(double color) override;
  void setPattern(double pattern) override;
  
  void setBorder(double style) override;
  void setBorderColor(double color) override;
  void setBottom(double style) override;
  void setTop(double style) override;
  void setLeft(double style) override;
  void setRight(double style) override;
  void setBottomColor(double color) override;
  void setTopColor(double color) override;
  void setLeftColor(double color) override;
  void setRightColor(double color) override;
  void setDiagonal(double style) override;
  void setDiagonalColor(double color) override;
  
  void setLocked() override;
  void setUnlocked() override;
  
  void setHyperlink() override;
  void setFontOnly() override;

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
};

}