#include "HybridNitroXlsx.hpp"
#include "XlsxError.hpp"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>

namespace margelo::nitro::xlsx {

HybridNitroXlsx::HybridNitroXlsx()
    : HybridObject("NitroXlsx"), HybridNitroXlsxSpec() {
}

HybridNitroXlsx::~HybridNitroXlsx() {
}

std::shared_ptr<HybridXlsxWorkbookSpec> HybridNitroXlsx::createWorkbook() {
  return withXlsxError(xlsx_error::IO_ERROR, [&]() -> std::shared_ptr<HybridXlsxWorkbookSpec> {
    return std::make_shared<HybridXlsxWorkbook>();
  });
}

std::shared_ptr<HybridXlsxWorkbookSpec> HybridNitroXlsx::openWorkbook(const std::string& path) {
  return withXlsxError(xlsx_error::IO_ERROR, [&]() -> std::shared_ptr<HybridXlsxWorkbookSpec> {
    auto workbook = std::make_shared<HybridXlsxWorkbook>();
    workbook->openFromFile(path);
    return workbook;
  });
}

std::shared_ptr<HybridXlsxWorkbookSpec> HybridNitroXlsx::openWorkbookFromBuffer(const std::shared_ptr<ArrayBuffer>& buffer) {
  return withXlsxError(xlsx_error::IO_ERROR, [&]() -> std::shared_ptr<HybridXlsxWorkbookSpec> {
    auto workbook = std::make_shared<HybridXlsxWorkbook>();
    workbook->openFromBuffer(buffer->data(), buffer->size());
    return workbook;
  });
}

std::shared_ptr<HybridXlsxWorkbookSpec> HybridNitroXlsx::fromJSON(const std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>>& data) {
  return withXlsxError(xlsx_error::INVALID_ARGUMENT, [&]() -> std::shared_ptr<HybridXlsxWorkbookSpec> {
    auto workbook = std::make_shared<HybridXlsxWorkbook>();
    // Always leave the workbook usable: create the underlying XLDocument even
    // when there is no data, so getBuffer()/toJSON()/getWorksheetCount() work.
    workbook->ensureOpen();

    // Snapshot the sheet(s) OpenXLSX created by default (e.g. "Sheet1") so they
    // can be removed once the key sheets exist (deleteSheet refuses to delete
    // the last sheet, so key sheets must be added first).
    std::vector<std::string> defaultNames;
    {
      double count = workbook->getWorksheetCount();
      for (double i = 0; i < count; ++i) {
        defaultNames.push_back(workbook->getWorksheet(i)->getName());
      }
    }

    for (const auto& [sheetName, rows] : data) {
      auto sheet = workbook->addWorksheet(sheetName);
      sheet->fromJSON(rows);
    }

    // README promises one worksheet per key: drop the default sheet(s) unless a
    // key claimed them. Skip when there is no data - a workbook must keep at
    // least one sheet.
    if (!data.empty()) {
      for (const auto& name : defaultNames) {
        if (data.count(name) == 0) {
          workbook->deleteSheet(name);
        }
      }
    }
    return workbook;
  });
}

}
