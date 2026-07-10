#include "HybridXlsxWorkbook.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>

namespace margelo::nitro::xlsx {

HybridXlsxWorkbook::HybridXlsxWorkbook()
    : HybridObject("XlsxWorkbook"), HybridXlsxWorkbookSpec(), _finalized(false) {
  _workbook = std::make_unique<OpenXLSX::XLDocument>();
  _workbook->create();
}

HybridXlsxWorkbook::~HybridXlsxWorkbook() {
}

std::shared_ptr<HybridXlsxWorksheetSpec> HybridXlsxWorkbook::addWorksheet(const std::optional<std::string>& name) {
  std::string sheetName = name.has_value() ? *name : "";
  OpenXLSX::XLWorksheet worksheet = _workbook->addWorksheet(sheetName);
  auto hybridWorksheet = std::make_shared<HybridXlsxWorksheet>(*_workbook, worksheet);
  _worksheets.push_back(hybridWorksheet);
  if (name.has_value()) {
    _worksheetNames[*name] = hybridWorksheet;
  }
  return hybridWorksheet;
}

std::shared_ptr<HybridXlsxWorksheetSpec> HybridXlsxWorkbook::getWorksheet(double index) {
  size_t idx = static_cast<size_t>(index);
  if (idx >= _worksheets.size()) {
    throw std::runtime_error("Worksheet index out of range: " + std::to_string(idx));
  }
  return _worksheets[idx];
}

std::shared_ptr<HybridXlsxWorksheetSpec> HybridXlsxWorkbook::getWorksheetByName(const std::string& name) {
  auto it = _worksheetNames.find(name);
  if (it == _worksheetNames.end()) {
    throw std::runtime_error("Worksheet not found: " + name);
  }
  return it->second;
}

std::shared_ptr<HybridXlsxWorksheetSpec> HybridXlsxWorkbook::getOrAddWorksheet(const std::string& name) {
  auto it = _worksheetNames.find(name);
  if (it != _worksheetNames.end()) {
    return it->second;
  }
  return addWorksheet(name);
}

double HybridXlsxWorkbook::getWorksheetCount() {
  return static_cast<double>(_workbook->workbook()->worksheets().size());
}

std::shared_ptr<HybridXlsxFormatSpec> HybridXlsxWorkbook::addFormat() {
  auto hybridFormat = std::make_shared<HybridXlsxFormat>();
  _formats.push_back(hybridFormat);
  return hybridFormat;
}

std::shared_ptr<Promise<std::shared_ptr<ArrayBuffer>>> HybridXlsxWorkbook::getBuffer() {
  auto promise = Promise<std::shared_ptr<ArrayBuffer>>::create();

  if (_finalized) {
    promise->reject(std::make_exception_ptr(std::runtime_error("Workbook has already been finalized")));
    return promise;
  }

  try {
    std::string tempPath = "/tmp/xlsx_temp_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".xlsx";
    _workbook->saveAs(tempPath);
    
    std::ifstream file(tempPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
      promise->reject(std::make_exception_ptr(std::runtime_error("Failed to open temp file")));
      return promise;
    }
    
    std::streampos size = file.tellg();
    std::vector<uint8_t> buffer(size);
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    file.close();
    
    std::remove(tempPath.c_str());
    
    auto arrayBuffer = ArrayBuffer::copy(buffer.data(), buffer.size());
    _finalized = true;
    
    promise->resolve(arrayBuffer);
  } catch (const std::exception& e) {
    promise->reject(std::make_exception_ptr(e));
  }
  
  return promise;
}

}