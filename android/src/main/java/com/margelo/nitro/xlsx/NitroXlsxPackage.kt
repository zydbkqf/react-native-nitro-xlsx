package com.margelo.nitro.xlsx

import com.facebook.react.ReactPackage
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.NativeModule
import com.facebook.react.uimanager.ViewManager

class NitroXlsxPackage : ReactPackage {
  companion object {
    init {
      NitroXlsxOnLoad.initializeNative()
    }

    /** Pass the app cache directory to the native layer for temp XLSX files. */
    @JvmStatic
    private external fun nativeSetCacheDir(dir: String)
  }

  @Suppress("OVERRIDE_DEPRECATION")
  override fun createNativeModules(reactContext: ReactApplicationContext): List<NativeModule> {
    try {
      nativeSetCacheDir(reactContext.cacheDir.absolutePath)
    } catch (_: Throwable) {
      // native resolver falls back to Context.getCacheDir() via JNI
    }
    return emptyList()
  }

  override fun createViewManagers(reactContext: ReactApplicationContext): List<ViewManager<*, *>> {
    return emptyList()
  }
}
