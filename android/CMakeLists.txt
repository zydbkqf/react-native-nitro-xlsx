cmake_minimum_required(VERSION 3.22.1)
project("NitroXlsx")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/../thirdparty)
set(OPENXLSX_DIR ${THIRDPARTY_DIR}/OpenXLSX)

# OpenXLSX, pugixml and miniz are vendored under thirdparty/ so the build
# never clones anything from GitHub.
foreach(required_dir OpenXLSX pugixml miniz)
  if(NOT EXISTS ${THIRDPARTY_DIR}/${required_dir})
    message(FATAL_ERROR
      "Vendored dependency '${required_dir}' is missing at ${THIRDPARTY_DIR}/${required_dir}.\n"
      "The thirdparty/ tree is part of the package - reinstall react-native-nitro-xlsx."
    )
  endif()
endforeach()

# Point OpenXLSX's FetchContent lookups at the vendored copies, so
# manage_dependency() populates them from disk instead of cloning GitHub.
set(FORCE_FETCH_ALL ON CACHE BOOL "" FORCE)
set(FETCHCONTENT_SOURCE_DIR_PUGIXML_FETCH ${THIRDPARTY_DIR}/pugixml CACHE PATH "" FORCE)
set(FETCHCONTENT_SOURCE_DIR_MINIZ_FETCH ${THIRDPARTY_DIR}/miniz CACHE PATH "" FORCE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "" FORCE)

set(OPENXLSX_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(OPENXLSX_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(OPENXLSX_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(OPENXLSX_CREATE_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(${OPENXLSX_DIR} OpenXLSX_build)

add_library(
  NitroXlsx
  SHARED
  ../cpp/HybridNitroXlsx.cpp
  ../cpp/HybridXlsxWorkbook.cpp
  ../cpp/HybridXlsxWorksheet.cpp
  ../cpp/HybridXlsxCellFormat.cpp
  ../cpp/HybridXlsxConditionalFormat.cpp
  ../cpp/HybridXlsxConditionalFormats.cpp
  ../cpp/XlsxJson.cpp
  ../cpp/XlsxCacheDir.cpp
  cpp-adapter.cpp
)

target_include_directories(
  NitroXlsx
  PRIVATE
  ../cpp
  ${OPENXLSX_DIR}
  ${OPENXLSX_DIR}/OpenXLSX/headers
)

target_link_libraries(NitroXlsx OpenXLSX)

target_link_options(NitroXlsx PRIVATE -Wl,--unresolved-symbols=ignore-all)

include(${CMAKE_SOURCE_DIR}/../nitrogen/generated/android/NitroXlsx+autolinking.cmake)
