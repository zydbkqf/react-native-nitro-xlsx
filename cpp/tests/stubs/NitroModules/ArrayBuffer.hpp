#pragma once

// Test-build stand-in for <NitroModules/ArrayBuffer.hpp>: a simple owning byte
// buffer without JSI. Only the pieces HybridXlsxWorkbook/HybridNitroXlsx touch
// are provided (copy/data/size).

#include <cstdint>
#include <memory>
#include <vector>

namespace margelo::nitro {

class ArrayBuffer {
public:
  static std::shared_ptr<ArrayBuffer> copy(const uint8_t* data, size_t size) {
    auto buffer = std::shared_ptr<ArrayBuffer>(new ArrayBuffer());
    buffer->_bytes.assign(data, data + size);
    return buffer;
  }

  uint8_t* data() { return _bytes.data(); }
  const uint8_t* data() const { return _bytes.data(); }
  size_t size() const { return _bytes.size(); }

private:
  ArrayBuffer() = default;
  std::vector<uint8_t> _bytes;
};

} // namespace margelo::nitro
