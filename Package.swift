// swift-tools-version: 5.9
import PackageDescription

// NOTE: This Package.swift is a Swift Package Manager manifest stub for
// React Native 0.87+ compatibility. The library is still primarily built
// through CocoaPods (NitroXlsx.podspec), where OpenXLSX, pugixml and miniz
// are cloned and compiled from source by scripts/prepare-openxlsx.js.
//
// The local package reference below lets RN 0.87's spm_dependency helper
// register NitroXlsx in the Pods project when SwiftPM mode is enabled.
// Full SPM-based compilation of the C++ sources is not yet supported.

let package = Package(
    name: "NitroXlsx",
    platforms: [
        .iOS(.v15)
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
            sources: ["dummy.swift"]
        )
    ]
)
