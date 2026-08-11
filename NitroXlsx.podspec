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

  s.exclude_files = "cpp/tests/**/*"

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

  s.prepare_command = <<-CMD
    if [ ! -d "OpenXLSX" ]; then
      git clone --depth 1 --branch v0.5.1 https://github.com/troldal/OpenXLSX.git
    fi
    mkdir -p OpenXLSX/ThirdParty
    if [ ! -d "OpenXLSX/ThirdParty/pugixml" ]; then
      git clone --depth 1 --branch v1.15 https://github.com/zeux/pugixml.git OpenXLSX/ThirdParty/pugixml
    fi
    if [ ! -d "OpenXLSX/ThirdParty/miniz" ]; then
      git clone --depth 1 --branch 3.0.2 https://github.com/richgel999/miniz.git OpenXLSX/ThirdParty/miniz
    fi
    if [ ! -f "OpenXLSX/ThirdParty/miniz/miniz_export.h" ]; then
      cat > OpenXLSX/ThirdParty/miniz/miniz_export.h << 'EOF'
#ifndef MINIZ_EXPORT_H
#define MINIZ_EXPORT_H

#ifndef MINIZ_EXPORT
#define MINIZ_EXPORT
#endif

#ifndef MINIZ_DEPRECATED
#define MINIZ_DEPRECATED
#endif

#ifndef MINIZ_DEPRECATED_EXPORT
#define MINIZ_DEPRECATED_EXPORT MINIZ_EXPORT MINIZ_DEPRECATED
#endif

#ifndef MINIZ_NO_EXPORT
#define MINIZ_NO_EXPORT
#endif

#endif
EOF
    fi
    if [ ! -f "OpenXLSX/OpenXLSX/headers/OpenXLSX-Exports.hpp" ]; then
      cat > OpenXLSX/OpenXLSX/headers/OpenXLSX-Exports.hpp << 'EOF'
#ifndef OPENXLSX_EXPORTS_HPP
#define OPENXLSX_EXPORTS_HPP

#ifndef OPENXLSX_EXPORT
#define OPENXLSX_EXPORT
#endif

#ifndef OPENXLSX_HIDDEN
#define OPENXLSX_HIDDEN
#endif

#endif
EOF
    fi
    if [ ! -d "OpenXLSX/OpenXLSX/headers/OpenXLSX" ]; then
      cd OpenXLSX/OpenXLSX/headers && ln -sf . OpenXLSX && cd -
    fi
  CMD

  load 'nitrogen/generated/ios/NitroXlsx+autolinking.rb'
  add_nitrogen_files(s)
end
