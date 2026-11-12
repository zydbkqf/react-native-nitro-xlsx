#pragma once

// Test-build stand-in for <NitroModules/HybridObject.hpp>. The real HybridObject
// pulls in JSI / React Native, which the gtest build does not have. Only the
// pieces the nitrogen Spec stubs and the HybridXlsxConditionalFormat(s)
// constructors touch are provided.

namespace margelo::nitro {

class HybridObject {
public:
  explicit HybridObject(const char* /*name*/) {}
  virtual ~HybridObject() = default;
  virtual void loadHybridMethods() {}
};

} // namespace margelo::nitro
