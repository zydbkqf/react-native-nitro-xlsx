#pragma once

// Test-build stand-in for nitrogen's CellType.hpp: just the enum, without the
// JSIConverter machinery. Values must stay in sync with the generated file.

namespace margelo::nitro::xlsx {

  enum class CellType {
    STRING  = 0,
    NUMBER  = 1,
    BOOLEAN = 2,
    EMPTY   = 3,
    DATE    = 4,
    ERROR   = 5,
    FORMULA = 6,
    BLANK   = 7,
  };

} // namespace margelo::nitro::xlsx
