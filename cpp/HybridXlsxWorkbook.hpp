#pragma once

#include "HybridXlsxWorkbookSpec.hpp"
#include <OpenXLSX/XLDocument.hpp>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "HybridXlsxWorksheet.hpp"
#include "HybridXlsxCellFormat.hpp"

namespace margelo::nitro::xlsx {

class HybridXlsxWorkbook : public HybridXlsxWorkbookSpec {
public:
  HybridXlsxWorkbook();
  ~HybridXlsxWorkbook() override;

  void openFromFile(const std::string& path);
  void openFromBuffer(const uint8_t* data, size_t size);

  // Ensure the underlying XLDocument has been created/opened so the workbook is
  // usable even if it was never explicitly opened (fromJSON, createWorkbook).
  void ensureOpen();

  std::shared_ptr<HybridXlsxWorksheetSpec> addWorksheet(const std::optional<std::string>& name) override;
  std::shared_ptr<HybridXlsxWorksheetSpec> getWorksheet(double index) override;
  std::shared_ptr<HybridXlsxWorksheetSpec> getWorksheetByName(const std::string& name) override;
  std::shared_ptr<HybridXlsxWorksheetSpec> getOrAddWorksheet(const std::string& name) override;
  double getWorksheetCount() override;
  void deleteSheet(const std::string& name) override;
  void updateSheetName(const std::string& oldName, const std::string& newName) override;
  std::shared_ptr<HybridXlsxWorksheetSpec> clone(const std::string& existingName, const std::string& newName) override;
  std::shared_ptr<HybridXlsxCellFormatSpec> addCellFormat() override;
  std::shared_ptr<Promise<std::shared_ptr<ArrayBuffer>>> getBuffer() override;
  std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>> toJSON() override;

private:
  std::unique_ptr<OpenXLSX::XLDocument> _workbook;
  std::vector<std::shared_ptr<HybridXlsxWorksheet>> _worksheets;
  std::unordered_map<std::string, std::shared_ptr<HybridXlsxWorksheet>> _worksheetNames;
  std::vector<std::shared_ptr<HybridXlsxCellFormat>> _cellFormats;
  bool _finalized;
  // Identifies the current underlying XLDocument. Bumped every time the
  // document is (re)created or opened so per-document caches (cell styles) can
  // tell a recycled heap address apart from the document it was built for.
  uint64_t documentGeneration() const { return _generation; }

  std::string _tempDir;
  uint64_t _generation = 0;

  void loadWorksheets();
  std::string tempFilePath(const char* prefix);
};

}