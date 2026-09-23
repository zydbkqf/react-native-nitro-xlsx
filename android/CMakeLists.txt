cmake_minimum_required(VERSION 3.22.1)
project("NitroXlsx")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPENXLSX_DIR ${CMAKE_SOURCE_DIR}/../OpenXLSX)

# Prepare OpenXLSX and its dependencies (pugixml, miniz) using the same
# Node script that CocoaPods' prepare_command runs on iOS.
find_program(NODE_EXECUTABLE node REQUIRED)
execute_process(
  COMMAND ${NODE_EXECUTABLE} ${CMAKE_SOURCE_DIR}/../scripts/prepare-openxlsx.js
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/..
  RESULT_VARIABLE PREPARE_RESULT
)
if(NOT PREPARE_RESULT EQUAL 0)
  message(FATAL_ERROR
    "Failed to prepare OpenXLSX dependencies.\n"
    "Please ensure Node.js is installed and run:\n"
    "  node scripts/prepare-openxlsx.js"
  )
endif()

# Verify OpenXLSX is available
if(NOT EXISTS ${OPENXLSX_DIR}/CMakeLists.txt)
  message(FATAL_ERROR
    "OpenXLSX not found at ${OPENXLSX_DIR}.\n"
    "Please manually clone OpenXLSX:\n"
    "  cd ${CMAKE_SOURCE_DIR}/.. && git clone --depth 1 --branch v0.5.1 https://github.com/troldal/OpenXLSX.git OpenXLSX\n"
    "Then clean CMake cache and rebuild."
  )
endif()

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
