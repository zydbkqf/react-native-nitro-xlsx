cmake_minimum_required(VERSION 3.22.1)
project("NitroXlsx")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  OpenXLSX
  GIT_REPOSITORY https://github.com/troldal/OpenXLSX.git
  GIT_TAG v0.5.1
)
FetchContent_GetProperties(OpenXLSX)
if(NOT OpenXLSX_POPULATED)
  FetchContent_Populate(OpenXLSX)
  add_subdirectory(${OpenXLSX_SOURCE_DIR} ${OpenXLSX_BINARY_DIR})
endif()

add_library(
  NitroXlsx
  SHARED
  ../cpp/HybridNitroXlsx.cpp
  ../cpp/HybridXlsxWorkbook.cpp
  ../cpp/HybridXlsxWorksheet.cpp
  ../cpp/HybridXlsxFormat.cpp
  cpp-adapter.cpp
)

target_include_directories(
  NitroXlsx
  PRIVATE
  ../cpp
)

target_link_libraries(NitroXlsx OpenXLSX)

target_link_options(NitroXlsx PRIVATE -Wl,--unresolved-symbols=ignore-all)

include(${CMAKE_SOURCE_DIR}/../nitrogen/generated/android/NitroXlsx+autolinking.cmake)
