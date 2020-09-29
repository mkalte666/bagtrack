# Licence note is at the end of the file
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(remute)
include(cmake/funcEnableStrictOptions.cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)
add_subdirectory(3rdparty/imgui-cmake-blob)

set(JSON_BuildTests OFF CACHE BOOL "json tests" FORCE)
set(JSON_Install OFF CACHE BOOL "json install" FORCE)
add_subdirectory(3rdparty/json)

set(HTTPLIB_COMPILE ON CACHE BOOL "httplib compile" FORCE)
add_subdirectory(3rdparty/cpp-httplib)

add_executable(bagtrack src/main.cpp src/messagebox.h)
target_include_directories(bagtrack PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(bagtrack imgui gl3w SDL2::SDL2 SDL2::SDL2main)
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