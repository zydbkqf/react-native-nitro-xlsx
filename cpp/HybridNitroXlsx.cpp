#include "HybridNitroXlsx.hpp"
#include <fstream>

namespace margelo::nitro::xlsx {

HybridNitroXlsx::HybridNitroXlsx()
    : HybridObject("NitroXlsx"), HybridNitroXlsxSpec() {
}

HybridNitroXlsx::~HybridNitroXlsx() {
}

std::shared_ptr<HybridXlsxWorkbookSpec> HybridNitroXlsx::createWorkbook(const std::optional<std::string>& tempDir) {
  return std::make_shared<HybridXlsxWorkbook>(tempDir);
}

std::shared_ptr<HybridXlsxWorkbookSpec> HybridNitroXlsx::openWorkbook(const std::string& path) {
  auto workbook = std::make_shared<HybridXlsxWorkbook>();
  workbook->openFromFile(path);
  return workbook;
}

std::shared_ptr<HybridXlsxWorkbookSpec> HybridNitroXlsx::openWorkbookFromBuffer(const std::shared_ptr<ArrayBuffer>& buffer) {
  auto workbook = std::make_shared<HybridXlsxWorkbook>();
  workbook->openFromBuffer(buffer->data(), buffer->size());
  return workbook;
}

}