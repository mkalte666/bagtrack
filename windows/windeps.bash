#!/bin/bash

# error reporting during builds
set -e
print_error() {
  echo "Error while building. Check the build.log in the build folder"
}
trap 'print_error $LINENO' ERR

# config
if [ -z "$HOST" ]; then
  HOST="x86_64-w64-mingw32"
fi
cc_name="$(command -v $HOST-gcc)"
cxx_name="$(command -v $HOST-g++)"
rc_name="$(command -v $HOST-windres)"

sdl_name="SDL2-2.0.12"
ssl_name="openssl-1.1.1h"

sdl_tar="$sdl_name.tar.gz"
ssl_tar="$ssl_name.tar.gz"

# note for users
echo "If this fails:"
echo "Make sure mingw-w64 is installed and has all usual libs (d3d etc) installed"
echo "Also, you need c++17 support! gcc should be >= 8"
echo "In case it doesnt find the compiler at all, set the HOST variable. it is currently set to '$HOST'"
echo "Example: On ubuntu, this should be something like 'x86_64-w64-mingw32'"
# cleanup
echo "Cleanup old dep download and build"
rm -rf ./deps

root_dir="$(realpath ./)"
mkdir -p ./deps/dl || exit 1
dl_dir="$(realpath ./deps/dl)"
mkdir -p ./deps/build || exit 1
build_dir="$(realpath ./deps/build)"
mkdir -p ./deps/install || exit 1
local_root="$(realpath deps/install)"

echo "local root: $local_root"

# make cmake toolchain file (needed later)
echo "Building toolchain file"
roots="/usr/$HOST/ $local_root"
{
  echo "set(CMAKE_SYSTEM_NAME Windows)"
  echo "set(CMAKE_C_COMPILER $cc_name)"
  echo "set(CMAKE_CXX_COMPILER $cxx_name)"
  echo "set(CMAKE_RC_COMPILER $rc_name)"
  echo "set(CMAKE_FIND_ROOT_PATH $roots)"
  echo "set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)"
  echo "set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)"
  echo "set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)"
} >> deps/toolchain.cmake || exit 1

# download dependencies
echo "Downloading dependencies"
cd deps/dl || exit 1
wget --quiet "https://www.libsdl.org/release/$sdl_tar" &
wget --quiet "https://www.openssl.org/source/$ssl_tar" &
wait
echo "Extracting dependencies"
tar xfz "$sdl_tar"
rm -f "$sdl_tar"
tar xfz "$ssl_tar"
rm -f "$ssl_tar"
cd "$root_dir"

# builds come below

# sdl
echo "Building SDL2"
mkdir -p "$build_dir/$sdl_name"
cd "$build_dir/$sdl_name" || exit 1
{
  cmake "$dl_dir/$sdl_name/" \
    -DCMAKE_TOOLCHAIN_FILE="$root_dir/deps/toolchain.cmake" \
    -DCMAKE_BUILD_TYPE=Release\
    -DCMAKE_INSTALL_PREFIX="$local_root"
  make -j"$(nproc)"
  make install
} >> build.log 2>&1 || exit 1

# ssl
echo "Building OpenSSL"
mkdir -p "$build_dir/$ssl_name"
cd "$build_dir/$ssl_name" || exit 1
{
  export CC="$cc_name"
  export CXX="$cxx_name"
  export RC="$rc_name"
  "$dl_dir/$ssl_name/Configure" --prefix="$local_root" no-idea no-mdc2 no-rc5 shared mingw64
  make depend
  make -j"$(nproc)"
  make install
} >> build.log 2>&1 || exit 1