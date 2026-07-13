cmake_minimum_required(VERSION 3.22.1)
project("NitroXlsx")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPENXLSX_DIR ${CMAKE_SOURCE_DIR}/../OpenXLSX)
if(NOT EXISTS ${OPENXLSX_DIR}/CMakeLists.txt)
  include(FetchContent)
  FetchContent_Declare(
    OpenXLSX
    GIT_REPOSITORY https://github.com/troldal/OpenXLSX.git
    GIT_TAG v0.5.1
  )
  FetchContent_GetProperties(OpenXLSX)
  if(NOT OpenXLSX_POPULATED)
    FetchContent_Populate(OpenXLSX)
  endif()
  set(OPENXLSX_DIR ${openxlsx_SOURCE_DIR})
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
