#include <gtest/gtest.h>
#include <OpenXLSX.hpp>
#include <XLDocument.hpp>
#include <XLSheet.hpp>
#include <XLCell.hpp>
#include <XLCellRange.hpp>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <string>

namespace fs {

inline std::string temp_path(const std::string& suffix = ".xlsx") {
  auto now = std::chrono::system_clock::now().time_since_epoch().count();
  return "/tmp/nitro_xlsx_ws_test_" + std::to_string(now) + suffix;
}

inline void remove_file(const std::string& path) {
  std::remove(path.c_str());
}

} // namespace fs

// Test fixture for Worksheet tests
class HybridXlsxWorksheetTest : public ::testing::Test {
protected:
  std::unique_ptr<OpenXLSX::XLDocument> doc;
  std::string testFilePath;

  void SetUp() override {
    testFilePath = fs::temp_path();
    doc = std::make_unique<OpenXLSX::XLDocument>();
    doc->create(testFilePath, OpenXLSX::XLForceOverwrite);
    // OpenXLSX creates a default "Sheet1" - tests will add their own
  }

  void TearDown() override {
    if (doc) {
      doc->close();
    }
    if (!testFilePath.empty()) {
      fs::remove_file(testFilePath);
    }
  }
};

// Basic Worksheet Tests
TEST_F(HybridXlsxWorksheetTest, CreateWorksheet) {
  doc->workbook().addWorksheet("TestSheet");
  EXPECT_EQ(doc->workbook().worksheet("TestSheet").name(), "TestSheet");
}

TEST_F(HybridXlsxWorksheetTest, WorksheetName) {
  doc->workbook().addWorksheet("MySheet");
  EXPECT_EQ(doc->workbook().worksheet("MySheet").name(), "MySheet");
}

// Cell Writing Tests
TEST_F(HybridXlsxWorksheetTest, WriteStringCell) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = "Test";

  EXPECT_EQ(ws.cell(1, 1).value().get<std::string>(), "Test");
}

TEST_F(HybridXlsxWorksheetTest, WriteNumberCell) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = 3.14159;

  EXPECT_DOUBLE_EQ(ws.cell(1, 1).value().get<double>(), 3.14159);
}

TEST_F(HybridXlsxWorksheetTest, WriteIntegerCell) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = 42;

  EXPECT_EQ(ws.cell(1, 1).value().get<int>(), 42);
}

TEST_F(HybridXlsxWorksheetTest, WriteBooleanCell) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = true;
  ws.cell(1, 2).value() = false;

  EXPECT_TRUE(ws.cell(1, 1).value().get<bool>());
  EXPECT_FALSE(ws.cell(1, 2).value().get<bool>());
}

// Cell Type Tests
TEST_F(HybridXlsxWorksheetTest, CellTypeString) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = "Hello";
  EXPECT_EQ(ws.cell(1, 1).value().type(), OpenXLSX::XLValueType::String);
}

TEST_F(HybridXlsxWorksheetTest, CellTypeInteger) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = 42;
  EXPECT_EQ(ws.cell(1, 1).value().type(), OpenXLSX::XLValueType::Integer);
}

TEST_F(HybridXlsxWorksheetTest, CellTypeBoolean) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = true;
  EXPECT_EQ(ws.cell(1, 1).value().type(), OpenXLSX::XLValueType::Boolean);
}

TEST_F(HybridXlsxWorksheetTest, CellTypeEmpty) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  EXPECT_EQ(ws.cell(1, 1).value().type(), OpenXLSX::XLValueType::Empty);
}

// Formula Tests
TEST_F(HybridXlsxWorksheetTest, WriteFormula) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = 10.0;
  ws.cell(2, 1).value() = 20.0;
  ws.cell(3, 1).formula() = "A1+A2";

  EXPECT_EQ(ws.cell(3, 1).formula().get(), "A1+A2");
}

TEST_F(HybridXlsxWorksheetTest, ComplexFormula) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = 1.0;
  ws.cell(1, 2).value() = 2.0;
  ws.cell(1, 3).value() = 3.0;
  ws.cell(2, 1).formula() = "SUM(A1:C1)";
  ws.cell(2, 2).formula() = "AVERAGE(A1:C1)";
  ws.cell(2, 3).formula() = "MAX(A1:C1)";

  EXPECT_EQ(ws.cell(2, 1).formula().get(), "SUM(A1:C1)");
  EXPECT_EQ(ws.cell(2, 2).formula().get(), "AVERAGE(A1:C1)");
  EXPECT_EQ(ws.cell(2, 3).formula().get(), "MAX(A1:C1)");
}

// Row and Column Tests
TEST_F(HybridXlsxWorksheetTest, WriteMultipleRows) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");

  for (int row = 1; row <= 10; ++row) {
    ws.cell(row, 1).value() = static_cast<double>(row);
  }

  for (int row = 1; row <= 10; ++row) {
    EXPECT_DOUBLE_EQ(ws.cell(row, 1).value().get<double>(), row);
  }
}

TEST_F(HybridXlsxWorksheetTest, WriteMultipleColumns) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");

  for (int col = 1; col <= 5; ++col) {
    ws.cell(1, col).value() = static_cast<double>(col);
  }

  for (int col = 1; col <= 5; ++col) {
    EXPECT_DOUBLE_EQ(ws.cell(1, col).value().get<double>(), col);
  }
}

// Save and Reload Tests
TEST_F(HybridXlsxWorksheetTest, SaveAndReload) {
  {
    doc->workbook().addWorksheet("TestSheet1");
    auto ws = doc->workbook().worksheet("TestSheet1");
    ws.cell(1, 1).value() = "Persistent";
    ws.cell(2, 1).value() = 123.45;
    ws.cell(3, 1).value() = true;

    doc->save();
    doc->close();
  }

  // Reload
  OpenXLSX::XLDocument doc2;
  doc2.open(testFilePath);
  auto ws2 = doc2.workbook().worksheet("TestSheet1");

  EXPECT_EQ(ws2.cell(1, 1).value().get<std::string>(), "Persistent");
  EXPECT_DOUBLE_EQ(ws2.cell(2, 1).value().get<double>(), 123.45);
  EXPECT_TRUE(ws2.cell(3, 1).value().get<bool>());

  doc2.close();
  fs::remove_file(testFilePath);
}

TEST_F(HybridXlsxWorksheetTest, SaveWithFormulas) {
  {
    doc->workbook().addWorksheet("Formulas");
    auto ws = doc->workbook().worksheet("Formulas");
    ws.cell(1, 1).value() = 100.0;
    ws.cell(2, 1).value() = 200.0;
    ws.cell(3, 1).formula() = "A1+A2";
    ws.cell(4, 1).formula() = "A1*A2";

    doc->save();
    doc->close();
  }

  // Reload
  OpenXLSX::XLDocument doc2;
  doc2.open(testFilePath);
  auto ws2 = doc2.workbook().worksheet("Formulas");

  EXPECT_EQ(ws2.cell(3, 1).formula().get(), "A1+A2");
  EXPECT_EQ(ws2.cell(4, 1).formula().get(), "A1*A2");

  doc2.close();
  fs::remove_file(testFilePath);
}

// Edge Cases
TEST_F(HybridXlsxWorksheetTest, OverwriteCell) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = "Original";
  ws.cell(1, 1).value() = "Updated";

  EXPECT_EQ(ws.cell(1, 1).value().get<std::string>(), "Updated");
}

TEST_F(HybridXlsxWorksheetTest, SpecialCharacters) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = "Special: !@#$%^&*()_+-=[]{}|;':,./<>?";
  ws.cell(2, 1).value() = "Unicode: 你好世界";

  EXPECT_EQ(ws.cell(1, 1).value().get<std::string>(), "Special: !@#$%^&*()_+-=[]{}|;':,./<>?");
  EXPECT_EQ(ws.cell(2, 1).value().get<std::string>(), "Unicode: 你好世界");
}

TEST_F(HybridXlsxWorksheetTest, NegativeNumbers) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = -42.5;
  ws.cell(2, 1).value() = -1e10;
  ws.cell(3, 1).value() = -0.0001;

  EXPECT_DOUBLE_EQ(ws.cell(1, 1).value().get<double>(), -42.5);
  EXPECT_DOUBLE_EQ(ws.cell(2, 1).value().get<double>(), -1e10);
  EXPECT_DOUBLE_EQ(ws.cell(3, 1).value().get<double>(), -0.0001);
}

TEST_F(HybridXlsxWorksheetTest, VeryLargeNumbers) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = 1e15;
  ws.cell(2, 1).value() = 9.999e14;

  EXPECT_DOUBLE_EQ(ws.cell(1, 1).value().get<double>(), 1e15);
  EXPECT_DOUBLE_EQ(ws.cell(2, 1).value().get<double>(), 9.999e14);
}

TEST_F(HybridXlsxWorksheetTest, EmptyString) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  ws.cell(1, 1).value() = "";

  EXPECT_EQ(ws.cell(1, 1).value().get<std::string>(), "");
}

TEST_F(HybridXlsxWorksheetTest, LongString) {
  doc->workbook().addWorksheet("TestSheet1");
  auto ws = doc->workbook().worksheet("TestSheet1");
  std::string longString(1000, 'A');
  ws.cell(1, 1).value() = longString;

  EXPECT_EQ(ws.cell(1, 1).value().get<std::string>(), longString);
}

// ========== deleteRow ==========

TEST_F(HybridXlsxWorksheetTest, DeleteRow) {
  doc->workbook().addWorksheet("DeleteRow");
  auto ws = doc->workbook().worksheet("DeleteRow");
  ws.cell(1, 1).value() = "Row1";
  ws.cell(2, 1).value() = "Row2";
  ws.cell(3, 1).value() = "Row3";

  EXPECT_TRUE(ws.deleteRow(2));
  EXPECT_EQ(ws.cell(1, 1).value().get<std::string>(), "Row1");
  EXPECT_EQ(ws.cell(3, 1).value().get<std::string>(), "Row3");
}

TEST_F(HybridXlsxWorksheetTest, DeleteRowMissingReturnsFalse) {
  doc->workbook().addWorksheet("DeleteRowMissing");
  auto ws = doc->workbook().worksheet("DeleteRowMissing");
  ws.cell(1, 1).value() = "Only";
  EXPECT_FALSE(ws.deleteRow(99));
}

// ========== unmergeCells ==========

TEST_F(HybridXlsxWorksheetTest, MergeThenUnmerge) {
  doc->workbook().addWorksheet("Unmerge");
  auto ws = doc->workbook().worksheet("Unmerge");
  ws.mergeCells("A1:B2");
  EXPECT_EQ(ws.merges().count(), 1u);

  ws.unmergeCells("A1:B2");
  EXPECT_EQ(ws.merges().count(), 0u);
}

// ========== hasFormula / formula / findCell ==========

TEST_F(HybridXlsxWorksheetTest, HasFormulaAndFormula) {
  doc->workbook().addWorksheet("Formulas2");
  auto ws = doc->workbook().worksheet("Formulas2");
  ws.cell(1, 1).value() = 10.0;
  ws.cell(2, 1).value() = 20.0;
  ws.cell(3, 1).formula() = "A1+A2";
  ws.cell(1, 2).value() = "plain";

  EXPECT_TRUE(ws.cell(3, 1).hasFormula());
  EXPECT_FALSE(ws.cell(1, 2).hasFormula());
  EXPECT_EQ(ws.cell(3, 1).formula().get(), "A1+A2");
  EXPECT_FALSE(ws.findCell(50, 50).hasFormula());
}

TEST_F(HybridXlsxWorksheetTest, FindCell) {
  doc->workbook().addWorksheet("FindCell");
  auto ws = doc->workbook().worksheet("FindCell");
  ws.cell(2, 3).value() = "Found";

  EXPECT_FALSE(ws.findCell(2, 3).empty());
  EXPECT_EQ(ws.findCell(2, 3).value().get<std::string>(), "Found");
  // findCell does not create missing cells
  EXPECT_TRUE(ws.findCell(1, 1).empty());
  EXPECT_TRUE(ws.findCell(10, 10).empty());
}

// ========== Comments ==========

TEST_F(HybridXlsxWorksheetTest, SetAndGetComment) {
  doc->workbook().addWorksheet("Comments");
  auto ws = doc->workbook().worksheet("Comments");
  ws.comments().set("A1", "Hello comment", 0);

  EXPECT_EQ(ws.comments().count(), 1u);
  EXPECT_EQ(ws.comments().get("A1"), "Hello comment");
}

TEST_F(HybridXlsxWorksheetTest, DeleteComment) {
  doc->workbook().addWorksheet("CommentsDel");
  auto ws = doc->workbook().worksheet("CommentsDel");
  ws.comments().set("B2", "To be deleted", 0);
  EXPECT_EQ(ws.comments().count(), 1u);

  EXPECT_TRUE(ws.comments().deleteComment("B2"));
  EXPECT_EQ(ws.comments().count(), 0u);
  EXPECT_EQ(ws.comments().get("B2"), "");
}

TEST_F(HybridXlsxWorksheetTest, CommentAuthor) {
  doc->workbook().addWorksheet("CommentAuthor");
  auto ws = doc->workbook().worksheet("CommentAuthor");
  uint16_t authorId = ws.comments().addAuthor("Denis");
  ws.comments().set("A1", "authored", authorId);

  EXPECT_EQ(ws.comments().author(authorId), "Denis");
  EXPECT_EQ(ws.comments().authorId("A1"), authorId);
}

// ========== Conditional formatting ==========

TEST_F(HybridXlsxWorksheetTest, ConditionalFormatsCreate) {
  doc->workbook().addWorksheet("CF");
  auto ws = doc->workbook().worksheet("CF");
  auto cfs = ws.conditionalFormats();

  size_t idx = cfs.create();
  auto cf = cfs.conditionalFormatByIndex(idx);
  cf.setSqref("A1:A10");

  EXPECT_EQ(cfs.count(), 1u);
  EXPECT_EQ(cf.sqref(), "A1:A10");
}

TEST_F(HybridXlsxWorksheetTest, ConditionalFormatRules) {
  doc->workbook().addWorksheet("CFRules");
  auto ws = doc->workbook().worksheet("CFRules");
  auto cfs = ws.conditionalFormats();

  size_t idx = cfs.create();
  auto cf = cfs.conditionalFormatByIndex(idx);
  cf.setSqref("B1:B5");

  auto rules = cf.cfRules();
  size_t ruleIdx = rules.create();
  auto rule = rules.cfRuleByIndex(ruleIdx);

  rule.setType(OpenXLSX::XLCfType::CellIs);
  rule.setOperator(OpenXLSX::XLCfOperator::GreaterThan);
  rule.setFormula("100");
  rule.setDxfId(0);
  rules.setPriority(ruleIdx, 1);

  EXPECT_EQ(rules.count(), 1u);
  EXPECT_EQ(rule.type(), OpenXLSX::XLCfType::CellIs);
  EXPECT_EQ(rule.Operator(), OpenXLSX::XLCfOperator::GreaterThan);
  EXPECT_EQ(rule.formula(), "100");
  EXPECT_EQ(rule.dxfId(), 0u);
  EXPECT_EQ(rule.priority(), 1u);
}

TEST_F(HybridXlsxWorksheetTest, ConditionalFormatExpressionRule) {
  doc->workbook().addWorksheet("CFExpr");
  auto ws = doc->workbook().worksheet("CFExpr");
  auto cfs = ws.conditionalFormats();

  size_t idx = cfs.create();
  auto cf = cfs.conditionalFormatByIndex(idx);
  cf.setSqref("C1:C20");

  auto rules = cf.cfRules();
  size_t ruleIdx = rules.create();
  auto rule = rules.cfRuleByIndex(ruleIdx);

  rule.setType(OpenXLSX::XLCfType::Expression);
  rule.setFormula("MOD(ROW(),2)=0");
  rule.setStopIfTrue(true);

  EXPECT_EQ(rule.type(), OpenXLSX::XLCfType::Expression);
  EXPECT_EQ(rule.formula(), "MOD(ROW(),2)=0");
  EXPECT_TRUE(rule.stopIfTrue());
}

// ========== Sheet protection ==========

TEST_F(HybridXlsxWorksheetTest, ProtectSheet) {
  doc->workbook().addWorksheet("Protect");
  auto ws = doc->workbook().worksheet("Protect");

  EXPECT_FALSE(ws.sheetProtected());
  EXPECT_TRUE(ws.protectSheet());
  EXPECT_TRUE(ws.sheetProtected());

  // protectSheet(false) turns protection off
  EXPECT_TRUE(ws.protectSheet(false));
  EXPECT_FALSE(ws.sheetProtected());
}

TEST_F(HybridXlsxWorksheetTest, ProtectObjectsAndScenarios) {
  doc->workbook().addWorksheet("ProtectObj");
  auto ws = doc->workbook().worksheet("ProtectObj");

  EXPECT_FALSE(ws.objectsProtected());
  EXPECT_FALSE(ws.scenariosProtected());

  EXPECT_TRUE(ws.protectObjects());
  EXPECT_TRUE(ws.protectScenarios());
  EXPECT_TRUE(ws.objectsProtected());
  EXPECT_TRUE(ws.scenariosProtected());

  EXPECT_TRUE(ws.protectObjects(false));
  EXPECT_FALSE(ws.objectsProtected());
}

// ========== Password ==========

TEST_F(HybridXlsxWorksheetTest, SetPasswordAndCheck) {
  doc->workbook().addWorksheet("Pwd");
  auto ws = doc->workbook().worksheet("Pwd");

  EXPECT_FALSE(ws.passwordIsSet());
  EXPECT_TRUE(ws.setPassword("secret"));
  EXPECT_TRUE(ws.passwordIsSet());
  EXPECT_FALSE(ws.passwordHash().empty());

  EXPECT_TRUE(ws.clearPassword());
  EXPECT_FALSE(ws.passwordIsSet());
}

TEST_F(HybridXlsxWorksheetTest, SetPasswordHash) {
  doc->workbook().addWorksheet("PwdHash");
  auto ws = doc->workbook().worksheet("PwdHash");

  // 4-digit hex hash as produced by ExcelPasswordHashAsString
  std::string hash = OpenXLSX::ExcelPasswordHashAsString("secret");
  EXPECT_FALSE(hash.empty());

  EXPECT_TRUE(ws.setPasswordHash(hash));
  EXPECT_TRUE(ws.passwordIsSet());
  EXPECT_EQ(ws.passwordHash(), hash);
}

TEST_F(HybridXlsxWorksheetTest, ClearSheetProtection) {
  doc->workbook().addWorksheet("ClearProt");
  auto ws = doc->workbook().worksheet("ClearProt");

  ws.setPassword("pw");
  ws.protectSheet();
  ws.allowDeleteRows();
  EXPECT_TRUE(ws.sheetProtected());

  EXPECT_TRUE(ws.clearSheetProtection());
  EXPECT_FALSE(ws.sheetProtected());
  EXPECT_FALSE(ws.passwordIsSet());
}

// ========== Fine-grained permissions ==========

TEST_F(HybridXlsxWorksheetTest, AllowAndDenyPermissions) {
  doc->workbook().addWorksheet("Perms");
  auto ws = doc->workbook().worksheet("Perms");
  ws.protectSheet();

  // Defaults on a protected sheet: insert/delete not allowed, select is allowed
  EXPECT_FALSE(ws.insertColumnsAllowed());
  EXPECT_FALSE(ws.insertRowsAllowed());
  EXPECT_FALSE(ws.deleteColumnsAllowed());
  EXPECT_FALSE(ws.deleteRowsAllowed());
  EXPECT_TRUE(ws.selectLockedCellsAllowed());
  EXPECT_TRUE(ws.selectUnlockedCellsAllowed());

  // allow* enables the action despite protection
  EXPECT_TRUE(ws.allowDeleteColumns());
  EXPECT_TRUE(ws.allowDeleteRows());
  EXPECT_TRUE(ws.deleteColumnsAllowed());
  EXPECT_TRUE(ws.deleteRowsAllowed());

  // deny* disables the action again
  EXPECT_TRUE(ws.denyDeleteColumns());
  EXPECT_TRUE(ws.denyDeleteRows());
  EXPECT_FALSE(ws.deleteColumnsAllowed());
  EXPECT_FALSE(ws.deleteRowsAllowed());

  // select: default allowed, deny turns it off, allow turns it back on
  EXPECT_TRUE(ws.denySelectLockedCells());
  EXPECT_FALSE(ws.selectLockedCellsAllowed());
  EXPECT_TRUE(ws.allowSelectLockedCells());
  EXPECT_TRUE(ws.selectLockedCellsAllowed());

  EXPECT_TRUE(ws.denySelectUnlockedCells());
  EXPECT_FALSE(ws.selectUnlockedCellsAllowed());
  EXPECT_TRUE(ws.allowSelectUnlockedCells());
  EXPECT_TRUE(ws.selectUnlockedCellsAllowed());

  // insert
  EXPECT_TRUE(ws.allowInsertColumns());
  EXPECT_TRUE(ws.allowInsertRows());
  EXPECT_TRUE(ws.insertColumnsAllowed());
  EXPECT_TRUE(ws.insertRowsAllowed());
  EXPECT_TRUE(ws.denyInsertColumns());
  EXPECT_TRUE(ws.denyInsertRows());
  EXPECT_FALSE(ws.insertColumnsAllowed());
  EXPECT_FALSE(ws.insertRowsAllowed());
}

TEST_F(HybridXlsxWorksheetTest, AllowExplicitFalse) {
  doc->workbook().addWorksheet("AllowFalse");
  auto ws = doc->workbook().worksheet("AllowFalse");
  ws.protectSheet();
  ws.allowDeleteRows(true);
  EXPECT_TRUE(ws.deleteRowsAllowed());

  ws.allowDeleteRows(false);
  EXPECT_FALSE(ws.deleteRowsAllowed());
}

// ========== Protection persists across save/load ==========

TEST_F(HybridXlsxWorksheetTest, ProtectionPersists) {
  {
    doc->workbook().addWorksheet("PersistProt");
    auto ws = doc->workbook().worksheet("PersistProt");
    ws.setPassword("s3cret");
    ws.protectSheet();
    ws.allowDeleteRows();
    ws.denySelectLockedCells();
    doc->save();
    doc->close();
  }

  OpenXLSX::XLDocument doc2;
  doc2.open(testFilePath);
  auto ws2 = doc2.workbook().worksheet("PersistProt");

  EXPECT_TRUE(ws2.sheetProtected());
  EXPECT_TRUE(ws2.passwordIsSet());
  EXPECT_TRUE(ws2.deleteRowsAllowed());
  EXPECT_FALSE(ws2.selectLockedCellsAllowed());

  doc2.close();
  fs::remove_file(testFilePath);
}

TEST_F(HybridXlsxWorksheetTest, SheetProtectionSummary) {
  doc->workbook().addWorksheet("Summary");
  auto ws = doc->workbook().worksheet("Summary");
  ws.protectSheet();
  std::string summary = ws.sheetProtectionSummary();
  EXPECT_FALSE(summary.empty());
}

// ========== Visibility (hide / activate) ==========

TEST_F(HybridXlsxWorksheetTest, HideSheet) {
  doc->workbook().addWorksheet("Visible");
  doc->workbook().addWorksheet("ToHide");
  auto ws = doc->workbook().worksheet("ToHide");

  EXPECT_EQ(ws.visibility(), OpenXLSX::XLSheetState::Visible);
  EXPECT_EQ(ws.name(), "ToHide");

  ws.setVisibility(OpenXLSX::XLSheetState::Hidden);

  // Sheet is hidden but still fully readable
  EXPECT_EQ(ws.visibility(), OpenXLSX::XLSheetState::Hidden);
  EXPECT_EQ(ws.name(), "ToHide");
  ws.cell(1, 1).value() = "StillHere";
  EXPECT_EQ(ws.cell(1, 1).value().get<std::string>(), "StillHere");
}

TEST_F(HybridXlsxWorksheetTest, HidePersistsAcrossSaveLoad) {
  {
    doc->workbook().addWorksheet("PersistHide");
    auto ws = doc->workbook().worksheet("PersistHide");
    ws.setVisibility(OpenXLSX::XLSheetState::Hidden);
    doc->save();
    doc->close();
  }

  OpenXLSX::XLDocument doc2;
  doc2.open(testFilePath);
  auto ws2 = doc2.workbook().worksheet("PersistHide");
  EXPECT_EQ(ws2.visibility(), OpenXLSX::XLSheetState::Hidden);

  doc2.close();
  fs::remove_file(testFilePath);
}

TEST_F(HybridXlsxWorksheetTest, ActivateSheet) {
  doc->workbook().addWorksheet("First");
  doc->workbook().addWorksheet("Second");
  auto second = doc->workbook().worksheet("Second");

  second.setActive();
  EXPECT_TRUE(second.isActive());
}
