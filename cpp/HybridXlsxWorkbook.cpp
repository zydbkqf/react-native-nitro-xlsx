#include "HybridXlsxWorkbook.hpp"
#include "XlsxError.hpp"
#include "XlsxJson.hpp"
#include "XlsxCacheDir.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <chrono>

#ifdef __ANDROID__
#include <android/api-level.h>
#include <sys/stat.h>
#endif

namespace margelo::nitro::xlsx {

HybridXlsxWorkbook::HybridXlsxWorkbook()
    : HybridObject("XlsxWorkbook"), HybridXlsxWorkbookSpec(), _finalized(false) {
  _workbook = std::make_unique<OpenXLSX::XLDocument>();
  _tempDir = getCacheDir();
}

std::string HybridXlsxWorkbook::tempFilePath(const char* prefix) {
  return _tempDir + "/" + prefix + "_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".xlsx";
}

HybridXlsxWorkbook::~HybridXlsxWorkbook() {
}

void HybridXlsxWorkbook::openFromFile(const std::string& path) {
  _workbook->open(path);
  loadWorksheets();
}

void HybridXlsxWorkbook::openFromBuffer(const uint8_t* data, size_t size) {
  std::string tempPath = tempFilePath("xlsx_open");
  std::ofstream file(tempPath, std::ios::binary);
  file.write(reinterpret_cast<const char*>(data), size);
  file.close();

  _workbook->open(tempPath);
  std::remove(tempPath.c_str());
  loadWorksheets();
}

void HybridXlsxWorkbook::loadWorksheets() {
  _worksheets.clear();
  _worksheetNames.clear();

  auto wb = _workbook->workbook();
  auto count = wb.worksheetCount();
  for (uint16_t i = 1; i <= count; ++i) {
    auto worksheet = wb.worksheet(i);
    std::string sheetName = worksheet.name();
    auto hybridWorksheet = std::make_shared<HybridXlsxWorksheet>(*_workbook, worksheet);
    _worksheets.push_back(hybridWorksheet);
    _worksheetNames[sheetName] = hybridWorksheet;
  }
}

void HybridXlsxWorkbook::ensureOpen() {
  if (_workbook->isOpen()) return;
  std::string tempPath = tempFilePath("xlsx_create");
  _workbook->create(tempPath, OpenXLSX::XLForceOverwrite);
  std::remove(tempPath.c_str());
  loadWorksheets();
}

std::shared_ptr<HybridXlsxWorksheetSpec> HybridXlsxWorkbook::addWorksheet(const std::optional<std::string>& name) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&]() -> std::shared_ptr<HybridXlsxWorksheetSpec> {
    ensureOpen();

    std::string sheetName;
    if (name.has_value()) {
      sheetName = *name;
      auto it = _worksheetNames.find(sheetName);
      if (it != _worksheetNames.end()) {
        // Worksheet already exists (e.g., default Sheet1 created by OpenXLSX), return it
        return it->second;
      }
    } else {
      int idx = 1;
      do {
        sheetName = "Sheet" + std::to_string(idx++);
      } while (_worksheetNames.count(sheetName) > 0);
    }

    _workbook->workbook().addWorksheet(sheetName);
    auto worksheet = _workbook->workbook().worksheet(sheetName);
    auto hybridWorksheet = std::make_shared<HybridXlsxWorksheet>(*_workbook, worksheet);
    _worksheets.push_back(hybridWorksheet);
    _worksheetNames[sheetName] = hybridWorksheet;
    return hybridWorksheet;
  });
}

std::shared_ptr<HybridXlsxWorksheetSpec> HybridXlsxWorkbook::getWorksheet(double index) {
  return withXlsxError(xlsx_error::INDEX_OUT_OF_RANGE, [&]() -> std::shared_ptr<HybridXlsxWorksheetSpec> {
    ensureOpen();
    size_t idx = static_cast<size_t>(index);
    if (idx >= _worksheets.size()) {
      throw XlsxError(xlsx_error::INDEX_OUT_OF_RANGE, "Worksheet index out of range: " + std::to_string(idx));
    }
    return _worksheets[idx];
  });
}

std::shared_ptr<HybridXlsxWorksheetSpec> HybridXlsxWorkbook::getWorksheetByName(const std::string& name) {
  return withXlsxError(xlsx_error::SHEET_NOT_FOUND, [&]() -> std::shared_ptr<HybridXlsxWorksheetSpec> {
    ensureOpen();
    auto it = _worksheetNames.find(name);
    if (it == _worksheetNames.end()) {
      throw XlsxError(xlsx_error::SHEET_NOT_FOUND, "Worksheet not found: " + name);
    }
    return it->second;
  });
}

std::shared_ptr<HybridXlsxWorksheetSpec> HybridXlsxWorkbook::getOrAddWorksheet(const std::string& name) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&]() -> std::shared_ptr<HybridXlsxWorksheetSpec> {
    ensureOpen();
    auto it = _worksheetNames.find(name);
    if (it != _worksheetNames.end()) {
      return it->second;
    }
    return addWorksheet(name);
  });
}

double HybridXlsxWorkbook::getWorksheetCount() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(_workbook->workbook().worksheetCount());
  });
}

void HybridXlsxWorkbook::deleteSheet(const std::string& name) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    ensureOpen();
    auto it = _worksheetNames.find(name);
    if (it == _worksheetNames.end()) {
      throw XlsxError(xlsx_error::SHEET_NOT_FOUND, "Worksheet not found: " + name);
    }
    if (_worksheetNames.size() <= 1) {
      throw XlsxError(xlsx_error::INVALID_ARGUMENT, "Cannot delete the last worksheet in a workbook");
    }

    _workbook->workbook().deleteSheet(name);

    // Drop the cached hybrid wrappers for the deleted sheet
    for (auto wit = _worksheets.begin(); wit != _worksheets.end(); ++wit) {
      if (*wit == it->second) {
        _worksheets.erase(wit);
        break;
      }
    }
    _worksheetNames.erase(it);
  });
}

void HybridXlsxWorkbook::updateSheetName(const std::string& oldName, const std::string& newName) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    ensureOpen();
    auto it = _worksheetNames.find(oldName);
    if (it == _worksheetNames.end()) {
      throw XlsxError(xlsx_error::SHEET_NOT_FOUND, "Worksheet not found: " + oldName);
    }
    if (_worksheetNames.count(newName) > 0) {
      throw XlsxError(xlsx_error::SHEET_EXISTS, "Worksheet already exists: " + newName);
    }

    // Rename the sheet itself
    it->second->applyRename(oldName, newName);

    // Rewrite formula references on every worksheet (formulas in other sheets may reference oldName)
    for (auto& [name, hybrid] : _worksheetNames) {
      if (name == oldName) continue;
      hybrid->rewriteSheetNameRef(oldName, newName);
    }

    // Refresh the name map
    auto hybrid = it->second;
    _worksheetNames.erase(it);
    _worksheetNames[newName] = hybrid;
  });
}

std::shared_ptr<HybridXlsxWorksheetSpec> HybridXlsxWorkbook::clone(const std::string& existingName, const std::string& newName) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&]() -> std::shared_ptr<HybridXlsxWorksheetSpec> {
    ensureOpen();
    if (_worksheetNames.count(existingName) == 0) {
      throw XlsxError(xlsx_error::SHEET_NOT_FOUND, "Worksheet not found: " + existingName);
    }
    if (_worksheetNames.count(newName) > 0) {
      throw XlsxError(xlsx_error::SHEET_EXISTS, "Worksheet already exists: " + newName);
    }

    _workbook->workbook().cloneSheet(existingName, newName);
    auto worksheet = _workbook->workbook().worksheet(newName);
    auto hybridWorksheet = std::make_shared<HybridXlsxWorksheet>(*_workbook, worksheet);
    _worksheets.push_back(hybridWorksheet);
    _worksheetNames[newName] = hybridWorksheet;
    return hybridWorksheet;
  });
}

std::shared_ptr<HybridXlsxCellFormatSpec> HybridXlsxWorkbook::addCellFormat() {
  auto hybridFormat = std::make_shared<HybridXlsxCellFormat>();
  _cellFormats.push_back(hybridFormat);
  return hybridFormat;
}

std::shared_ptr<Promise<std::shared_ptr<ArrayBuffer>>> HybridXlsxWorkbook::getBuffer() {
  auto promise = Promise<std::shared_ptr<ArrayBuffer>>::create();

  if (_finalized) {
    promise->reject(std::make_exception_ptr(XlsxError(xlsx_error::WORKBOOK_CLOSED, "Workbook has already been finalized")));
    return promise;
  }

  try {
    std::string tempPath = tempFilePath("xlsx_temp");
    _workbook->saveAs(tempPath, OpenXLSX::XLForceOverwrite);

    std::ifstream file(tempPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
      promise->reject(std::make_exception_ptr(XlsxError(xlsx_error::IO_ERROR, "Failed to open temp file")));
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
  } catch (const XlsxError& e) {
    promise->reject(std::make_exception_ptr(e));
  } catch (const OpenXLSX::XLException& e) {
    promise->reject(std::make_exception_ptr(XlsxError(xlsx_error::XLSX_ERROR, e.what())));
  } catch (const std::exception& e) {
    promise->reject(std::make_exception_ptr(XlsxError(xlsx_error::INTERNAL_ERROR, e.what())));
  }

  return promise;
}

std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>> HybridXlsxWorkbook::toJSON() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    auto wb = _workbook->workbook();
    return workbookToRecords(wb);
  });
}

}