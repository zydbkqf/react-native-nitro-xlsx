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
  s.source       = { :git => "", :tag => "#{s.version}" }

  s.source_files = [
    "cpp/**/*.{h,hpp,cpp}",
    "OpenXLSX/OpenXLSX/**/*.hpp",
    "OpenXLSX/OpenXLSX/**/*.cpp",
    "OpenXLSX/ThirdParty/pugixml/**/*.hpp",
    "OpenXLSX/ThirdParty/pugixml/**/*.cpp",
    "OpenXLSX/ThirdParty/Zippy/**/*.hpp",
    "OpenXLSX/ThirdParty/Zippy/**/*.cpp",
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
    "HEADER_SEARCH_PATHS" => "$(inherited) ${PODS_TARGET_SRCROOT}/OpenXLSX ${PODS_TARGET_SRCROOT}/OpenXLSX/ThirdParty/pugixml ${PODS_TARGET_SRCROOT}/OpenXLSX/ThirdParty/Zippy ${PODS_TARGET_SRCROOT}/OpenXLSX/ThirdParty/miniz",
  }

  s.user_target_xcconfig = {
    "OTHER_CFLAGS" => "$(inherited) -fmodules",
    "OTHER_CPLUSPLUSFLAGS" => "$(inherited) -fmodules",
    "HEADER_SEARCH_PATHS" => "$(inherited) ${PODS_TARGET_SRCROOT}/OpenXLSX ${PODS_TARGET_SRCROOT}/OpenXLSX/ThirdParty/pugixml ${PODS_TARGET_SRCROOT}/OpenXLSX/ThirdParty/Zippy ${PODS_TARGET_SRCROOT}/OpenXLSX/ThirdParty/miniz",
  }

  s.prepare_command = <<-CMD
    if [ ! -d "OpenXLSX" ]; then
      git clone --depth 1 --branch v0.5.1 https://github.com/troldal/OpenXLSX.git
    fi
    if [ ! -d "OpenXLSX/ThirdParty" ]; then
      cd OpenXLSX && git submodule update --init --recursive && cd ..
    fi
  CMD

  load 'nitrogen/generated/ios/NitroXlsx+autolinking.rb'
  add_nitrogen_files(s)
end
