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
    if (data.empty()) return workbook;

    for (const auto& [sheetName, rows] : data) {
      auto sheet = workbook->addWorksheet(sheetName);
      sheet->fromJSON(rows);
    }
    return workbook;
  });
}

}
