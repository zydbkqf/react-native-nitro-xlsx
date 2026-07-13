cmake_minimum_required(VERSION 3.14)
project(NitroXlsxTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Find OpenXLSX
include(FetchContent)

# Option to use local OpenXLSX (set OPENXLSX_PATH env var to override)
set(OPENXLSX_PATH "" CACHE PATH "Path to local OpenXLSX source")

if(OPENXLSX_PATH AND EXISTS "${OPENXLSX_PATH}/CMakeLists.txt")
  message(STATUS "Using local OpenXLSX from: ${OPENXLSX_PATH}")
  set(OpenXLSX_SOURCE_DIR "${OPENXLSX_PATH}" CACHE PATH "OpenXLSX source directory")
  add_subdirectory(${OpenXLSX_PATH} ${CMAKE_BINARY_DIR}/_deps/openxlsx-build)
else()
  FetchContent_Declare(
    OpenXLSX
    GIT_REPOSITORY https://github.com/troldal/OpenXLSX.git
    GIT_TAG v0.5.1
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(OpenXLSX)
endif()

# GoogleTest - try to find local installation first, fallback to FetchContent
find_path(GTEST_INCLUDE_DIR
  NAMES gtest/gtest.h
  PATHS
    /opt/homebrew/include
    /usr/local/include
    /usr/include
    ${ANDROID_NDK_HOME}/sources/third_party/googletest/include
    $ENV{HOME}/Library/Android/sdk/ndk/*/sources/third_party/googletest/include
  NO_DEFAULT_PATH
)

find_library(GTEST_LIBRARY
  NAMES gtest
  PATHS
    /opt/homebrew/lib
    /usr/local/lib
    /usr/lib
  NO_DEFAULT_PATH
)

find_library(GTEST_MAIN_LIBRARY
  NAMES gtest_main
  PATHS
    /opt/homebrew/lib
    /usr/local/lib
    /usr/lib
  NO_DEFAULT_PATH
)

set(NITRO_XLSX_TEST_SOURCES
  test_HybridXlsxFormat.cpp
  test_HybridXlsxWorksheet.cpp
  test_HybridXlsxWorkbook.cpp
)

set(NITRO_XLSX_TEST_SOURCES
  test_HybridXlsxFormat.cpp
  test_HybridXlsxWorksheet.cpp
  test_HybridXlsxWorkbook.cpp
  test_main.cpp
)

# Enable testing
enable_testing()

# Build the test binary
add_executable(NitroXlsxTests
  ${NITRO_XLSX_TEST_SOURCES}
)

if(GTEST_INCLUDE_DIR AND GTEST_LIBRARY AND GTEST_MAIN_LIBRARY)
  message(STATUS "Using local GoogleTest: ${GTEST_LIBRARY}")
  target_include_directories(NitroXlsxTests
    PRIVATE
      .
      ${OpenXLSX_SOURCE_DIR}
      ${GTEST_INCLUDE_DIR}
  )
  target_link_libraries(NitroXlsxTests
    PRIVATE
      OpenXLSX
      ${GTEST_LIBRARY}
      ${GTEST_MAIN_LIBRARY}
      pthread
  )
else()
  message(STATUS "Local GoogleTest not found, using FetchContent")
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    GIT_SHALLOW TRUE
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  target_include_directories(NitroXlsxTests
    PRIVATE
      .
      ${OpenXLSX_SOURCE_DIR}
      ${gtest_SOURCE_DIR}/include
  )
  target_link_libraries(NitroXlsxTests
    PRIVATE
      OpenXLSX
      GTest::gtest_main
  )
endif()

# Use C++20
target_compile_features(NitroXlsxTests PUBLIC cxx_std_20)

# Discover tests
include(GoogleTest)
gtest_discover_tests(NitroXlsxTests)