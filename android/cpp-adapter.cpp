#include <jni.h>
#include <fbjni/fbjni.h>
#include "NitroXlsxOnLoad.hpp"
#include "XlsxCacheDir.hpp"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
  return facebook::jni::initialize(vm, []() {
    margelo::nitro::xlsx::registerAllNatives();
  });
}

// Called from NitroXlsxPackage with Context.getCacheDir().absolutePath
extern "C" JNIEXPORT void JNICALL
Java_com_margelo_nitro_xlsx_NitroXlsxPackage_nativeSetCacheDir(JNIEnv* env, jclass, jstring dir) {
  if (dir == nullptr) return;
  const char* chars = env->GetStringUTFChars(dir, nullptr);
  if (chars != nullptr) {
    margelo::nitro::xlsx::setCacheDir(std::string(chars));
    env->ReleaseStringUTFChars(dir, chars);
  }
}
