// Android / desktop implementation of XlsxCacheDir.hpp.
// (Apple is handled by ios/XlsxCacheDir.mm.)

#ifndef __APPLE__

#include "XlsxCacheDir.hpp"

#include <cstdlib>
#include <dirent.h>
#include <mutex>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __ANDROID__
#include <jni.h>
#include <fbjni/fbjni.h>
#endif

namespace margelo::nitro::xlsx {

namespace {
  std::string g_cacheDir;
  std::mutex g_cacheDirMutex;

  bool isUsableDir(const std::string& path) {
    if (path.empty()) return false;
    struct stat st {};
    if (stat(path.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) return false;
    // Must be writable (and traversable) for temp XLSX files to be created.
    if (access(path.c_str(), W_OK | X_OK) != 0) return false;
    DIR* dir = opendir(path.c_str());
    if (dir == nullptr) return false;
    closedir(dir);
    return true;
  }
}

void setCacheDir(const std::string& dir) {
  std::lock_guard<std::mutex> lock(g_cacheDirMutex);
  g_cacheDir = dir;
}

std::string getCacheDir() {
  {
    std::lock_guard<std::mutex> lock(g_cacheDirMutex);
    if (!g_cacheDir.empty()) return g_cacheDir;
  }
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

  // Fallback candidates, first usable one wins:
  //   1. $TMPDIR
  //   2. /tmp
  //   3. /data/local/tmp (writable shell dir on Android)
  // /tmp is returned unvalidated only as a last resort when none of the
  // candidates is usable, since e.g. Android may not provide /tmp at all.
  const char* tmp = std::getenv("TMPDIR");
  if (tmp != nullptr && isUsableDir(tmp)) {
    return std::string(tmp);
  }
  for (const char* candidate : {"/tmp", "/data/local/tmp"}) {
    if (isUsableDir(candidate)) return std::string(candidate);
  }
  return "/tmp";
}

} // namespace detail
} // namespace margelo::nitro::xlsx

#endif // !__APPLE__
