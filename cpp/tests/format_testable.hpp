#pragma once

// Test shim: the real implementation lives in cpp/XlsxCellStyle.hpp (production
// code). Tests keep the historical `test::TestableFormat` name via alias.
#include "../XlsxCellStyle.hpp"

namespace margelo::nitro::xlsx::test {
using TestableFormat = XlsxCellStyle;
} // namespace margelo::nitro::xlsx::test
