cmake_minimum_required(VERSION 3.0.2)
cmake_policy(SET CMP0048 NEW)

project(libertine 
        VERSION 0.99.1
        LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

find_package(PkgConfig REQUIRED)
find_package(GObjectIntrospection REQUIRED)
include(GNUInstallDirs)
include(CheckIncludeFile)
include(CheckFunctionExists)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})

find_package(Qt5Core  REQUIRED)
find_package(Qt5Gui   REQUIRED)
find_package(Qt5Quick REQUIRED)

pkg_check_modules(GLIB2 REQUIRED glib-2.0)
include_directories(${GLIB2_INCLUDE_DIRS})

pkg_check_modules(PYTHON3 REQUIRED python3)
include_directories(${PYTHON3_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)

add_subdirectory(libertine)
add_subdirectory(python)
add_subdirectory(data)
add_subdirectory(tools)
add_subdirectory(liblibertine)

include(CTest)
add_subdirectory(tests)

set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${LIBERTINE_VERSION})
add_custom_target(dist
  COMMAND echo debian export-ignore >.gitattributes
  && git archive --worktree-attributes --prefix=${ARCHIVE_NAME}/ HEAD
     | xz >${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.xz
  && rm .gitattributes
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

