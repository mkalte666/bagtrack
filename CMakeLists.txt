# Licence note is at the end of the file
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(bagtrack)
include(cmake/funcEnableStrictOptions.cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
add_subdirectory(3rdparty/imgui-cmake-blob)

set(JSON_BuildTests
    OFF
    CACHE BOOL "json tests" FORCE)
set(JSON_Install
    OFF
    CACHE BOOL "json install" FORCE)
add_subdirectory(3rdparty/json)

set(HTTPLIB_COMPILE
    ON
    CACHE BOOL "httplib compile" FORCE)
add_subdirectory(3rdparty/cpp-httplib)

add_subdirectory(3rdparty/fmt)

if(WIN32)
    message(FATAL_ERROR "fix nfd for win32!")
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    find_package(GTK REQUIRED)
    add_library(nfd STATIC 3rdparty/nativefiledialog/src/nfd_common.c
                           3rdparty/nativefiledialog/src/nfd_gtk.c)
    target_include_directories(nfd PUBLIC 3rdparty/nativefiledialog/src/include
                                          ${GTK3_INCLUDE_DIRS})
    target_link_libraries(nfd PUBLIC ${GTK3_LIBRARIES})
endif()

add_executable(
    bagtrack
    src/api.cpp
    src/api.h
    src/filteritems.cpp
    src/filteritems.h
    src/fixfmt.h
    src/infocache.cpp
    src/infocache.h
    src/iteminfo.cpp
    src/iteminfo.h
    src/itemtracker.cpp
    src/itemtracker.h
    src/main.cpp
    src/messagebox.h
    src/settings.cpp
    src/settings.h
    src/sortitems.cpp
    src/sortitems.h
    src/time_killer.h
    src/tpinfo.cpp
    src/tpinfo.h
    src/trackerstate.cpp
    src/trackerstate.h
    src/windows/apikeyeditor.cpp
    src/windows/apikeyeditor.h
    src/windows/goldtracker.cpp
    src/windows/goldtracker.h
    src/windows/itemselector.cpp
    src/windows/itemselector.h
    src/windows/itemwidget.cpp
    src/windows/itemwidget.h
    src/windows/timeselector.cpp
    src/windows/timeselector.h
    src/windows/trackereverything.cpp
    src/windows/trackereverything.h
    src/windows/window.cpp
    src/windows/window.h)
target_include_directories(bagtrack PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(
    bagtrack
    imgui
    gl3w
    nfd
    SDL2::SDL2_image
    SDL2::SDL2
    SDL2::SDL2main
    fmt::fmt
    nlohmann_json::nlohmann_json
    httplib::httplib)

# fmt breaks strict options
enablestrictoptions(bagtrack)
# bagtrack CMakeLists.txt
#
# Copyright (C) 2020 Malte Kießling
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <https://www.gnu.org/licenses/>.
