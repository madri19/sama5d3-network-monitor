#!/bin/bash
set -e

# Path to Buildroot toolchain
BUILDROOT_PATH=~/buildroot-lima
TOOLCHAIN_PATH=$BUILDROOT_PATH/output/host/bin

# Create build directory
mkdir -p build
cd build

# Run CMake with explicit toolchain settings
cmake .. \
    -DCMAKE_C_COMPILER=$TOOLCHAIN_PATH/arm-buildroot-linux-gnueabihf-gcc \
    -DCMAKE_CXX_COMPILER=$TOOLCHAIN_PATH/arm-buildroot-linux-gnueabihf-g++ \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_SYSTEM_PROCESSOR=arm \
    -DCMAKE_FIND_ROOT_PATH=$BUILDROOT_PATH/output/host/arm-buildroot-linux-gnueabihf/sysroot \
    -DCMAKE_INSTALL_PREFIX=/usr

# Build
make -j$(nproc)

echo ""
echo "Build complete! Binary: build/network_monitor"
echo "To deploy: scp build/network_monitor root@<board-ip>:/usr/bin/"
