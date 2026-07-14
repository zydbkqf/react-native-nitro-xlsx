cmake_minimum_required(VERSION 3.22.1)
project("NitroXlsx")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPENXLSX_DIR ${CMAKE_SOURCE_DIR}/../OpenXLSX)
if(NOT EXISTS ${OPENXLSX_DIR}/CMakeLists.txt)
  # Clean up incomplete directory to ensure fresh clone
  if(EXISTS ${OPENXLSX_DIR})
    file(REMOVE_RECURSE ${OPENXLSX_DIR})
  endif()

  include(FetchContent)
  FetchContent_Declare(
    OpenXLSX
    GIT_REPOSITORY https://github.com/troldal/OpenXLSX.git
    GIT_TAG v0.5.1
    SOURCE_DIR ${OPENXLSX_DIR}
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
  )
  FetchContent_Populate(OpenXLSX)
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

# Create symlink for #include <OpenXLSX/XXX.hpp> resolution
# Same as iOS podspec: cd OpenXLSX/OpenXLSX/headers && ln -sf . OpenXLSX
if(NOT EXISTS ${OPENXLSX_DIR}/OpenXLSX/headers/OpenXLSX)
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink . OpenXLSX
    WORKING_DIRECTORY ${OPENXLSX_DIR}/OpenXLSX/headers
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
