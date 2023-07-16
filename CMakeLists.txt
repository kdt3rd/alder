# SPDX-License-Identifier: MIT
# Copyright (c) Contributors to the Alder project.

cmake_minimum_required(VERSION 3.12)

if(POLICY CMP0074)
  # enable find_package(<Package>) to use <Package>_ROOT as a hint
  cmake_policy(SET CMP0074 NEW)
endif()

if(POLICY CMP0077)
  # enable variables set outside to override options
  cmake_policy(SET CMP0077 NEW)
endif()

# project version from source
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/include/alder/version.h"
     alder_ver_defines REGEX "#define ALDER_VERSION_(MAJOR|MINOR|PATCH) ")

foreach(ver ${alder_ver_defines})
  if(ver MATCHES [[#define ALDER_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$]])
    set(ALDER_VER_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}")
  endif()
endforeach()

if(ALDER_VER_PATCH MATCHES [[\.([a-zA-Z0-9]+)$]])
  set(pybind11_VERSION_TYPE "${CMAKE_MATCH_1}")
endif()
string(REGEX MATCH "^[0-9]+" ALDER_VER_PATCH "${ALDER_VER_PATCH}")

set(ALDER_LIB_VERSION "${ALDER_VER_MAJOR}.${ALDER_VER_MINOR}.${ALDER_VER_PATCH}" CACHE STRING "lib version" )
message(INFO " configuring version ${ALDER_LIB_VERSION}" )

project(Alder VERSION ${ALDER_LIB_VERSION} LANGUAGES C CXX)

set(tmp 20)
if(CMAKE_CXX_STANDARD)
  set(tmp ${CMAKE_CXX_STANDARD})
endif()
set(ALDER_CXX_STANDARD "${tmp}" CACHE STRING "C++ standard to compile against")
set(tmp)

set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "\${ORIGIN}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
if(APPLE)
  set(CMAKE_MACOSX_RPATH ON)
endif()

if(BUILD_TESTING AND NOT ALDER_IS_SUBPROJECT)
  # Enable testing *before* adding any subdirectories that may include tests
  enable_testing()
  set(ALDER_DO_TESTS TRUE)
endif()

include(GNUInstallDirs)

if(NOT "${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
  set(ALDER_IS_SUBPROJECT ON)
  message(STATUS "Alder is configuring as a cmake subproject")
endif()

########################################

add_library(alder STATIC)

target_sources(alder
  PRIVATE
    src/machine.cpp
  PUBLIC FILE_SET HEADERS 
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
  FILES
    include/alder/version.h
    include/alder/machine.h
)

# only use C++11 in API, but use more advanced features internally
# but don't force everyone to comply (be careful in the API!!!)
target_compile_features(alder
  PRIVATE cxx_std_${ALDER_CXX_STANDARD}
  INTERFACE cxx_std_11
)

set_target_properties(alder PROPERTIES
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
  C_VISIBILITY_PRESET hidden
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN ON
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

#if(BUILD_SHARED_LIBS)
#  set_target_properties(alder PROPERTIES
#    SOVERSION 1
#    VERSION ${ALDER_LIB_VERSION}
#  )
#endif()

# CMake how to include the "installed" version of the headers.
target_include_directories(alder
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_include_directories(alder PRIVATE "${PROJECT_SOURCE_DIR}/src")

install(TARGETS alder
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PUBLIC_HEADER
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/alder
  FILE_SET HEADERS
)
add_library(${PROJECT_NAME}::alder ALIAS alder)


########################################

if(ALDER_DO_TESTS)
  add_executable(test_machine
    test/machine_test.cpp
    )
  target_link_libraries(test_machine ${PROJECT_NAME}::alder)
  add_test(NAME machine COMMAND test_machine)
endif()

