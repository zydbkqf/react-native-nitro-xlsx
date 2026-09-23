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

  s.exclude_files = [
    "cpp/tests/**/*",
    "OpenXLSX/ThirdParty/miniz/tests/**/*",
    "OpenXLSX/ThirdParty/miniz/examples/**/*",
  ]

  s.source_files = [
    "cpp/**/*.{h,hpp,cpp}",
    "OpenXLSX/OpenXLSX/**/*.hpp",
    "OpenXLSX/OpenXLSX/**/*.cpp",
    "OpenXLSX/ThirdParty/pugixml/src/**/*.hpp",
    "OpenXLSX/ThirdParty/pugixml/src/**/*.cpp",
    "OpenXLSX/OpenXLSX/headers/detail/Zippy.hpp",
    "OpenXLSX/ThirdParty/miniz/**/*.h",
    "OpenXLSX/ThirdParty/miniz/**/*.c",
  ]

  s.private_header_files = [
    "OpenXLSX/**/*.hpp",
    "OpenXLSX/**/*.h",
  ]

  s.dependency "React"

  s.pod_target_xcconfig = {
    "CLANG_CXX_LANGUAGE_STANDARD" => "c++20",
    "SWIFT_OBJC_INTEROP_MODE" => "objcxx",
    "DEFINES_MODULE" => "YES",
    "SWIFT_INSTALL_OBJC_HEADER" => "NO",
    "OTHER_CFLAGS" => "$(inherited) -fmodules",
    "OTHER_CPLUSPLUSFLAGS" => "$(inherited) -fmodules",
    "HEADER_SEARCH_PATHS" => "$(inherited) ${PODS_TARGET_SRCROOT}/OpenXLSX ${PODS_TARGET_SRCROOT}/OpenXLSX/OpenXLSX ${PODS_TARGET_SRCROOT}/OpenXLSX/OpenXLSX/headers ${PODS_TARGET_SRCROOT}/OpenXLSX/ThirdParty/pugixml/src ${PODS_TARGET_SRCROOT}/OpenXLSX/ThirdParty/miniz",
  }

  s.user_target_xcconfig = {
    "OTHER_CFLAGS" => "$(inherited) -fmodules",
    "OTHER_CPLUSPLUSFLAGS" => "$(inherited) -fmodules",
    "HEADER_SEARCH_PATHS" => "$(inherited) ${PODS_TARGET_SRCROOT}/OpenXLSX ${PODS_TARGET_SRCROOT}/OpenXLSX/OpenXLSX ${PODS_TARGET_SRCROOT}/OpenXLSX/OpenXLSX/headers ${PODS_TARGET_SRCROOT}/OpenXLSX/ThirdParty/pugixml/src ${PODS_TARGET_SRCROOT}/OpenXLSX/ThirdParty/miniz",
  }

  # Prepare OpenXLSX and its dependencies (pugixml, miniz) via a Node script
  # so the same logic can be reused from both CocoaPods and Android CMake.
  s.prepare_command = "node #{__dir__}/scripts/prepare-openxlsx.js"

  # Swift Package Manager declaration for React Native 0.87+.
  # This exposes NitroXlsx as a local Swift package; the heavy lifting for
  # OpenXLSX is still done by the prepare command above.
  if ENV['NITRO_XLSX_USE_SPM'] == '1' && defined?(SPM) && SPM.respond_to?(:dependency)
    SPM.dependency(s,
      url: File.join(__dir__, "."),
      requirement: { kind: 'upToNextMajorVersion', minimumVersion: s.version.to_s },
      products: ['NitroXlsx']
    )
  end

  load 'nitrogen/generated/ios/NitroXlsx+autolinking.rb'
  add_nitrogen_files(s)
end
