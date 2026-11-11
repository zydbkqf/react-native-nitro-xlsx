// swift-tools-version: 5.9
import PackageDescription

// NOTE: This manifest is a placeholder for the Swift Package Manager migration,
// not the build path used today. CocoaPods (NitroXlsx.podspec) still compiles
// the library, including the vendored OpenXLSX / pugixml / miniz tree under
// thirdparty/.
//
// It is kept so that (a) the package is syntactically valid and `swift package
// describe` / `swift build` work, and (b) the eventual migration has a starting
// point. Today the target only compiles ios/XlsxCacheDir.mm — it exposes
// getCacheDir / setCacheDir but not the Nitro module itself.
//
// Migration target (React Native 0.88+ / nitro-modules 1.x): the package becomes
// one library product whose targets cover `cpp/`, `ios/XlsxCacheDir.mm` and the
// thirdparty/ sources, depending on `.product(name: "NitroModules",
// package: "NitroModules")`, with `swiftpmConfig` in package.json driving
// autolinking via `npx react-native spm`. The unrelated NITRO_XLSX_USE_SPM env
// switch that used to live in the podspec has been removed — it was never an
// upstream concept.
//
// Target settings worth knowing:
// - This is a C-family target, so SwiftPM insists on a public headers
//   directory and defaults it to `include/`, which does not exist here.
//   `publicHeadersPath: "."` points the generated umbrella header at the
//   (header-less) source dir instead of failing to resolve the manifest.
// - cxxLanguageStandard must match the podspec's CLANG_CXX_LANGUAGE_STANDARD
//   (c++20); SwiftPM defaults to gnu++14 and warns on the nested namespace
//   definitions used by the sources.

let package = Package(
    name: "NitroXlsx",
    platforms: [
        // Matches NitroXlsx.podspec (`:ios => "15.1"`). SwiftPM's .v15 only
        // expresses 15.0, so the explicit version string is used instead.
        .iOS("15.1")
    ],
    products: [
        .library(
            name: "NitroXlsx",
            targets: ["NitroXlsx"]
        )
    ],
    targets: [
        .target(
            name: "NitroXlsx",
            path: "ios",
            sources: ["XlsxCacheDir.mm"],
            publicHeadersPath: ".",
            cxxSettings: [
                .headerSearchPath("../cpp")
            ]
        )
    ],
    cxxLanguageStandard: .cxx20
)
