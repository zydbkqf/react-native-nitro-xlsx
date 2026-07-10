#pragma once

#include "HybridNitroXlsxSpec.hpp"
#include <memory>
#include "HybridXlsxWorkbook.hpp"

namespace margelo::nitro::xlsx {

class HybridNitroXlsx : public HybridNitroXlsxSpec {
public:
  HybridNitroXlsx();
  ~HybridNitroXlsx() override;

  std::shared_ptr<HybridXlsxWorkbookSpec> createWorkbook() override;
};

}