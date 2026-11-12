#include <gtest/gtest.h>
#include <OpenXLSX.hpp>
#include <XLDocument.hpp>
#include <XLSheet.hpp>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <string>
#include <vector>

#include "XlsxJson.hpp"
#include "HybridXlsxConditionalFormats.hpp"
#include "XlsxError.hpp"

using margelo::nitro::xlsx::worksheetToRecords;
using margelo::nitro::xlsx::recordsToWorksheet;
using margelo::nitro::xlsx::workbookToRecords;
using margelo::nitro::xlsx::HybridXlsxConditionalFormats;
using margelo::nitro::xlsx::HybridXlsxConditionalFormatSpec;
using margelo::nitro::xlsx::XlsxError;
namespace xlsx_error = margelo::nitro::xlsx::xlsx_error;
using margelo::nitro::AnyMap;

namespace {

// Runs fn and expects an XlsxError whose code (and optional message substring) match.
template <typename Fn>
::testing::AssertionResult throwsXlsxError(Fn&& fn,
                                           const std::string& code,
                                           const std::string& messageContains = "") {
  try {
    fn();
  } catch (const XlsxError& e) {
    if (e.code() != code) {
      return ::testing::AssertionFailure()
             << "expected code " << code << ", got " << e.code() << " (" << e.what() << ")";
    }
    if (!messageContains.empty() && std::string(e.what()).find(messageContains) == std::string::npos) {
      return ::testing::AssertionFailure() << "message missing '" << messageContains << "': " << e.what();
    }
    return ::testing::AssertionSuccess();
  } catch (const std::exception& e) {
    return ::testing::AssertionFailure() << "expected XlsxError, got std::exception: " << e.what();
  } catch (...) {
    return ::testing::AssertionFailure() << "expected XlsxError, got unknown exception";
  }
  return ::testing::AssertionFailure() << "no exception thrown";
}

} // namespace

namespace fs {

inline std::string temp_path(const std::string& suffix = ".xlsx") {
  auto now = std::chrono::system_clock::now().time_since_epoch().count();
  return "/tmp/nitro_xlsx_json_test_" + std::to_string(now) + suffix;
}

inline void remove_file(const std::string& path) {
  std::remove(path.c_str());
}

} // namespace fs

class XlsxJsonTest : public ::testing::Test {
protected:
  std::unique_ptr<OpenXLSX::XLDocument> doc;
  std::string testFilePath;

  void SetUp() override {
    testFilePath = fs::temp_path();
    doc = std::make_unique<OpenXLSX::XLDocument>();
    doc->create(testFilePath, OpenXLSX::XLForceOverwrite);
  }

  void TearDown() override {
    if (doc) doc->close();
    if (!testFilePath.empty()) fs::remove_file(testFilePath);
  }
};

// ========== worksheetToRecords ==========

TEST_F(XlsxJsonTest, ToRecordsUsesFirstRowAsKeys) {
  doc->workbook().addWorksheet("Data");
  auto ws = doc->workbook().worksheet("Data");
  ws.cell(1, 1).value() = "Name";
  ws.cell(1, 2).value() = "Age";
  ws.cell(2, 1).value() = "Alice";
  ws.cell(2, 2).value() = 30.0;
  ws.cell(3, 1).value() = "Bob";
  ws.cell(3, 2).value() = 25.0;

  auto rows = worksheetToRecords(ws);
  ASSERT_EQ(rows.size(), 2u);

  EXPECT_EQ(rows[0]->getString("Name"), "Alice");
  EXPECT_DOUBLE_EQ(rows[0]->getDouble("Age"), 30.0);
  EXPECT_EQ(rows[1]->getString("Name"), "Bob");
  EXPECT_DOUBLE_EQ(rows[1]->getDouble("Age"), 25.0);
}

TEST_F(XlsxJsonTest, ToRecordsWithCustomKeys) {
  doc->workbook().addWorksheet("Data");
  auto ws = doc->workbook().worksheet("Data");
  ws.cell(1, 1).value() = "Alice";
  ws.cell(1, 2).value() = 30.0;

  auto rows = worksheetToRecords(ws, std::vector<std::string>{"firstName", "yearsOld"});
  ASSERT_EQ(rows.size(), 1u);
  EXPECT_EQ(rows[0]->getString("firstName"), "Alice");
  EXPECT_DOUBLE_EQ(rows[0]->getDouble("yearsOld"), 30.0);
}

TEST_F(XlsxJsonTest, ToRecordsEmptySheet) {
  doc->workbook().addWorksheet("Empty");
  auto ws = doc->workbook().worksheet("Empty");
  EXPECT_TRUE(worksheetToRecords(ws).empty());
}

TEST_F(XlsxJsonTest, ToRecordsSkipsEmptyRows) {
  doc->workbook().addWorksheet("Sparse");
  auto ws = doc->workbook().worksheet("Sparse");
  ws.cell(1, 1).value() = "Name";
  ws.cell(2, 1).value() = "Alice";
  // row 3 left empty
  ws.cell(4, 1).value() = "Bob";

  auto rows = worksheetToRecords(ws);
  ASSERT_EQ(rows.size(), 2u);
  EXPECT_EQ(rows[0]->getString("Name"), "Alice");
  EXPECT_EQ(rows[1]->getString("Name"), "Bob");
}

TEST_F(XlsxJsonTest, ToRecordsHandlesMixedTypes) {
  doc->workbook().addWorksheet("Types");
  auto ws = doc->workbook().worksheet("Types");
  ws.cell(1, 1).value() = "S";
  ws.cell(1, 2).value() = "N";
  ws.cell(1, 3).value() = "B";
  ws.cell(2, 1).value() = "text";
  ws.cell(2, 2).value() = 3.14;
  ws.cell(2, 3).value() = true;

  auto rows = worksheetToRecords(ws);
  ASSERT_EQ(rows.size(), 1u);
  EXPECT_EQ(rows[0]->getString("S"), "text");
  EXPECT_DOUBLE_EQ(rows[0]->getDouble("N"), 3.14);
  EXPECT_TRUE(rows[0]->getBoolean("B"));
}

// ========== recordsToWorksheet ==========

TEST_F(XlsxJsonTest, FromRecordsWritesHeaderAndData) {
  doc->workbook().addWorksheet("Data");
  auto ws = doc->workbook().worksheet("Data");

  std::vector<std::shared_ptr<AnyMap>> rows;
  auto r1 = AnyMap::make();
  r1->setString("Name", "Alice");
  r1->setDouble("Age", 30);
  rows.push_back(r1);
  auto r2 = AnyMap::make();
  r2->setString("Name", "Bob");
  r2->setDouble("Age", 25);
  rows.push_back(r2);

  recordsToWorksheet(ws, rows);

  // AnyMap key order is not insertion order — keys are sorted per row
  // then merged first-seen, so "Age" comes before "Name".
  EXPECT_EQ(ws.cell(1, 1).value().get<std::string>(), "Age");
  EXPECT_EQ(ws.cell(1, 2).value().get<std::string>(), "Name");
  EXPECT_DOUBLE_EQ(ws.cell(2, 1).value().get<double>(), 30.0);
  EXPECT_EQ(ws.cell(2, 2).value().get<std::string>(), "Alice");
  EXPECT_DOUBLE_EQ(ws.cell(3, 1).value().get<double>(), 25.0);
  EXPECT_EQ(ws.cell(3, 2).value().get<std::string>(), "Bob");
}

TEST_F(XlsxJsonTest, FromRecordsEmptyInputIsNoop) {
  doc->workbook().addWorksheet("Data");
  auto ws = doc->workbook().worksheet("Data");
  recordsToWorksheet(ws, {});
  EXPECT_EQ(ws.cell(1, 1).value().type(), OpenXLSX::XLValueType::Empty);
}

TEST_F(XlsxJsonTest, FromRecordsExtendsKeysFromLaterRows) {
  doc->workbook().addWorksheet("Data");
  auto ws = doc->workbook().worksheet("Data");

  std::vector<std::shared_ptr<AnyMap>> rows;
  auto r1 = AnyMap::make();
  r1->setString("Name", "Alice");
  rows.push_back(r1);
  auto r2 = AnyMap::make();
  r2->setString("Name", "Bob");
  r2->setBoolean("Active", true);
  rows.push_back(r2);

  recordsToWorksheet(ws, rows);

  // Header covers both keys, first appearance order preserved
  EXPECT_EQ(ws.cell(1, 1).value().get<std::string>(), "Name");
  EXPECT_EQ(ws.cell(1, 2).value().get<std::string>(), "Active");
  EXPECT_EQ(ws.cell(2, 1).value().get<std::string>(), "Alice");
  EXPECT_EQ(ws.cell(3, 2).value().get<bool>(), true);
}

TEST_F(XlsxJsonTest, FromRecordsWritesInt64AndBool) {
  doc->workbook().addWorksheet("Types");
  auto ws = doc->workbook().worksheet("Types");

  auto r = AnyMap::make();
  r->setInt64("Count", 42);
  r->setBoolean("On", true);
  std::vector<std::shared_ptr<AnyMap>> rows{r};

  recordsToWorksheet(ws, rows);
  // Keys sorted per row: Count, On
  EXPECT_EQ(ws.cell(1, 1).value().get<std::string>(), "Count");
  EXPECT_EQ(ws.cell(1, 2).value().get<std::string>(), "On");
  EXPECT_DOUBLE_EQ(ws.cell(2, 1).value().get<double>(), 42.0);
  EXPECT_TRUE(ws.cell(2, 2).value().get<bool>());
}

// ========== Round-trip ==========

TEST_F(XlsxJsonTest, RoundTrip) {
  doc->workbook().addWorksheet("RT");
  auto ws = doc->workbook().worksheet("RT");

  std::vector<std::shared_ptr<AnyMap>> original;
  auto r1 = AnyMap::make();
  r1->setString("Name", "Alice");
  r1->setDouble("Age", 30);
  r1->setBoolean("Active", true);
  original.push_back(r1);
  auto r2 = AnyMap::make();
  r2->setString("Name", "Bob");
  r2->setDouble("Age", 25);
  r2->setBoolean("Active", false);
  original.push_back(r2);

  recordsToWorksheet(ws, original);
  auto restored = worksheetToRecords(ws);

  ASSERT_EQ(restored.size(), 2u);
  EXPECT_EQ(restored[0]->getString("Name"), "Alice");
  EXPECT_DOUBLE_EQ(restored[0]->getDouble("Age"), 30.0);
  EXPECT_TRUE(restored[0]->getBoolean("Active"));
  EXPECT_EQ(restored[1]->getString("Name"), "Bob");
  EXPECT_DOUBLE_EQ(restored[1]->getDouble("Age"), 25.0);
  EXPECT_FALSE(restored[1]->getBoolean("Active"));
}

// ========== workbookToRecords (sheet name as key) ==========

TEST_F(XlsxJsonTest, WorkbookToRecordsKeysBySheetName) {
  // OpenXLSX creates a default "Sheet1" — add real sheets first, then drop it
  doc->workbook().addWorksheet("Data");
  doc->workbook().addWorksheet("Summary");
  doc->workbook().deleteSheet("Sheet1");

  auto data = doc->workbook().worksheet("Data");
  data.cell(1, 1).value() = "Name";
  data.cell(2, 1).value() = "Alice";

  auto summary = doc->workbook().worksheet("Summary");
  summary.cell(1, 1).value() = "Total";
  summary.cell(2, 1).value() = 1.0;

  auto wb = doc->workbook();
  auto record = workbookToRecords(wb);
  ASSERT_EQ(record.size(), 2u);
  EXPECT_TRUE(record.count("Data") == 1);
  EXPECT_TRUE(record.count("Summary") == 1);

  ASSERT_EQ(record["Data"].size(), 1u);
  EXPECT_EQ(record["Data"][0]->getString("Name"), "Alice");

  ASSERT_EQ(record["Summary"].size(), 1u);
  EXPECT_DOUBLE_EQ(record["Summary"][0]->getDouble("Total"), 1.0);
}

TEST_F(XlsxJsonTest, WorkbookToRecordsEmptySheets) {
  doc->workbook().addWorksheet("OnlyHeader");
  doc->workbook().deleteSheet("Sheet1");
  auto ws = doc->workbook().worksheet("OnlyHeader");
  ws.cell(1, 1).value() = "Col";

  auto wb = doc->workbook();
  auto record = workbookToRecords(wb);
  ASSERT_EQ(record.size(), 1u);
  EXPECT_TRUE(record["OnlyHeader"].empty());
}

// ========== fromJSON multi-sheet shape (Record keyed by sheet name) ==========

TEST_F(XlsxJsonTest, RecordKeyedBySheetNameRoundTrip) {
  // Simulate NitroXlsx.fromJSON({ Data: [...], Summary: [...] })
  std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>> input;

  auto d1 = AnyMap::make();
  d1->setString("Name", "Alice");
  d1->setDouble("Age", 30);
  input["Data"] = {d1};

  auto s1 = AnyMap::make();
  s1->setDouble("Total", 42);
  input["Summary"] = {s1};

  for (const auto& [name, rows] : input) {
    doc->workbook().addWorksheet(name);
    auto ws = doc->workbook().worksheet(name);
    recordsToWorksheet(ws, rows);
  }
  doc->workbook().deleteSheet("Sheet1");

  auto wb = doc->workbook();
  auto output = workbookToRecords(wb);
  ASSERT_EQ(output.size(), 2u);
  ASSERT_EQ(output["Data"].size(), 1u);
  EXPECT_EQ(output["Data"][0]->getString("Name"), "Alice");
  EXPECT_DOUBLE_EQ(output["Data"][0]->getDouble("Age"), 30.0);
  ASSERT_EQ(output["Summary"].size(), 1u);
  EXPECT_DOUBLE_EQ(output["Summary"][0]->getDouble("Total"), 42.0);
}

// ========== Regression: float header keys must not be truncated to int ==========

TEST_F(XlsxJsonTest, FloatHeaderKeysNotTruncated) {
  doc->workbook().addWorksheet("Data");
  auto ws = doc->workbook().worksheet("Data");
  ws.cell(1, 1).value() = 3.5;
  ws.cell(1, 2).value() = 3.9;
  ws.cell(1, 3).value() = 30.0;
  ws.cell(2, 1).value() = "a";
  ws.cell(2, 2).value() = "b";
  ws.cell(2, 3).value() = "c";

  auto rows = worksheetToRecords(ws);
  ASSERT_EQ(rows.size(), 1u);

  // 3.5 and 3.9 must stay distinct lossless keys — not both truncated to "3"
  auto keys = rows[0]->getAllKeys();
  EXPECT_EQ(keys.size(), 3u);
  EXPECT_TRUE(rows[0]->contains("3.5"));
  EXPECT_TRUE(rows[0]->contains("3.9"));
  EXPECT_FALSE(rows[0]->contains("3"));
  EXPECT_EQ(rows[0]->getString("3.5"), "a");
  EXPECT_EQ(rows[0]->getString("3.9"), "b");
  // Integral values render like the Integer branch (30.0 -> "30", not "30.000000")
  EXPECT_TRUE(rows[0]->contains("30"));
  EXPECT_EQ(rows[0]->getString("30"), "c");
}

TEST_F(XlsxJsonTest, FloatHeaderKeysRoundTrip) {
  doc->workbook().addWorksheet("RT");
  auto ws = doc->workbook().worksheet("RT");

  std::vector<std::shared_ptr<AnyMap>> original;
  auto r = AnyMap::make();
  r->setString("3.5", "x");
  r->setString("3.9", "y");
  original.push_back(r);

  recordsToWorksheet(ws, original);
  auto restored = worksheetToRecords(ws);
  ASSERT_EQ(restored.size(), 1u);
  EXPECT_TRUE(restored[0]->contains("3.5"));
  EXPECT_TRUE(restored[0]->contains("3.9"));
  EXPECT_EQ(restored[0]->getString("3.5"), "x");
  EXPECT_EQ(restored[0]->getString("3.9"), "y");
}

// ========== Regression: custom keys beyond the sheet width are null-filled ==========

TEST_F(XlsxJsonTest, CustomKeysBeyondSheetWidthYieldNull) {
  doc->workbook().addWorksheet("Data");
  auto ws = doc->workbook().worksheet("Data");
  ws.cell(1, 1).value() = "Alice";
  ws.cell(1, 2).value() = 30.0;

  auto rows = worksheetToRecords(ws, std::vector<std::string>{"firstName", "yearsOld", "extra"});
  ASSERT_EQ(rows.size(), 1u);
  EXPECT_EQ(rows[0]->getString("firstName"), "Alice");
  EXPECT_DOUBLE_EQ(rows[0]->getDouble("yearsOld"), 30.0);
  // "extra" is beyond the 2-column sheet: present with a null value, not dropped
  EXPECT_TRUE(rows[0]->contains("extra"));
  EXPECT_TRUE(rows[0]->isNull("extra"));
}

// ========== Regression: conditional format sqref / rule index validation ==========

TEST_F(XlsxJsonTest, ConditionalFormatsCreateRejectsBadSqref) {
  doc->workbook().addWorksheet("CF");
  auto ws = doc->workbook().worksheet("CF");
  HybridXlsxConditionalFormats formats(ws.conditionalFormats());

  EXPECT_TRUE(throwsXlsxError([&] { (void)formats.create(""); },
                              xlsx_error::INVALID_ARGUMENT, "Invalid sqref"));

  // A valid sqref still works and round-trips
  auto fmt = formats.create("A1:B2");
  ASSERT_NE(fmt, nullptr);
  EXPECT_EQ(fmt->getSqref(), "A1:B2");
}

TEST_F(XlsxJsonTest, GetRuleFormulaOutOfRangeThrows) {
  doc->workbook().addWorksheet("CF");
  auto ws = doc->workbook().worksheet("CF");
  HybridXlsxConditionalFormats formats(ws.conditionalFormats());
  auto fmt = formats.create("A1:A5");
  ASSERT_NE(fmt, nullptr);
  fmt->createRule();

  // Out-of-range rule index must throw like every other getter,
  // even when formulaIndex != 0 (which otherwise returns "").
  EXPECT_TRUE(throwsXlsxError([&] { (void)fmt->getRuleFormula(5, 0); },
                              xlsx_error::INDEX_OUT_OF_RANGE));
  EXPECT_TRUE(throwsXlsxError([&] { (void)fmt->getRuleFormula(5, 1); },
                              xlsx_error::INDEX_OUT_OF_RANGE));

  // In-range behavior unchanged: secondary formula index returns ""
  EXPECT_EQ(fmt->getRuleFormula(0, 1), "");
  fmt->setRuleFormula(0, 0, "A1>1");
  EXPECT_EQ(fmt->getRuleFormula(0, 0), "A1>1");
}
