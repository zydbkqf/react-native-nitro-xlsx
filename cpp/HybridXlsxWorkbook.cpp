#include "HybridXlsxWorkbook.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <iomanip>

#ifdef __ANDROID__
#include <android/api-level.h>
#include <sys/stat.h>
#endif

namespace {

void writeJsonString(std::ostringstream& out, const std::string& s) {
  out << '"';
  for (char c : s) {
    switch (c) {
      case '"':  out << "\\\""; break;
      case '\\': out << "\\\\"; break;
      case '\b': out << "\\b";  break;
      case '\f': out << "\\f";  break;
      case '\n': out << "\\n";  break;
      case '\r': out << "\\r";  break;
      case '\t': out << "\\t";  break;
      default:
        if (static_cast<unsigned char>(c) < 0x20) {
          out << "\\u" << std::hex << std::setw(4) << std::setfill('0')
              << static_cast<int>(static_cast<unsigned char>(c))
              << std::dec;
        } else {
          out << c;
        }
    }
  }
  out << '"';
}

} // anonymous namespace

namespace margelo::nitro::xlsx {

namespace {
  std::string getTempDir() {
#ifdef __ANDROID__
    // /tmp is typically not available in Android sandbox, use app's cache directory
      const char* cacheDir = std::getenv("TMPDIR");
    if (cacheDir != nullptr && cacheDir[0] != '\0') {
      return std::string(cacheDir);
    }
    // Try common Android cache directories
    const char* candidates[] = {
      "/data/local/tmp",
      "/data/data/tmp",
      "/sdcard/tmp",
      "/tmp"
    };
    for (const char* dir : candidates) {
      struct stat st;
      if (stat(dir, &st) == 0 && S_ISDIR(st.st_mode)) {
        return std::string(dir);
      }
    }
    return "/data/local/tmp";
#else
    return "/tmp";
#endif
  }
}

HybridXlsxWorkbook::HybridXlsxWorkbook(const std::optional<std::string>& tempDir)
    : HybridObject("XlsxWorkbook"), HybridXlsxWorkbookSpec(), _finalized(false) {
  _workbook = std::make_unique<OpenXLSX::XLDocument>();
  if (tempDir.has_value()) {
    _tempDir = *tempDir;
  } else {
    _tempDir = getTempDir();
  }
}

HybridXlsxWorkbook::~HybridXlsxWorkbook() {
}

void HybridXlsxWorkbook::openFromFile(const std::string& path) {
  _workbook->open(path);
  loadWorksheets();
}

void HybridXlsxWorkbook::openFromBuffer(const uint8_t* data, size_t size) {
  std::string tempPath = _tempDir + "/xlsx_open_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".xlsx";
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

std::shared_ptr<HybridXlsxWorksheetSpec> HybridXlsxWorkbook::addWorksheet(const std::optional<std::string>& name) {
  if (!_workbook->isOpen()) {
    std::string tempPath = _tempDir + "/xlsx_create_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".xlsx";
    _workbook->create(tempPath, OpenXLSX::XLForceOverwrite);
    std::remove(tempPath.c_str());
    loadWorksheets();
  }

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
  return static_cast<double>(_workbook->workbook().worksheetCount());
}

std::shared_ptr<HybridXlsxCellFormatSpec> HybridXlsxWorkbook::addCellFormat() {
  auto hybridFormat = std::make_shared<HybridXlsxCellFormat>();
  _cellFormats.push_back(hybridFormat);
  return hybridFormat;
}

std::shared_ptr<Promise<std::shared_ptr<ArrayBuffer>>> HybridXlsxWorkbook::getBuffer() {
  auto promise = Promise<std::shared_ptr<ArrayBuffer>>::create();

  if (_finalized) {
    promise->reject(std::make_exception_ptr(std::runtime_error("Workbook has already been finalized")));
    return promise;
  }

  try {
    std::string tempPath = _tempDir + "/xlsx_temp_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".xlsx";
    _workbook->saveAs(tempPath, OpenXLSX::XLForceOverwrite);
    
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

std::string HybridXlsxWorkbook::toJSON(const std::optional<std::vector<std::string>>& keys) {
  if (_worksheets.empty()) {
    return "[]";
  }
  auto& ws = _worksheets[0];
  auto lastRow = static_cast<unsigned int>(ws->getLastRow());
  auto lastCol = static_cast<unsigned int>(ws->getLastColumn());
  if (lastRow == 0 || lastCol == 0) {
    return "[]";
  }

  unsigned int dataStartRow = 1;
  std::vector<std::string> resolvedKeys;

  if (keys.has_value()) {
    resolvedKeys = *keys;
  } else {
    dataStartRow = 2;
    for (unsigned int c = 1; c <= lastCol; ++c) {
      auto val = ws->getCellValue(1, c);
      if (auto* s = std::get_if<std::string>(&val)) {
        resolvedKeys.push_back(*s);
      } else if (auto* d = std::get_if<double>(&val)) {
        resolvedKeys.push_back(std::to_string(static_cast<int>(*d)));
      } else {
        resolvedKeys.push_back("col" + std::to_string(c));
      }
    }
  }

  if (resolvedKeys.empty()) {
    return "[]";
  }

  std::ostringstream out;
  out << "[";

  bool firstRow = true;

  for (unsigned int r = dataStartRow; r <= lastRow; ++r) {
    bool hasData = false;
    for (unsigned int c = 1; c <= std::min(static_cast<unsigned int>(resolvedKeys.size()), lastCol); ++c) {
      auto cellType = ws->getCellType(r, c);
      if (cellType != CellType::EMPTY) {
        hasData = true;
        break;
      }
    }
    if (!hasData) continue;

    if (!firstRow) out << ",";
    firstRow = false;

    out << "{";
    unsigned int keyCount = std::min(static_cast<unsigned int>(resolvedKeys.size()), lastCol);
    for (unsigned int c = 0; c < keyCount; ++c) {
      if (c > 0) out << ",";
      writeJsonString(out, resolvedKeys[c]);
      out << ":";
      auto val = ws->getCellValue(r, c + 1);
      if (auto* s = std::get_if<std::string>(&val)) {
        writeJsonString(out, *s);
      } else if (auto* d = std::get_if<double>(&val)) {
        if (std::isnan(*d) || std::isinf(*d)) {
          out << "null";
        } else {
          out << *d;
        }
      } else if (auto* b = std::get_if<bool>(&val)) {
        out << (*b ? "true" : "false");
      } else {
        out << "null";
      }
    }
    out << "}";
  }

  out << "]";
  return out.str();
}

}