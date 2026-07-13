#include <gtest/gtest.h>
#include "format_testable.hpp"

using namespace margelo::nitro::xlsx::test;

class HybridXlsxFormatTest : public ::testing::Test {
protected:
  TestableFormat format;

  void SetUp() override {
    format.reset();
  }
};

// Default Values Tests
TEST_F(HybridXlsxFormatTest, DefaultValues) {
  EXPECT_EQ(format.getFontName(), "");
  EXPECT_EQ(format.getFontSize(), 0);
  EXPECT_EQ(format.getFontColor(), 0);
  EXPECT_FALSE(format.getIsBold());
  EXPECT_FALSE(format.getIsItalic());
  EXPECT_EQ(format.getUnderline(), 0);
  EXPECT_FALSE(format.getIsStrikeout());
  EXPECT_FALSE(format.getIsTextWrap());
  EXPECT_EQ(format.getBgColor(), 0);
  EXPECT_FALSE(format.getIsLocked());
}

// Font Tests
TEST_F(HybridXlsxFormatTest, SetFontName) {
  format.setFontName("Arial");
  EXPECT_EQ(format.getFontName(), "Arial");
}

TEST_F(HybridXlsxFormatTest, SetFontNameMultipleTimes) {
  format.setFontName("Arial");
  format.setFontName("Times New Roman");
  format.setFontName("Calibri");
  EXPECT_EQ(format.getFontName(), "Calibri");
}

TEST_F(HybridXlsxFormatTest, SetFontSize) {
  format.setFontSize(12.0);
  EXPECT_EQ(format.getFontSize(), 12.0);

  format.setFontSize(24.5);
  EXPECT_EQ(format.getFontSize(), 24.5);
}

TEST_F(HybridXlsxFormatTest, SetFontColor) {
  format.setFontColor(0xFF0000);
  EXPECT_EQ(format.getFontColor(), 0xFF0000);
}

TEST_F(HybridXlsxFormatTest, SetFontScript) {
  format.setFontScript(1);
  EXPECT_EQ(format.getFontScript(), 1);

  format.setFontScript(2);
  EXPECT_EQ(format.getFontScript(), 2);
}

TEST_F(HybridXlsxFormatTest, SetBold) {
  format.setBold();
  EXPECT_TRUE(format.getIsBold());
}

TEST_F(HybridXlsxFormatTest, SetItalic) {
  format.setItalic();
  EXPECT_TRUE(format.getIsItalic());
}

TEST_F(HybridXlsxFormatTest, SetUnderlineWithValue) {
  format.setUnderline(2.0);
  EXPECT_EQ(format.getUnderline(), 2.0);
}

TEST_F(HybridXlsxFormatTest, SetUnderlineWithoutValue) {
  format.setUnderline(std::nullopt);
  EXPECT_EQ(format.getUnderline(), 1.0);
}

TEST_F(HybridXlsxFormatTest, SetStrikeout) {
  format.setStrikeout();
  EXPECT_TRUE(format.getIsStrikeout());
}

// Alignment Tests
TEST_F(HybridXlsxFormatTest, SetAlign) {
  format.setAlign(2.0);
  EXPECT_EQ(format.getAlign(), 2.0);
}

TEST_F(HybridXlsxFormatTest, SetVerticalAlign) {
  format.setVerticalAlign(1.0);
  EXPECT_EQ(format.getVerticalAlign(), 1.0);
}

TEST_F(HybridXlsxFormatTest, SetTextWrap) {
  format.setTextWrap();
  EXPECT_TRUE(format.getIsTextWrap());
}

TEST_F(HybridXlsxFormatTest, SetRotation) {
  format.setRotation(45.0);
  EXPECT_EQ(format.getRotation(), 45.0);
}

TEST_F(HybridXlsxFormatTest, SetIndent) {
  format.setIndent(3.0);
  EXPECT_EQ(format.getIndent(), 3.0);
}

TEST_F(HybridXlsxFormatTest, SetShrink) {
  format.setShrink();
  EXPECT_TRUE(format.getIsShrink());
}

// Number Format Tests
TEST_F(HybridXlsxFormatTest, SetNumFormat) {
  format.setNumFormat("#,##0.00");
  EXPECT_EQ(format.getNumFormat(), "#,##0.00");
}

TEST_F(HybridXlsxFormatTest, SetNumFormatIndex) {
  format.setNumFormatIndex(5.0);
  EXPECT_EQ(format.getNumFormatIndex(), 5.0);
}

// Color Tests
TEST_F(HybridXlsxFormatTest, SetBgColor) {
  format.setBgColor(0x00FF00);
  EXPECT_EQ(format.getBgColor(), 0x00FF00);
}

TEST_F(HybridXlsxFormatTest, SetFgColor) {
  format.setFgColor(0x0000FF);
  EXPECT_EQ(format.getFgColor(), 0x0000FF);
}

TEST_F(HybridXlsxFormatTest, SetPattern) {
  format.setPattern(1.0);
  EXPECT_EQ(format.getPattern(), 1.0);
}

// Border Tests
TEST_F(HybridXlsxFormatTest, SetBorder) {
  format.setBorder(1.0);
  EXPECT_EQ(format.getBorder(), 1.0);
}

TEST_F(HybridXlsxFormatTest, SetBorderColor) {
  format.setBorderColor(0x000000);
  EXPECT_EQ(format.getBorderColor(), 0x000000);
}

TEST_F(HybridXlsxFormatTest, SetBottom) {
  format.setBottom(1.0);
  EXPECT_EQ(format.getBottom(), 1.0);
}

TEST_F(HybridXlsxFormatTest, SetTop) {
  format.setTop(2.0);
  EXPECT_EQ(format.getTop(), 2.0);
}

TEST_F(HybridXlsxFormatTest, SetLeft) {
  format.setLeft(3.0);
  EXPECT_EQ(format.getLeft(), 3.0);
}

TEST_F(HybridXlsxFormatTest, SetRight) {
  format.setRight(4.0);
  EXPECT_EQ(format.getRight(), 4.0);
}

TEST_F(HybridXlsxFormatTest, SetBottomColor) {
  format.setBottomColor(0x111111);
  EXPECT_EQ(format.getBottomColor(), 0x111111);
}

TEST_F(HybridXlsxFormatTest, SetTopColor) {
  format.setTopColor(0x222222);
  EXPECT_EQ(format.getTopColor(), 0x222222);
}

TEST_F(HybridXlsxFormatTest, SetLeftColor) {
  format.setLeftColor(0x333333);
  EXPECT_EQ(format.getLeftColor(), 0x333333);
}

TEST_F(HybridXlsxFormatTest, SetRightColor) {
  format.setRightColor(0x444444);
  EXPECT_EQ(format.getRightColor(), 0x444444);
}

TEST_F(HybridXlsxFormatTest, SetDiagonal) {
  format.setDiagonal(1.0);
  EXPECT_EQ(format.getDiagonal(), 1.0);
}

TEST_F(HybridXlsxFormatTest, SetDiagonalColor) {
  format.setDiagonalColor(0x555555);
  EXPECT_EQ(format.getDiagonalColor(), 0x555555);
}

// Protection Tests
TEST_F(HybridXlsxFormatTest, SetLocked) {
  format.setLocked();
  EXPECT_TRUE(format.getIsLocked());
}

TEST_F(HybridXlsxFormatTest, SetUnlocked) {
  format.setLocked();
  format.setUnlocked();
  EXPECT_FALSE(format.getIsLocked());
}

// Other Tests
TEST_F(HybridXlsxFormatTest, SetHyperlink) {
  format.setHyperlink();
  EXPECT_TRUE(format.getIsHyperlink());
}

TEST_F(HybridXlsxFormatTest, SetFontOnly) {
  format.setFontOnly();
  EXPECT_TRUE(format.getIsFontOnly());
}

// Comprehensive Tests
TEST_F(HybridXlsxFormatTest, HeaderStyleCombination) {
  format.setBold();
  format.setFontColor(0xFFFFFF);
  format.setBgColor(0x0000FF);
  format.setAlign(2.0);
  format.setFontName("Arial");
  format.setFontSize(14.0);

  EXPECT_TRUE(format.getIsBold());
  EXPECT_EQ(format.getFontColor(), 0xFFFFFF);
  EXPECT_EQ(format.getBgColor(), 0x0000FF);
  EXPECT_EQ(format.getAlign(), 2.0);
  EXPECT_EQ(format.getFontName(), "Arial");
  EXPECT_EQ(format.getFontSize(), 14.0);
}

TEST_F(HybridXlsxFormatTest, CurrencyFormat) {
  format.setNumFormat("$#,##0.00");
  format.setFontColor(0x000000);
  format.setAlign(3.0);

  EXPECT_EQ(format.getNumFormat(), "$#,##0.00");
  EXPECT_EQ(format.getFontColor(), 0x000000);
  EXPECT_EQ(format.getAlign(), 3.0);
}

TEST_F(HybridXlsxFormatTest, ResetClearsAll) {
  format.setFontName("Arial");
  format.setFontSize(12.0);
  format.setBold();
  format.setItalic();
  format.setBgColor(0xFF0000);

  format.reset();

  EXPECT_EQ(format.getFontName(), "");
  EXPECT_EQ(format.getFontSize(), 0);
  EXPECT_FALSE(format.getIsBold());
  EXPECT_FALSE(format.getIsItalic());
  EXPECT_EQ(format.getBgColor(), 0);
}