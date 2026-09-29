#pragma once

#include "HybridXlsxConditionalFormatsSpec.hpp"
#include "HybridXlsxConditionalFormat.hpp"
#include <OpenXLSX/XLSheet.hpp>
#include <memory>
#include <string>
#include <vector>

namespace margelo::nitro::xlsx {

class HybridXlsxConditionalFormats : public HybridXlsxConditionalFormatsSpec {
public:
  explicit HybridXlsxConditionalFormats(OpenXLSX::XLConditionalFormats formats);
  ~HybridXlsxConditionalFormats() override;

  double getCount() override;
  std::shared_ptr<HybridXlsxConditionalFormatSpec> create(const std::string& sqref) override;
  std::shared_ptr<HybridXlsxConditionalFormatSpec> getByIndex(double index) override;
  std::string summary() override;

private:
  OpenXLSX::XLConditionalFormats _formats;
  // Keep hybrid wrappers alive so JS holds stay valid
  std::vector<std::shared_ptr<HybridXlsxConditionalFormat>> _created;
};

} // namespace margelo::nitro::xlsx
