#pragma once

#include "HybridNitroXlsxSpec.hpp"
#include <memory>
#include "HybridXlsxWorkbook.hpp"

namespace margelo::nitro::xlsx {

class HybridNitroXlsx : public HybridNitroXlsxSpec {
public:
  HybridNitroXlsx();
  ~HybridNitroXlsx() override;

  std::shared_ptr<HybridXlsxWorkbookSpec> createWorkbook(const std::optional<std::string>& tempDir = std::nullopt) override;
  std::shared_ptr<HybridXlsxWorkbookSpec> openWorkbook(const std::string& path) override;
  std::shared_ptr<HybridXlsxWorkbookSpec> openWorkbookFromBuffer(const std::shared_ptr<ArrayBuffer>& buffer) override;
};

}