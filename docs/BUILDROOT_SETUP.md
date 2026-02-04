# Buildroot Setup Guide

Complete instructions for building the embedded Linux system for SAMA5D3.

## Prerequisites

### Linux Environment

Buildroot requires a Linux environment. On macOS, use Lima:
```bash
brew install lima
limactl start
lima
```

### Install Dependencies
```bash
sudo apt-get update
sudo apt-get install -y build-essential git wget cpio rsync bc unzip libncurses-dev
```

## Building the System

### 1. Clone Buildroot
```bash
mkdir -p ~/embedded
cd ~/embedded
git clone https://github.com/buildroot/buildroot.git
cd buildroot
```

### 2. Configure for SAMA5D3
```bash
make atmel_sama5d3_xplained_mmc_defconfig
```

### 3. Enable C++ Support
```bash
echo "BR2_TOOLCHAIN_BUILDROOT_CXX=y" >> .config
echo "BR2_INSTALL_LIBSTDCPP=y" >> .config
make olddefconfig
```

### 4. Build
```bash
make
```

**Build time:** 20-60 minutes depending on system specs

### 5. Locate Output Images
```bash
ls -lh output/images/
```

Key files:
- `sdcard.img` - Complete bootable SD card image
- `zImage` - Linux kernel
- `u-boot.bin` - Bootloader
- `rootfs.ext4` - Root filesystem

## Output Structure
```
output/
├── images/
│   ├── sdcard.img          # Flash this to SD card
│   ├── zImage              # Linux kernel
│   ├── boot.bin            # AT91Bootstrap
│   ├── u-boot.bin          # U-Boot bootloader
│   └── rootfs.ext4         # Root filesystem
├── host/
│   └── bin/
│       └── arm-*-gcc       # Cross-compiler toolchain
└── target/                 # Target rootfs (for reference)
```

## Customization

### Adding Packages
```bash
make menuconfig
# Navigate to Target packages -> ...
# Select desired packages
make
```

### Kernel Configuration
```bash
make linux-menuconfig
make
```

## Troubleshooting

### macOS Compatibility Issues

Buildroot expects GNU/Linux. Common macOS issues:
- Use Lima for proper Linux environment
- BSD tools (sed, find, tar) won't work - use GNU versions

### Build Failures
```bash
# Clean and retry
make clean
make
```

### Missing C++ Compiler

Ensure `BR2_TOOLCHAIN_BUILDROOT_CXX=y` is set in `.config`

## Next Steps

After successful build, see:
- [Hardware Setup](HARDWARE_SETUP.md) - Connect serial console
- [Deployment Guide](DEPLOYMENT.md) - Flash SD card and boot
