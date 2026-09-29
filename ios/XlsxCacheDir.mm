// Apple (iOS / macOS) cache directory via Foundation.
// Full implementation of XlsxCacheDir.hpp for __APPLE__.

#ifdef __APPLE__

#include "XlsxCacheDir.hpp"

#include <cstdlib>
#include <sys/stat.h>

#import <Foundation/Foundation.h>

namespace margelo::nitro::xlsx {

namespace {
  std::string g_cacheDir;

  bool isUsableDir(const std::string& path) {
    if (path.empty()) return false;
    struct stat st {};
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
  }
}

void setCacheDir(const std::string& dir) {
  g_cacheDir = dir;
}

std::string getCacheDir() {
  if (!g_cacheDir.empty()) return g_cacheDir;
  return detail::resolvePlatformCacheDir();
}

namespace detail {

std::string resolvePlatformCacheDir() {
  @autoreleasepool {
    NSArray<NSString*>* caches =
        NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    if (caches.count > 0) {
      std::string result([caches.firstObject UTF8String]);
      if (isUsableDir(result)) return result;
    }
    NSString* tmp = NSTemporaryDirectory();
    if (tmp != nil) {
      std::string result([tmp UTF8String]);
      if (isUsableDir(result)) return result;
    }
  }
  const char* env = std::getenv("TMPDIR");
  if (env != nullptr && isUsableDir(env)) {
    return std::string(env);
  }
  return "/tmp";
}

} // namespace detail
} // namespace margelo::nitro::xlsx

#endif // __APPLE__
