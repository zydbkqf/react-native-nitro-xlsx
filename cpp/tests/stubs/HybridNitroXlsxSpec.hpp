#pragma once

// Test-build stand-in for nitrogen's HybridNitroXlsxSpec.hpp.
// Mirrors the pure-virtual interface (signatures must stay in sync) without the
// Nitro/JSI hybrid-registration machinery.

#include <NitroModules/HybridObject.hpp>
#include <NitroModules/ArrayBuffer.hpp>
#include <NitroModules/AnyMap.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace margelo::nitro::xlsx {

  class HybridXlsxWorkbookSpec;

  using namespace margelo::nitro;

  class HybridNitroXlsxSpec: public virtual HybridObject {
    public:
      explicit HybridNitroXlsxSpec(): HybridObject(TAG) { }
      ~HybridNitroXlsxSpec() override = default;

    public:
      virtual std::shared_ptr<HybridXlsxWorkbookSpec> createWorkbook() = 0;
      virtual std::shared_ptr<HybridXlsxWorkbookSpec> openWorkbook(const std::string& path) = 0;
      virtual std::shared_ptr<HybridXlsxWorkbookSpec> openWorkbookFromBuffer(const std::shared_ptr<ArrayBuffer>& buffer) = 0;
      virtual std::shared_ptr<HybridXlsxWorkbookSpec> fromJSON(const std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>>& data) = 0;

    protected:
      void loadHybridMethods() override {}

    protected:
      static constexpr auto TAG = "NitroXlsx";
  };

} // namespace margelo::nitro::xlsx
