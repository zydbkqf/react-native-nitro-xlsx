#pragma once

#include "HybridXlsxWorkbookSpec.hpp"
#include <OpenXLSX/XLDocument.hpp>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "HybridXlsxWorksheet.hpp"
#include "HybridXlsxFormat.hpp"

namespace margelo::nitro::xlsx {

class HybridXlsxWorkbook : public HybridXlsxWorkbookSpec {
public:
  HybridXlsxWorkbook();
  ~HybridXlsxWorkbook() override;

  std::shared_ptr<HybridXlsxWorksheetSpec> addWorksheet(const std::optional<std::string>& name) override;
  std::shared_ptr<HybridXlsxWorksheetSpec> getWorksheet(double index) override;
  std::shared_ptr<HybridXlsxWorksheetSpec> getWorksheetByName(const std::string& name) override;
  std::shared_ptr<HybridXlsxWorksheetSpec> getOrAddWorksheet(const std::string& name) override;
  double getWorksheetCount() override;
  std::shared_ptr<HybridXlsxFormatSpec> addFormat() override;
  std::shared_ptr<Promise<std::shared_ptr<ArrayBuffer>>> getBuffer() override;

private:
  std::unique_ptr<OpenXLSX::XLDocument> _workbook;
  std::vector<std::shared_ptr<HybridXlsxWorksheet>> _worksheets;
  std::unordered_map<std::string, std::shared_ptr<HybridXlsxWorksheet>> _worksheetNames;
  std::vector<std::shared_ptr<HybridXlsxFormat>> _formats;
  bool _finalized;
};

}