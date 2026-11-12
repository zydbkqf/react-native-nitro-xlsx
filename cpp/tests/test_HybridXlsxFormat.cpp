#include <gtest/gtest.h>
#include "format_testable.hpp"

#include <OpenXLSX.hpp>
#include <XLDocument.hpp>
#include <XLSheet.hpp>
#include <XLCell.hpp>
#include <chrono>
#include <cstdio>
#include <memory>
#include <set>
#include <string>

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

// ============================================================================
// applyToDocument regression tests against a real XLDocument / XLStyles
// ============================================================================

namespace {

std::string format_temp_path() {
  auto now = std::chrono::system_clock::now().time_since_epoch().count();
  return "/tmp/nitro_xlsx_fmt_test_" + std::to_string(now) + ".xlsx";
}

} // namespace

class FormatApplyTest : public ::testing::Test {
protected:
  std::unique_ptr<OpenXLSX::XLDocument> doc;
  std::string testFilePath;

  void SetUp() override {
    testFilePath = format_temp_path();
    doc = std::make_unique<OpenXLSX::XLDocument>();
    doc->create(testFilePath, OpenXLSX::XLForceOverwrite);
  }

  void TearDown() override {
    if (doc) {
      doc->close();
    }
    if (!testFilePath.empty()) {
      std::remove(testFilePath.c_str());
    }
  }

  OpenXLSX::XLStyles& styles() { return doc->styles(); }

  // Find the numFmt entry carrying the given format code, if any.
  bool findNumFmtByCode(const std::string& code, uint32_t& outId) {
    for (size_t i = 0; i < styles().numberFormats().count(); ++i) {
      OpenXLSX::XLNumberFormat nf = styles().numberFormats()[i];
      if (nf.formatCode() == code) {
        outId = nf.numberFormatId();
        return true;
      }
    }
    return false;
  }
};

// Bug 2: black (#000000) must not be treated as "unset".
TEST_F(FormatApplyTest, BlackFontColorLandsInStyles) {
  TestableFormat fmt;
  fmt.setFontColor(0x000000);

  OpenXLSX::XLStyleIndex idx = fmt.applyToDocument(styles());
  OpenXLSX::XLCellFormat cf = styles().cellFormats()[idx];
  ASSERT_TRUE(cf.applyFont());
  OpenXLSX::XLFont font = styles().fonts()[cf.fontIndex()];
  EXPECT_EQ(font.fontColor().hex(), "ff000000");
}

TEST_F(FormatApplyTest, BlackBgColorLandsInStyles) {
  TestableFormat fmt;
  fmt.setBgColor(0x000000);
  fmt.setFgColor(0x000000);

  OpenXLSX::XLStyleIndex idx = fmt.applyToDocument(styles());
  OpenXLSX::XLCellFormat cf = styles().cellFormats()[idx];
  ASSERT_TRUE(cf.applyFill());
  OpenXLSX::XLFill fill = styles().fills()[cf.fillIndex()];
  EXPECT_EQ(fill.backgroundColor().hex(), "ff000000");
  EXPECT_EQ(fill.color().hex(), "ff000000");
}

// Bug 3: setUnlocked() must write locked=false + applyProtection.
TEST_F(FormatApplyTest, SetUnlockedWritesLockedFalse) {
  TestableFormat fmt;
  fmt.setUnlocked();

  OpenXLSX::XLStyleIndex idx = fmt.applyToDocument(styles());
  OpenXLSX::XLCellFormat cf = styles().cellFormats()[idx];
  EXPECT_TRUE(cf.applyProtection());
  EXPECT_FALSE(cf.locked());
}

TEST_F(FormatApplyTest, SetLockedWritesLockedTrue) {
  TestableFormat fmt;
  fmt.setLocked();

  OpenXLSX::XLStyleIndex idx = fmt.applyToDocument(styles());
  OpenXLSX::XLCellFormat cf = styles().cellFormats()[idx];
  EXPECT_TRUE(cf.applyProtection());
  EXPECT_TRUE(cf.locked());
}

// Bug 4: setNumFormatIndex() must write the built-in numFmtId.
TEST_F(FormatApplyTest, NumFormatIndexWritesId) {
  TestableFormat fmt;
  fmt.setNumFormatIndex(14.0); // built-in date format

  OpenXLSX::XLStyleIndex idx = fmt.applyToDocument(styles());
  OpenXLSX::XLCellFormat cf = styles().cellFormats()[idx];
  EXPECT_TRUE(cf.applyNumberFormat());
  EXPECT_EQ(cf.numberFormatId(), 14u);
}

// Bug 4: a custom format string wins over the index when both are set.
TEST_F(FormatApplyTest, NumFormatStringWinsOverIndex) {
  TestableFormat fmt;
  fmt.setNumFormat("0.0000");
  fmt.setNumFormatIndex(14.0);

  OpenXLSX::XLStyleIndex idx = fmt.applyToDocument(styles());
  OpenXLSX::XLCellFormat cf = styles().cellFormats()[idx];
  EXPECT_TRUE(cf.applyNumberFormat());
  uint32_t id = 0;
  ASSERT_TRUE(findNumFmtByCode("0.0000", id));
  EXPECT_EQ(cf.numberFormatId(), id);
  EXPECT_NE(cf.numberFormatId(), 14u);
}

// Bug 1: a setter between two applies must not serve the stale cached style.
TEST_F(FormatApplyTest, SetterBetweenAppliesProducesNewStyle) {
  TestableFormat fmt;
  fmt.setBold();

  OpenXLSX::XLStyleIndex idx1 = fmt.applyToDocument(styles());
  EXPECT_TRUE(styles().fonts()[styles().cellFormats()[idx1].fontIndex()].bold());

  fmt.setFontColor(0xFF0000);
  OpenXLSX::XLStyleIndex idx2 = fmt.applyToDocument(styles());
  ASSERT_NE(idx1, idx2);

  OpenXLSX::XLCellFormat cf2 = styles().cellFormats()[idx2];
  OpenXLSX::XLFont font2 = styles().fonts()[cf2.fontIndex()];
  EXPECT_TRUE(font2.bold());
  EXPECT_EQ(font2.fontColor().hex(), "ffff0000");

  // Without a setter in between, the cached index is reused.
  EXPECT_EQ(fmt.applyToDocument(styles()), idx2);
}

// Bug 5: custom numFmtId must not collide with ids already in the document.
TEST_F(FormatApplyTest, CustomNumFmtIdDoesNotCollideWithExistingIds) {
  for (uint32_t id = 164; id <= 179; ++id) {
    OpenXLSX::XLStyleIndex nfIdx = styles().numberFormats().create();
    OpenXLSX::XLNumberFormat nf = styles().numberFormats()[nfIdx];
    nf.setFormatCode("0.000_" + std::to_string(id));
    nf.setNumberFormatId(id);
  }

  TestableFormat fmt;
  fmt.setNumFormat("0.00%");
  OpenXLSX::XLStyleIndex idx = fmt.applyToDocument(styles());
  OpenXLSX::XLCellFormat cf = styles().cellFormats()[idx];

  uint32_t assigned = 0;
  ASSERT_TRUE(findNumFmtByCode("0.00%", assigned));
  EXPECT_EQ(assigned, 180u);
  EXPECT_EQ(cf.numberFormatId(), assigned);

  std::set<uint32_t> ids;
  for (size_t i = 0; i < styles().numberFormats().count(); ++i) {
    EXPECT_TRUE(ids.insert(styles().numberFormats()[i].numberFormatId()).second)
        << "duplicate numFmtId " << styles().numberFormats()[i].numberFormatId();
  }
}

// Bug 5: identical format codes reuse an existing numFmt entry.
TEST_F(FormatApplyTest, IdenticalFormatCodeReusesNumFmt) {
  TestableFormat fmt1;
  fmt1.setNumFormat("$#,##0.00");
  fmt1.applyToDocument(styles());

  TestableFormat fmt2;
  fmt2.setNumFormat("$#,##0.00");
  fmt2.applyToDocument(styles());

  size_t matches = 0;
  for (size_t i = 0; i < styles().numberFormats().count(); ++i) {
    if (styles().numberFormats()[i].formatCode() == "$#,##0.00") matches++;
  }
  EXPECT_EQ(matches, 1u);
}

// Bug 6: the style cache must not leak across documents.
TEST_F(FormatApplyTest, StyleIndexNotReusedAcrossDocuments) {
  TestableFormat fmt;
  fmt.setFontName("Comic Sans MS");

  OpenXLSX::XLStyleIndex idx1 = fmt.applyToDocument(styles());

  std::string path2 = format_temp_path() + ".second";
  OpenXLSX::XLDocument doc2;
  doc2.create(path2, OpenXLSX::XLForceOverwrite);
  OpenXLSX::XLStyles& styles2 = doc2.styles();

  OpenXLSX::XLStyleIndex idx2 = fmt.applyToDocument(styles2);
  OpenXLSX::XLCellFormat cf2 = styles2.cellFormats()[idx2];
  EXPECT_TRUE(cf2.applyFont());
  EXPECT_EQ(styles2.fonts()[cf2.fontIndex()].fontName(), "Comic Sans MS");

  // The first document is untouched by the second apply.
  OpenXLSX::XLCellFormat cf1 = styles().cellFormats()[idx1];
  EXPECT_TRUE(cf1.applyFont());

  doc2.close();
  std::remove(path2.c_str());
}

// Bug 7: border styles and colors must materialize.
TEST_F(FormatApplyTest, BordersMaterialize) {
  TestableFormat fmt;
  fmt.setBorder(2.0);            // Medium on all four sides
  fmt.setBorderColor(0x0000FF);  // ... in blue
  fmt.setTop(5.0);               // Top overridden to Thick
  fmt.setTopColor(0xFF0000);     // ... in red
  fmt.setRight(0.0);             // Right keeps the all-sides Medium

  OpenXLSX::XLStyleIndex idx = fmt.applyToDocument(styles());
  OpenXLSX::XLCellFormat cf = styles().cellFormats()[idx];
  ASSERT_TRUE(cf.applyBorder());
  OpenXLSX::XLBorder border = styles().borders()[cf.borderIndex()];

  EXPECT_EQ(border.top().style(), OpenXLSX::XLLineStyle::XLLineStyleThick);
  EXPECT_EQ(border.top().color().rgb().hex(), "ffff0000");
  EXPECT_EQ(border.left().style(), OpenXLSX::XLLineStyle::XLLineStyleMedium);
  EXPECT_EQ(border.left().color().rgb().hex(), "ff0000ff");
  EXPECT_EQ(border.bottom().style(), OpenXLSX::XLLineStyle::XLLineStyleMedium);
  EXPECT_EQ(border.right().style(), OpenXLSX::XLLineStyle::XLLineStyleMedium);
}

// Bug 7: a side with only a color gets a THIN line; diagonal gets direction flags.
TEST_F(FormatApplyTest, BorderColorOnlyAndDiagonalMaterialize) {
  TestableFormat fmt;
  fmt.setBottomColor(0x00FF00); // no style -> THIN
  fmt.setDiagonal(6.0);         // Double diagonal
  fmt.setDiagonalColor(0x123456);

  OpenXLSX::XLStyleIndex idx = fmt.applyToDocument(styles());
  OpenXLSX::XLCellFormat cf = styles().cellFormats()[idx];
  ASSERT_TRUE(cf.applyBorder());
  OpenXLSX::XLBorder border = styles().borders()[cf.borderIndex()];

  EXPECT_EQ(border.bottom().style(), OpenXLSX::XLLineStyle::XLLineStyleThin);
  EXPECT_EQ(border.bottom().color().rgb().hex(), "ff00ff00");
  // Untouched sides stay empty.
  EXPECT_FALSE(static_cast<bool>(border.left()));

  EXPECT_EQ(border.diagonal().style(), OpenXLSX::XLLineStyle::XLLineStyleDouble);
  EXPECT_EQ(border.diagonal().color().rgb().hex(), "ff123456");
  EXPECT_TRUE(border.diagonalUp());
  EXPECT_TRUE(border.diagonalDown());
}
// Regression: reusing one style object for a second document must not hit the
// cache of the first document just because XLStyles was reallocated at the same
// address (that used to hand out a cell format index the new document never
// created).
TEST(FormatCacheIsolation, SecondDocumentGetsItsOwnCellFormat) {
  TestableFormat fmt;
  fmt.setBold();

  std::unique_ptr<OpenXLSX::XLDocument> docA = std::make_unique<OpenXLSX::XLDocument>();
  std::string pathA = format_temp_path();
  docA->create(pathA, OpenXLSX::XLForceOverwrite);
  OpenXLSX::XLStyleIndex idxA = fmt.applyToDocument(docA->styles(), /*generation*/ 1);
  size_t countA = docA->styles().cellFormats().count();
  EXPECT_LT(static_cast<size_t>(idxA), countA);
  docA->close();
  docA.reset();

  std::unique_ptr<OpenXLSX::XLDocument> docB = std::make_unique<OpenXLSX::XLDocument>();
  std::string pathB = format_temp_path();
  docB->create(pathB, OpenXLSX::XLForceOverwrite);
  OpenXLSX::XLStyleIndex idxB = fmt.applyToDocument(docB->styles(), /*generation*/ 2);

  // docB must have created its own format, and the index must be in range.
  EXPECT_GE(docB->styles().cellFormats().count(), 2u);
  EXPECT_LT(static_cast<size_t>(idxB), docB->styles().cellFormats().count());

  docB->close();
  std::remove(pathA.c_str());
  std::remove(pathB.c_str());
}

// The cache still works within one document.
TEST(FormatCacheIsolation, SameDocumentReusesCachedIndex) {
  std::unique_ptr<OpenXLSX::XLDocument> doc = std::make_unique<OpenXLSX::XLDocument>();
  std::string path = format_temp_path();
  doc->create(path, OpenXLSX::XLForceOverwrite);

  TestableFormat fmt;
  fmt.setBold();
  auto idx1 = fmt.applyToDocument(doc->styles(), 7);
  auto idx2 = fmt.applyToDocument(doc->styles(), 7);
  EXPECT_EQ(idx1, idx2);
  EXPECT_EQ(doc->styles().cellFormats().count(), 2u);

  doc->close();
  std::remove(path.c_str());
}

// A solid fill is painted with the foreground colour; a background-only colour
// must be mirrored into fg or it is invisible in Excel.
TEST_F(FormatApplyTest, SolidBgColorMirrorsIntoFg) {
  TestableFormat fmt;
  fmt.setBgColor(0x0000FF); // blue, no fg
  OpenXLSX::XLStyleIndex idx = fmt.applyToDocument(styles());
  OpenXLSX::XLFill fill = styles().fills()[styles().cellFormats()[idx].fillIndex()];
  EXPECT_EQ(fill.patternType(), OpenXLSX::XLPatternType::XLPatternSolid);
  EXPECT_EQ(fill.color().hex(), "ff0000ff");
}

TEST_F(FormatApplyTest, ExplicitFgColorNotOverriddenByBg) {
  TestableFormat fmt;
  fmt.setBgColor(0x0000FF);
  fmt.setFgColor(0xFF0000); // red wins
  OpenXLSX::XLStyleIndex idx = fmt.applyToDocument(styles());
  OpenXLSX::XLFill fill = styles().fills()[styles().cellFormats()[idx].fillIndex()];
  EXPECT_EQ(fill.color().hex(), "ffff0000");
}

// Same styles address but a different generation (workbook reopened in place):
// the cache must rebuild instead of returning the stale index.
TEST(FormatCacheIsolation, DifferentGenerationInvalidatesCache) {
  std::unique_ptr<OpenXLSX::XLDocument> doc = std::make_unique<OpenXLSX::XLDocument>();
  std::string path = format_temp_path();
  doc->create(path, OpenXLSX::XLForceOverwrite);

  TestableFormat fmt;
  fmt.setBold();
  auto idx1 = fmt.applyToDocument(doc->styles(), 1);
  auto idx2 = fmt.applyToDocument(doc->styles(), 2);
  EXPECT_NE(idx1, idx2);
  EXPECT_EQ(doc->styles().cellFormats().count(), 3u); // default + one per apply

  doc->close();
  std::remove(path.c_str());
}
