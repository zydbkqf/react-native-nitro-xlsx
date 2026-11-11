require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))

Pod::Spec.new do |s|
  s.name         = "NitroXlsx"
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platforms    = { :ios => "15.1" }
  s.source       = { :git => "https://github.com/zydbkqf/react-native-nitro-xlsx.git", :tag => "#{s.version}" }

  # OpenXLSX, pugixml and miniz are vendored under thirdparty/ and shipped
  # with the package, so no network access is needed during pod install.
  s.exclude_files = [
    "cpp/tests/**/*",
    "thirdparty/miniz/tests/**/*",
    "thirdparty/miniz/examples/**/*",
    "thirdparty/pugixml/tests/**/*",
    "thirdparty/OpenXLSX/Tests/**/*",
    "thirdparty/OpenXLSX/Examples/**/*",
    "thirdparty/OpenXLSX/Benchmarks/**/*",
  ]

  s.source_files = [
    "cpp/**/*.{h,hpp,cpp}",
    "ios/**/*.{h,hpp,mm}",
    "thirdparty/OpenXLSX/OpenXLSX/**/*.hpp",
    "thirdparty/OpenXLSX/OpenXLSX/**/*.cpp",
    "thirdparty/pugixml/src/**/*.hpp",
    "thirdparty/pugixml/src/**/*.cpp",
    "thirdparty/OpenXLSX/OpenXLSX/headers/detail/Zippy.hpp",
    "thirdparty/miniz/**/*.h",
    "thirdparty/miniz/**/*.c",
  ]

  s.private_header_files = [
    "thirdparty/OpenXLSX/**/*.hpp",
    "thirdparty/OpenXLSX/**/*.h",
  ]

  s.dependency "React"

  s.pod_target_xcconfig = {
    "CLANG_CXX_LANGUAGE_STANDARD" => "c++20",
    "SWIFT_OBJC_INTEROP_MODE" => "objcxx",
    "DEFINES_MODULE" => "YES",
    "SWIFT_INSTALL_OBJC_HEADER" => "NO",
    "OTHER_CFLAGS" => "$(inherited) -fmodules",
    "OTHER_CPLUSPLUSFLAGS" => "$(inherited) -fmodules",
    "HEADER_SEARCH_PATHS" => "$(inherited) ${PODS_TARGET_SRCROOT}/thirdparty/OpenXLSX ${PODS_TARGET_SRCROOT}/thirdparty/OpenXLSX/OpenXLSX ${PODS_TARGET_SRCROOT}/thirdparty/OpenXLSX/OpenXLSX/headers ${PODS_TARGET_SRCROOT}/thirdparty/pugixml/src ${PODS_TARGET_SRCROOT}/thirdparty/miniz",
  }

  s.user_target_xcconfig = {
    "OTHER_CFLAGS" => "$(inherited) -fmodules",
    "OTHER_CPLUSPLUSFLAGS" => "$(inherited) -fmodules",
    "HEADER_SEARCH_PATHS" => "$(inherited) ${PODS_TARGET_SRCROOT}/thirdparty/OpenXLSX ${PODS_TARGET_SRCROOT}/thirdparty/OpenXLSX/OpenXLSX ${PODS_TARGET_SRCROOT}/thirdparty/OpenXLSX/OpenXLSX/headers ${PODS_TARGET_SRCROOT}/thirdparty/pugixml/src ${PODS_TARGET_SRCROOT}/thirdparty/miniz",
  }

  # Swift Package Manager note: this podspec intentionally declares nothing for
  # SPM. React Native 0.88+ drives SwiftPM from `package.json` (`swiftpmConfig`)
  # and `npx react-native spm`, and the manifest lives in Package.swift. That
  # manifest is still a placeholder (it only compiles ios/XlsxCacheDir.mm) and
  # CocoaPods remains the build path for this library until the C++ targets,
  # the vendored thirdparty/ tree and the NitroModules dependency are wired
  # into it.

  load 'nitrogen/generated/ios/NitroXlsx+autolinking.rb'
  add_nitrogen_files(s)
end
