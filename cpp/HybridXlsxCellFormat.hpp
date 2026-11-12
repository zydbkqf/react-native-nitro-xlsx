#pragma once

#include "HybridXlsxCellFormatSpec.hpp"
#include "XlsxCellStyle.hpp"
#include <OpenXLSX/XLDocument.hpp>
#include <OpenXLSX/XLSheet.hpp>
#include <OpenXLSX/XLStyles.hpp>
#include <string>
#include <memory>

namespace margelo::nitro::xlsx {

class HybridXlsxCellFormat : public HybridXlsxCellFormatSpec {
public:
  HybridXlsxCellFormat();
  ~HybridXlsxCellFormat() override;

  // `documentGeneration` identifies the XLDocument that owns `styles` (see
  // HybridXlsxWorkbook::documentGeneration()). It must be passed on every call
  // so the style cache can never be reused by a different document that happens
  // to reuse the same heap address.
  OpenXLSX::XLStyleIndex applyToDocument(OpenXLSX::XLStyles& styles, uint64_t documentGeneration);
  void applyToCell(OpenXLSX::XLCell& cell, OpenXLSX::XLStyleIndex styleIndex);

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

  std::string getFontName() override;
  double getFontSize() override;
  double getFontColor() override;
  bool getIsBold() override;
  bool getIsItalic() override;
  std::string getNumFormat() override;
  double getBgColor() override;

private:
  // Style state and styles.xml materialization, shared with the unit tests.
  XlsxCellStyle _fmt;
};

}
