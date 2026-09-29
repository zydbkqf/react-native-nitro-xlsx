// Android / desktop implementation of XlsxCacheDir.hpp.
// (Apple is handled by ios/XlsxCacheDir.mm.)

#ifndef __APPLE__

#include "XlsxCacheDir.hpp"

#include <cstdlib>
#include <sys/stat.h>

#ifdef __ANDROID__
#include <jni.h>
#include <fbjni/fbjni.h>
#endif

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
#ifdef __ANDROID__
  // Ask the Android runtime for Context.getCacheDir()
  using namespace facebook::jni;
  try {
    auto activityThread = findClassStatic("android/app/ActivityThread");
    auto currentApplication = activityThread->getStaticMethod<JObject()>("currentApplication");
    // JStaticMethod::operator() takes (alias_ref<jclass>, args...)
    auto application = currentApplication(activityThread);
    if (application) {
      auto contextClass = findClassStatic("android/content/Context");
      auto getCacheDir = contextClass->getMethod<JObject()>("getCacheDir");
      // JMethod::operator() takes (alias_ref<jobject>, args...)
      auto cacheDir = getCacheDir(application);
      if (cacheDir) {
        auto fileClass = findClassStatic("java/io/File");
        auto getAbsolutePath = fileClass->getMethod<JString()>("getAbsolutePath");
        auto path = getAbsolutePath(cacheDir);
        std::string result = path->toStdString();
        if (isUsableDir(result)) return result;
      }
    }
  } catch (...) {
    // fall through to env / default
  }
#endif

  const char* tmp = std::getenv("TMPDIR");
  if (tmp != nullptr && isUsableDir(tmp)) {
    return std::string(tmp);
  }
  return "/tmp";
}

} // namespace detail
} // namespace margelo::nitro::xlsx

#endif // !__APPLE__
