#pragma once

// Test-build stand-in for nitrogen's HybridXlsxCellFormatSpec.hpp.
// Mirrors the pure-virtual interface (signatures must stay in sync) without the
// Nitro/JSI hybrid-registration machinery.

#include <NitroModules/HybridObject.hpp>
#include <optional>
#include <string>

namespace margelo::nitro::xlsx {

  using namespace margelo::nitro;

  class HybridXlsxCellFormatSpec: public virtual HybridObject {
    public:
      explicit HybridXlsxCellFormatSpec(): HybridObject(TAG) { }
      ~HybridXlsxCellFormatSpec() override = default;

    public:
      virtual void setFontName(const std::string& name) = 0;
      virtual void setFontSize(double size) = 0;
      virtual void setFontColor(double color) = 0;
      virtual void setFontScript(double style) = 0;
      virtual void setBold() = 0;
      virtual void setItalic() = 0;
      virtual void setUnderline(std::optional<double> style) = 0;
      virtual void setStrikeout() = 0;
      virtual void setAlign(double align) = 0;
      virtual void setVerticalAlign(double align) = 0;
      virtual void setTextWrap() = 0;
      virtual void setRotation(double angle) = 0;
      virtual void setIndent(double level) = 0;
      virtual void setShrink() = 0;
      virtual void setNumFormat(const std::string& format) = 0;
      virtual void setNumFormatIndex(double index) = 0;
      virtual void setBgColor(double color) = 0;
      virtual void setFgColor(double color) = 0;
      virtual void setPattern(double pattern) = 0;
      virtual void setBorder(double style) = 0;
      virtual void setBorderColor(double color) = 0;
      virtual void setBottom(double style) = 0;
      virtual void setTop(double style) = 0;
      virtual void setLeft(double style) = 0;
      virtual void setRight(double style) = 0;
      virtual void setBottomColor(double color) = 0;
      virtual void setTopColor(double color) = 0;
      virtual void setLeftColor(double color) = 0;
      virtual void setRightColor(double color) = 0;
      virtual void setDiagonal(double style) = 0;
      virtual void setDiagonalColor(double color) = 0;
      virtual void setLocked() = 0;
      virtual void setUnlocked() = 0;
      virtual void setHyperlink() = 0;
      virtual void setFontOnly() = 0;
      virtual std::string getFontName() = 0;
      virtual double getFontSize() = 0;
      virtual double getFontColor() = 0;
      virtual bool getIsBold() = 0;
      virtual bool getIsItalic() = 0;
      virtual std::string getNumFormat() = 0;
      virtual double getBgColor() = 0;

    protected:
      void loadHybridMethods() override {}

    protected:
      static constexpr auto TAG = "XlsxCellFormat";
  };

} // namespace margelo::nitro::xlsx
