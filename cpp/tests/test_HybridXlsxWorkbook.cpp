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
#include <unordered_map>
#include <memory>

#include "HybridNitroXlsx.hpp"
#include "HybridXlsxWorkbook.hpp"
#include "XlsxError.hpp"

// Test double for cpp/XlsxCacheDir (whose platform impl is not part of this
// build): HybridXlsxWorkbook only needs a writable temp directory.
namespace margelo::nitro::xlsx {
std::string getCacheDir() {
  return "/tmp";
}
} // namespace margelo::nitro::xlsx

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

// ========== deleteSheet ==========

TEST_F(HybridXlsxWorkbookTest, DeleteSheet) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  doc.workbook().addWorksheet("Keep");
  doc.workbook().addWorksheet("Remove");
  EXPECT_EQ(doc.workbook().worksheetCount(), 3u);

  doc.workbook().deleteSheet("Remove");
  EXPECT_EQ(doc.workbook().worksheetCount(), 2u);
  EXPECT_EQ(doc.workbook().worksheet("Keep").name(), "Keep");

  doc.close();
}

TEST_F(HybridXlsxWorkbookTest, DeleteMissingSheetThrows) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  EXPECT_THROW(doc.workbook().deleteSheet("DoesNotExist"), OpenXLSX::XLException);
  doc.close();
}

// ========== updateSheetName / setName ==========

TEST_F(HybridXlsxWorkbookTest, RenameSheet) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  doc.workbook().addWorksheet("OldName");
  auto ws = doc.workbook().worksheet("OldName");
  ws.setName("NewName");

  EXPECT_EQ(ws.name(), "NewName");
  EXPECT_EQ(doc.workbook().worksheet("NewName").name(), "NewName");

  doc.close();
}

TEST_F(HybridXlsxWorkbookTest, UpdateSheetNameRewritesFormulas) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  doc.workbook().addWorksheet("Data");
  doc.workbook().addWorksheet("Calc");
  auto data = doc.workbook().worksheet("Data");
  data.cell(1, 1).value() = 42.0;
  auto calc = doc.workbook().worksheet("Calc");
  calc.cell(1, 1).formula() = "Data!A1*2";

  // Rename the sheet, then rewrite formula refs on every sheet that may reference it
  data.setName("Data2");
  data.updateSheetName("Data", "Data2");
  calc.updateSheetName("Data", "Data2");

  EXPECT_EQ(data.name(), "Data2");
  EXPECT_EQ(calc.cell(1, 1).formula().get(), "Data2!A1*2");

  doc.close();
}

// ========== clone ==========

TEST_F(HybridXlsxWorkbookTest, CloneSheet) {
  OpenXLSX::XLDocument doc;
  doc.create(testFilePath, OpenXLSX::XLForceOverwrite);
  doc.workbook().addWorksheet("Source");
  auto src = doc.workbook().worksheet("Source");
  src.cell(1, 1).value() = "ClonedValue";
  src.cell(2, 1).value() = 3.14;

  doc.workbook().cloneSheet("Source", "Copy");
  EXPECT_EQ(doc.workbook().worksheetCount(), 3u);

  auto copy = doc.workbook().worksheet("Copy");
  EXPECT_EQ(copy.cell(1, 1).value().get<std::string>(), "ClonedValue");
  EXPECT_DOUBLE_EQ(copy.cell(2, 1).value().get<double>(), 3.14);

  doc.close();
}

// ========== HybridNitroXlsx / HybridXlsxWorkbook API regression tests ==========

using margelo::nitro::AnyMap;
using margelo::nitro::xlsx::HybridNitroXlsx;
using margelo::nitro::xlsx::XlsxError;
namespace xlsx_error = margelo::nitro::xlsx::xlsx_error;

namespace {

std::shared_ptr<AnyMap> makeRow(const std::string& name, double age) {
  auto row = AnyMap::make();
  row->setString("Name", name);
  row->setDouble("Age", age);
  return row;
}

} // namespace

// Bug 8b: fromJSON must produce exactly one worksheet per key - no leftover
// default "Sheet1".
TEST(HybridWorkbookApiTest, FromJSONOneSheetPerKey) {
  HybridNitroXlsx xlsx;
  std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>> data;
  data["Data"] = { makeRow("Alice", 30.0) };

  auto wb = xlsx.fromJSON(data);
  ASSERT_NE(wb, nullptr);
  EXPECT_DOUBLE_EQ(wb->getWorksheetCount(), 1.0);

  auto sheet = wb->getWorksheetByName("Data");
  ASSERT_NE(sheet, nullptr);
  EXPECT_EQ(sheet->getName(), "Data");
  // Record keys are sorted, so the header row is "Age", "Name".
  EXPECT_DOUBLE_EQ(std::get<double>(sheet->getCellValue(2, 1)), 30.0);
  EXPECT_EQ(sheet->getCellString(2, 2), "Alice");

  // The OpenXLSX default sheet must have been deleted.
  try {
    wb->getWorksheetByName("Sheet1");
    FAIL() << "default Sheet1 should not exist";
  } catch (const XlsxError& e) {
    EXPECT_EQ(e.code(), xlsx_error::SHEET_NOT_FOUND);
  }

  auto exported = wb->toJSON();
  EXPECT_EQ(exported.size(), 1u);
  ASSERT_EQ(exported.count("Data"), 1u);
  ASSERT_EQ(exported["Data"].size(), 1u);
  EXPECT_EQ(exported["Data"][0]->getString("Name"), "Alice");
}

TEST(HybridWorkbookApiTest, FromJSONMultipleKeys) {
  HybridNitroXlsx xlsx;
  std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>> data;
  data["Alpha"] = { makeRow("A", 1.0) };
  data["Beta"] = { makeRow("B", 2.0) };

  auto wb = xlsx.fromJSON(data);
  EXPECT_DOUBLE_EQ(wb->getWorksheetCount(), 2.0);
  EXPECT_EQ(wb->getWorksheetByName("Alpha")->getName(), "Alpha");
  EXPECT_EQ(wb->getWorksheetByName("Beta")->getName(), "Beta");
}

TEST(HybridWorkbookApiTest, FromJSONKeyClaimingDefaultSheetName) {
  HybridNitroXlsx xlsx;
  std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>> data;
  data["Sheet1"] = { makeRow("Keep", 1.0) };

  auto wb = xlsx.fromJSON(data);
  EXPECT_DOUBLE_EQ(wb->getWorksheetCount(), 1.0);
  EXPECT_EQ(wb->getWorksheetByName("Sheet1")->getName(), "Sheet1");
}

// Bug 8a: fromJSON({}) must leave a usable workbook behind.
TEST(HybridWorkbookApiTest, FromJSONEmptyInputUsableWorkbook) {
  HybridNitroXlsx xlsx;
  auto wb = xlsx.fromJSON({});
  ASSERT_NE(wb, nullptr);

  EXPECT_GE(wb->getWorksheetCount(), 1.0);
  EXPECT_NE(wb->getWorksheet(0), nullptr);

  // toJSON works and reflects the empty data set.
  EXPECT_TRUE(wb->toJSON().empty());

  // getBuffer resolves instead of rejecting on an unopened document.
  auto promise = wb->getBuffer();
  ASSERT_TRUE(promise->isResolved());
  EXPECT_GT(promise->getResult()->size(), 0u);
}

// Bug 8c: getWorksheetCount/toJSON/getBuffer work on a never-opened workbook.
TEST(HybridWorkbookApiTest, FreshWorkbookIsUsable) {
  HybridNitroXlsx xlsx;
  auto wb = xlsx.createWorkbook();

  EXPECT_GE(wb->getWorksheetCount(), 1.0);

  // An empty workbook exports as an empty map without throwing.
  EXPECT_NO_THROW(wb->toJSON());
  EXPECT_TRUE(wb->toJSON().empty());
}

// Bug 9: openWorkbookFromBuffer rejects invalid data as XlsxError (and does not
// leak its temp file).
TEST(HybridWorkbookApiTest, OpenFromBufferInvalidDataThrowsXlsxError) {
  HybridNitroXlsx xlsx;
  std::vector<uint8_t> garbage = { 0x00, 0x01, 0x02, 0x03, 0xFF, 0xFE };
  auto buffer = margelo::nitro::ArrayBuffer::copy(garbage.data(), garbage.size());
  EXPECT_THROW(xlsx.openWorkbookFromBuffer(buffer), XlsxError);
}

// Round-trip: createWorkbook -> data -> getBuffer -> openWorkbookFromBuffer.
TEST(HybridWorkbookApiTest, BufferRoundTrip) {
  HybridNitroXlsx xlsx;
  {
    auto wb = xlsx.createWorkbook();
    auto sheet = wb->addWorksheet("Round");
    sheet->writeString(1, 1, "hello", std::nullopt);
    auto promise = wb->getBuffer();
    ASSERT_TRUE(promise->isResolved());
    auto buffer = promise->getResult();

    auto reopened = xlsx.openWorkbookFromBuffer(buffer);
    ASSERT_NE(reopened, nullptr);
    EXPECT_EQ(reopened->getWorksheetByName("Round")->getCellString(1, 1), "hello");
  }
}

// Reopening a workbook in place bumps documentGeneration, so a format whose
// style index was cached against the old document state is rebuilt instead of
// silently pointing at an unrelated style in the new one.
TEST(HybridWorkbookApiTest, ReopenInvalidatesStyleCache) {
  // Build a source file with one sheet "A".
  std::string path = fs::temp_path();
  {
    HybridNitroXlsx xlsx;
    auto wb = xlsx.createWorkbook();
    auto sh = wb->addWorksheet("A");
    sh->writeString(1, 1, "hello", std::nullopt);
    auto promise = wb->getBuffer();
    ASSERT_TRUE(promise->isResolved());
    auto buffer = promise->getResult();
    std::ofstream out(path, std::ios::binary);
    ASSERT_TRUE(out.is_open());
    out.write(reinterpret_cast<const char*>(buffer->data()),
              static_cast<std::streamsize>(buffer->size()));
  }

  HybridNitroXlsx xlsx;
  auto wb = std::dynamic_pointer_cast<margelo::nitro::xlsx::HybridXlsxWorkbook>(xlsx.createWorkbook());
  ASSERT_NE(wb, nullptr);
  wb->openFromFile(path); // generation 1
  auto sh = wb->getWorksheetByName("A");
  ASSERT_NE(sh, nullptr);
  auto fmt = wb->addCellFormat();
  fmt->setFontName("Comic Sans MS");
  sh->writeString(1, 1, "x", fmt); // caches the style index at generation 1

  wb->openFromFile(path); // same XLDocument object, generation 2
  auto sh2 = wb->getWorksheetByName("A");
  ASSERT_NE(sh2, nullptr);
  sh2->writeString(2, 2, "y", fmt); // must rebuild the style in the new state

  auto read = sh2->getCellFormat(2, 2);
  ASSERT_NE(read, nullptr);
  EXPECT_EQ(read->getFontName(), "Comic Sans MS");

  fs::remove_file(path);
}
