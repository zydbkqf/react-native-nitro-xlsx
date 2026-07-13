#include <gtest/gtest.h>
#include <OpenXLSX.hpp>
#include <XLDocument.hpp>
#include <XLSheet.hpp>
#include <XLCell.hpp>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <string>
#include <vector>

namespace fs {

inline std::string temp_path(const std::string& suffix = ".xlsx") {
  auto now = std::chrono::system_clock::now().time_since_epoch().count();
  return "/tmp/nitro_xlsx_test_" + std::to_string(now) + suffix;
}

inline void remove_file(const std::string& path) {
  std::remove(path.c_str());
}

} // namespace fs

// Test fixture for Workbook tests
class HybridXlsxWorkbookTest : public ::testing::Test {
protected:
  std::string testFilePath;

  void SetUp() override {
    testFilePath = fs::temp_path();
  }

  void TearDown() override {
    if (!testFilePath.empty()) {
      fs::remove_file(testFilePath);
    }
  }
};

// Basic Workbook Creation Tests
TEST_F(HybridXlsxWorkbookTest, CreateNewWorkbook) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  EXPECT_TRUE(doc.isOpen());

  doc.close();
}

TEST_F(HybridXlsxWorkbookTest, CreateAndAddWorksheet) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  // OpenXLSX creates a default "Sheet1" - count is 1
  EXPECT_EQ(doc.workbook().worksheetCount(), 1u);

  doc.workbook().addWorksheet("TestSheet1");
  EXPECT_EQ(doc.workbook().worksheetCount(), 2u);

  doc.close();
}

TEST_F(HybridXlsxWorkbookTest, AddMultipleWorksheets) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  // OpenXLSX creates a default "Sheet1" - count is 1
  EXPECT_EQ(doc.workbook().worksheetCount(), 1u);

  doc.workbook().addWorksheet("TestSheet1");
  doc.workbook().addWorksheet("Sheet2");
  doc.workbook().addWorksheet("Sheet3");

  EXPECT_EQ(doc.workbook().worksheetCount(), 4u);

  doc.close();
}

TEST_F(HybridXlsxWorkbookTest, GetWorksheetByIndex) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);

  doc.workbook().addWorksheet("TestSheet1");
  doc.workbook().addWorksheet("Sheet2");

  // OpenXLSX creates default "Sheet1" at index 1
  EXPECT_EQ(doc.workbook().worksheet(1).name(), "Sheet1");
  EXPECT_EQ(doc.workbook().worksheet(2).name(), "TestSheet1");
  EXPECT_EQ(doc.workbook().worksheet(3).name(), "Sheet2");

  doc.close();
}

TEST_F(HybridXlsxWorkbookTest, GetWorksheetByName) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);

  doc.workbook().addWorksheet("MySheet");

  auto ws = doc.workbook().worksheet("MySheet");
  EXPECT_EQ(ws.name(), "MySheet");

  doc.close();
}

TEST_F(HybridXlsxWorkbookTest, GetOrAddWorksheet_Existing) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);

  doc.workbook().addWorksheet("TestSheet1");
  size_t countBefore = doc.workbook().worksheetCount();

  // Getting existing worksheet should not increase count
  auto ws = doc.workbook().worksheet("TestSheet1");
  size_t countAfter = doc.workbook().worksheetCount();

  EXPECT_EQ(countBefore, countAfter);
  EXPECT_EQ(ws.name(), "TestSheet1");

  doc.close();
}

// Save and Load Tests
TEST_F(HybridXlsxWorkbookTest, SaveWorkbook) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  doc.workbook().addWorksheet("TestSheet");

  doc.save();
  doc.close();

  // Verify file exists
  std::ifstream file(testFilePath, std::ios::binary);
  EXPECT_TRUE(file.good());
  file.close();
}

TEST_F(HybridXlsxWorkbookTest, OpenExistingWorkbook) {
  // First create a workbook
  {
    OpenXLSX::XLDocument doc;
    doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
    doc.workbook().addWorksheet("TestSheet");
    doc.save();
    doc.close();
  }

  // Then open it
  OpenXLSX::XLDocument doc;
  doc.open(testFilePath);
  EXPECT_TRUE(doc.isOpen());
  // OpenXLSX creates a default "Sheet1", so 2 worksheets after adding "TestSheet"
  EXPECT_EQ(doc.workbook().worksheetCount(), 2u);

  auto ws = doc.workbook().worksheet("TestSheet");
  EXPECT_EQ(ws.name(), "TestSheet");

  doc.close();
}

TEST_F(HybridXlsxWorkbookTest, OpenFromBuffer) {
  // First create and save a workbook
  {
    OpenXLSX::XLDocument doc;
    doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
    doc.workbook().addWorksheet("BufferTest");
    auto ws = doc.workbook().worksheet("BufferTest");
    ws.cell(1, 1).value() = "Hello";
    doc.save();
    doc.close();
  }

  // Read the file into a buffer
  std::ifstream file(testFilePath, std::ios::binary | std::ios::ate);
  ASSERT_TRUE(file.good());
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<uint8_t> buffer(size);
  file.read(reinterpret_cast<char*>(buffer.data()), size);
  file.close();

  // Open from the buffer (here just opening from file again to verify)
  OpenXLSX::XLDocument doc;
  doc.open(testFilePath);
  EXPECT_TRUE(doc.isOpen());
  // 2 worksheets: default "Sheet1" + "BufferTest"
  EXPECT_EQ(doc.workbook().worksheetCount(), 2u);

  doc.close();
}

// Worksheet Data Tests
TEST_F(HybridXlsxWorkbookTest, WriteAndReadString) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  doc.workbook().addWorksheet("TestSheet1");
  auto ws = doc.workbook().worksheet("TestSheet1");

  ws.cell(1, 1).value() = "Hello World";
  doc.save();
  doc.close();

  // Read back
  OpenXLSX::XLDocument doc2;
  doc2.open(testFilePath);
  auto ws2 = doc2.workbook().worksheet("TestSheet1");
  EXPECT_EQ(ws2.cell(1, 1).value().get<std::string>(), "Hello World");

  doc2.close();
}

TEST_F(HybridXlsxWorkbookTest, WriteAndReadNumber) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  doc.workbook().addWorksheet("TestSheet1");
  auto ws = doc.workbook().worksheet("TestSheet1");

  ws.cell(1, 1).value() = 42.5;
  ws.cell(2, 1).value() = -100;
  ws.cell(3, 1).value() = 0;

  doc.save();
  doc.close();

  // Read back
  OpenXLSX::XLDocument doc2;
  doc2.open(testFilePath);
  auto ws2 = doc2.workbook().worksheet("TestSheet1");
  EXPECT_DOUBLE_EQ(ws2.cell(1, 1).value().get<double>(), 42.5);
  EXPECT_DOUBLE_EQ(ws2.cell(2, 1).value().get<double>(), -100.0);
  EXPECT_DOUBLE_EQ(ws2.cell(3, 1).value().get<double>(), 0.0);

  doc2.close();
}

TEST_F(HybridXlsxWorkbookTest, WriteAndReadBoolean) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  doc.workbook().addWorksheet("TestSheet1");
  auto ws = doc.workbook().worksheet("TestSheet1");

  ws.cell(1, 1).value() = true;
  ws.cell(2, 1).value() = false;

  doc.save();
  doc.close();

  // Read back
  OpenXLSX::XLDocument doc2;
  doc2.open(testFilePath);
  auto ws2 = doc2.workbook().worksheet("TestSheet1");
  EXPECT_TRUE(ws2.cell(1, 1).value().get<bool>());
  EXPECT_FALSE(ws2.cell(2, 1).value().get<bool>());

  doc2.close();
}

TEST_F(HybridXlsxWorkbookTest, WriteAndReadFormula) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  doc.workbook().addWorksheet("TestSheet1");
  auto ws = doc.workbook().worksheet("TestSheet1");

  ws.cell(1, 1).value() = 10.0;
  ws.cell(2, 1).value() = 20.0;
  ws.cell(3, 1).formula() = "SUM(A1:A2)";

  doc.save();
  doc.close();

  // Read back
  OpenXLSX::XLDocument doc2;
  doc2.open(testFilePath);
  auto ws2 = doc2.workbook().worksheet("TestSheet1");
  EXPECT_EQ(ws2.cell(3, 1).formula().get(), "SUM(A1:A2)");

  doc2.close();
}

// Multiple Cells Test
TEST_F(HybridXlsxWorkbookTest, WriteMultipleCells) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  doc.workbook().addWorksheet("TestSheet1");
  auto ws = doc.workbook().worksheet("TestSheet1");

  for (int row = 1; row <= 5; ++row) {
    for (int col = 1; col <= 3; ++col) {
      ws.cell(row, col).value() = static_cast<double>(row * 10 + col);
    }
  }

  doc.save();
  doc.close();

  // Read back
  OpenXLSX::XLDocument doc2;
  doc2.open(testFilePath);
  auto ws2 = doc2.workbook().worksheet("TestSheet1");

  for (int row = 1; row <= 5; ++row) {
    for (int col = 1; col <= 3; ++col) {
      EXPECT_DOUBLE_EQ(ws2.cell(row, col).value().get<double>(), row * 10 + col);
    }
  }

  doc2.close();
}

// Worksheet Operations Tests
TEST_F(HybridXlsxWorkbookTest, DeleteWorksheet) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);

  doc.workbook().addWorksheet("TestSheet1");
  doc.workbook().addWorksheet("Sheet2");
  doc.workbook().addWorksheet("Sheet3");

  // 4 worksheets: default "Sheet1" + 3 added
  EXPECT_EQ(doc.workbook().worksheetCount(), 4u);

  doc.workbook().deleteSheet("Sheet2");
  EXPECT_EQ(doc.workbook().worksheetCount(), 3u);

  doc.close();
}

TEST_F(HybridXlsxWorkbookTest, WorksheetCount) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);

  // Default "Sheet1" exists
  EXPECT_EQ(doc.workbook().worksheetCount(), 1u);

  doc.workbook().addWorksheet("TestSheet1");
  EXPECT_EQ(doc.workbook().worksheetCount(), 2u);

  doc.workbook().addWorksheet("Sheet2");
  EXPECT_EQ(doc.workbook().worksheetCount(), 3u);

  doc.workbook().addWorksheet("Sheet3");
  EXPECT_EQ(doc.workbook().worksheetCount(), 4u);

  doc.close();
}

// Edge Cases
TEST_F(HybridXlsxWorkbookTest, MultipleOpenClose) {
  for (int i = 0; i < 5; ++i) {
    OpenXLSX::XLDocument doc;
    doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
    // Use a name that doesn't conflict with default "Sheet1" on the first iteration
    doc.workbook().addWorksheet("MySheet" + std::to_string(i));
    doc.save();
    doc.close();

    OpenXLSX::XLDocument doc2;
    doc2.open(testFilePath);
    EXPECT_TRUE(doc2.isOpen());
    doc2.close();

    fs::remove_file(testFilePath);
  }
}

TEST_F(HybridXlsxWorkbookTest, SaveLoadLargeData) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  doc.workbook().addWorksheet("LargeData");
  auto ws = doc.workbook().worksheet("LargeData");

  // Write 100x10 grid of data
  for (int row = 1; row <= 100; ++row) {
    for (int col = 1; col <= 10; ++col) {
      ws.cell(row, col).value() = static_cast<double>(row * col);
    }
  }

  doc.save();
  doc.close();

  // Read back
  OpenXLSX::XLDocument doc2;
  doc2.open(testFilePath);
  auto ws2 = doc2.workbook().worksheet("LargeData");

  for (int row = 1; row <= 100; ++row) {
    for (int col = 1; col <= 10; ++col) {
      EXPECT_DOUBLE_EQ(ws2.cell(row, col).value().get<double>(), row * col);
    }
  }

  doc2.close();
}
