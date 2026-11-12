#pragma once

// Test-build stand-in for nitrogen's HybridXlsxWorkbookSpec.hpp.
// Mirrors the pure-virtual interface (signatures must stay in sync) without the
// Nitro/JSI hybrid-registration machinery.

#include <NitroModules/HybridObject.hpp>
#include <NitroModules/ArrayBuffer.hpp>
#include <NitroModules/Promise.hpp>
#include <NitroModules/AnyMap.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace margelo::nitro::xlsx {

  class HybridXlsxWorksheetSpec;
  class HybridXlsxCellFormatSpec;

  using namespace margelo::nitro;

  class HybridXlsxWorkbookSpec: public virtual HybridObject {
    public:
      explicit HybridXlsxWorkbookSpec(): HybridObject(TAG) { }
      ~HybridXlsxWorkbookSpec() override = default;

    public:
      virtual std::shared_ptr<HybridXlsxWorksheetSpec> addWorksheet(const std::optional<std::string>& name) = 0;
      virtual std::shared_ptr<HybridXlsxWorksheetSpec> getWorksheet(double index) = 0;
      virtual std::shared_ptr<HybridXlsxWorksheetSpec> getWorksheetByName(const std::string& name) = 0;
      virtual std::shared_ptr<HybridXlsxWorksheetSpec> getOrAddWorksheet(const std::string& name) = 0;
      virtual double getWorksheetCount() = 0;
      virtual void deleteSheet(const std::string& name) = 0;
      virtual void updateSheetName(const std::string& oldName, const std::string& newName) = 0;
      virtual std::shared_ptr<HybridXlsxWorksheetSpec> clone(const std::string& existingName, const std::string& newName) = 0;
      virtual std::shared_ptr<HybridXlsxCellFormatSpec> addCellFormat() = 0;
      virtual std::shared_ptr<Promise<std::shared_ptr<ArrayBuffer>>> getBuffer() = 0;
      virtual std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>> toJSON() = 0;

    protected:
      void loadHybridMethods() override {}

    protected:
      static constexpr auto TAG = "XlsxWorkbook";
  };

} // namespace margelo::nitro::xlsx
