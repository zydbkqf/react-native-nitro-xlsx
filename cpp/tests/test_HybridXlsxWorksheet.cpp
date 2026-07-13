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
