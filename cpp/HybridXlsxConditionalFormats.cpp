#include "HybridXlsxConditionalFormats.hpp"
#include "XlsxError.hpp"

namespace margelo::nitro::xlsx {

HybridXlsxConditionalFormats::HybridXlsxConditionalFormats(OpenXLSX::XLConditionalFormats formats)
    : HybridObject("XlsxConditionalFormats"), HybridXlsxConditionalFormatsSpec(), _formats(std::move(formats)) {
}

HybridXlsxConditionalFormats::~HybridXlsxConditionalFormats() = default;

double HybridXlsxConditionalFormats::getCount() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(_formats.count());
  });
}

std::shared_ptr<HybridXlsxConditionalFormatSpec> HybridXlsxConditionalFormats::create(const std::string& sqref) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    size_t index = _formats.create();
    OpenXLSX::XLConditionalFormat format = _formats.conditionalFormatByIndex(index);
    format.setSqref(sqref);
    auto hybrid = std::make_shared<HybridXlsxConditionalFormat>(format);
    _created.push_back(hybrid);
    return std::static_pointer_cast<HybridXlsxConditionalFormatSpec>(hybrid);
  });
}

std::shared_ptr<HybridXlsxConditionalFormatSpec> HybridXlsxConditionalFormats::getByIndex(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    size_t idx = static_cast<size_t>(index);
    if (idx >= _formats.count()) {
      throw XlsxError(xlsx_error::INDEX_OUT_OF_RANGE,
                      "conditionalFormat index " + std::to_string(idx) + " out of range (count=" + std::to_string(_formats.count()) + ")");
    }
    auto hybrid = std::make_shared<HybridXlsxConditionalFormat>(_formats.conditionalFormatByIndex(idx));
    _created.push_back(hybrid);
    return std::static_pointer_cast<HybridXlsxConditionalFormatSpec>(hybrid);
  });
}

std::string HybridXlsxConditionalFormats::summary() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _formats.summary(); });
}

} // namespace margelo::nitro::xlsx
