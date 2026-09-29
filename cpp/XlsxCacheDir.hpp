#pragma once

#include <string>

namespace margelo::nitro::xlsx {

/**
 * Override the cache directory used for temporary XLSX files.
 * Normally called once from the Android native side at startup;
 * if never called, getCacheDir() falls back to the platform resolver.
 */
void setCacheDir(const std::string& dir);

/**
 * Directory used for temporary XLSX files.
 * Priority: explicit setCacheDir() > platform cache dir > TMPDIR > /tmp
 *
 * Implemented in ios/XlsxCacheDir.mm (Apple) and cpp/XlsxCacheDir.cpp (other).
 */
std::string getCacheDir();

namespace detail {
  /** Platform-specific cache directory (NSCachesDirectory / Context.getCacheDir). */
  std::string resolvePlatformCacheDir();
}

} // namespace margelo::nitro::xlsx
