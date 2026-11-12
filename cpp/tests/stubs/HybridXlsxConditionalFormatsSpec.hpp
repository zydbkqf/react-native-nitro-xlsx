#pragma once

// Test-build stand-in for nitrogen's HybridXlsxConditionalFormatsSpec.hpp.
// Mirrors the pure-virtual interface (signatures must stay in sync) without the
// Nitro/JSI hybrid-registration machinery.

#include <NitroModules/HybridObject.hpp>
#include <memory>
#include <string>

namespace margelo::nitro::xlsx {

  class HybridXlsxConditionalFormatSpec;

  using namespace margelo::nitro;

  class HybridXlsxConditionalFormatsSpec: public virtual HybridObject {
    public:
      explicit HybridXlsxConditionalFormatsSpec(): HybridObject(TAG) { }
      ~HybridXlsxConditionalFormatsSpec() override = default;

    public:
      virtual double getCount() = 0;
      virtual std::shared_ptr<HybridXlsxConditionalFormatSpec> create(const std::string& sqref) = 0;
      virtual std::shared_ptr<HybridXlsxConditionalFormatSpec> getByIndex(double index) = 0;
      virtual std::string summary() = 0;

    protected:
      void loadHybridMethods() override {}

    protected:
      static constexpr auto TAG = "XlsxConditionalFormats";
  };

} // namespace margelo::nitro::xlsx
