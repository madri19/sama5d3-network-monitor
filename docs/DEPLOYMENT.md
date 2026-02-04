# Deployment Guide

Instructions for flashing the SD card and deploying the network monitor application.

## Flash SD Card Image

### On macOS
```bash
# Insert SD card and find device
diskutil list

# Look for your SD card (e.g., /dev/disk4)
# ⚠️ VERIFY THIS IS YOUR SD CARD - THIS WILL ERASE IT!

# Unmount the disk
diskutil unmountDisk /dev/diskN

# Flash the image (use rdiskN for faster write)
sudo dd if=sdcard.img of=/dev/rdiskN bs=1m

# Eject safely
diskutil eject /dev/diskN
```

### On Linux
```bash
# Insert SD card and find device
lsblk

# Look for your SD card (e.g., /dev/sdb)
# ⚠️ VERIFY THIS IS YOUR SD CARD - THIS WILL ERASE IT!

# Flash the image
sudo dd if=sdcard.img of=/dev/sdX bs=1M status=progress

# Sync and eject
sync
sudo eject /dev/sdX
```

**Flash time:** ~1-2 minutes for the 77MB image

## First Boot

1. Insert SD card into board
2. Connect serial console
3. Power on board
4. Watch boot sequence (~30 seconds)
5. Login as `root` (no password)

## Deploy Network Monitor

### Method 1: Copy to Boot Partition (Quick Testing)

This method is fastest for testing:
```bash
# On board (after boot)
mount /dev/mmcblk0p1 /mnt
cp /mnt/network_monitor /usr/bin/
chmod +x /usr/bin/network_monitor
```

**Note:** Binary must be on FAT32 boot partition first (see below)

### Method 2: Network Transfer (Recommended for Development)

If you have network connectivity:
```bash
# From development machine
scp network_monitor root@<board-ip>:/usr/bin/

# On board
chmod +x /usr/bin/network_monitor
```

### Method 3: Rebuild SD Card Image (Production)

For permanent inclusion in rootfs:

1. Create rootfs overlay directory:
```bash
mkdir -p buildroot/board/atmel/sama5d3_xplained/rootfs_overlay/usr/bin
```

2. Copy binary:
```bash
cp network_monitor buildroot/board/atmel/sama5d3_xplained/rootfs_overlay/usr/bin/
```

3. Configure Buildroot:
```bash
# In buildroot directory
echo 'BR2_ROOTFS_OVERLAY="board/atmel/sama5d3_xplained/rootfs_overlay"' >> .config
make olddefconfig
make
```

4. Reflash SD card with new image

## Running the Application

### Start Network Monitor
```bash
network_monitor &
```

Expected output:
```
Network Monitor Starting...
Server listening on port 8080
Network monitor ready!
Connect with: telnet <board-ip> 8080
Press Ctrl+C to stop
```

### Connect from Remote Machine
```bash
# Using netcat (nc)
nc <board-ip> 8080

# Or telnet (if available)
telnet <board-ip> 8080
```

### Example Output
```
Network Interface Statistics
=============================

Interface: eth0
  RX:         4335 bytes,       53 packets
  TX:         3794 bytes,       35 packets

Interface: lo
  RX:          172 bytes,        3 packets
  TX:          172 bytes,        3 packets
```

## Auto-Start on Boot (Optional)

### Create Init Script
```bash
# On board
cat > /etc/init.d/S99network_monitor << 'EOF'
#!/bin/sh

case "$1" in
  start)
    echo "Starting network_monitor..."
    /usr/bin/network_monitor &
    ;;
  stop)
    echo "Stopping network_monitor..."
    killall network_monitor
    ;;
  restart)
    $0 stop
    sleep 1
    $0 start
    ;;
  *)
    echo "Usage: $0 {start|stop|restart}"
    exit 1
esac
EOF

chmod +x /etc/init.d/S99network_monitor
```

Now the monitor starts automatically on every boot.

## Troubleshooting

### "libstdc++.so.6: cannot open shared object file"

The binary isn't statically linked. Rebuild with:
```bash
# In CMakeLists.txt
set(CMAKE_EXE_LINKER_FLAGS "-static")
```

### "Bind failed on port 8080"

Port already in use:
```bash
killall -9 network_monitor
# Then restart
network_monitor &
```

### Can't Connect from Remote Machine

Check network:
```bash
# On board
ifconfig eth0
ping <remote-ip>

# From remote machine
ping <board-ip>
```

Verify server is running:
```bash
# On board
netstat -tlnp | grep 8080
```

### Application Crashes

Check with verbose output:
```bash
# Run in foreground to see errors
network_monitor
```

## Performance Notes

- **Memory usage:** ~1.7MB static binary
- **CPU usage:** Minimal (<1% idle)
- **Network overhead:** ~100 bytes per connection
- **Response time:** <10ms typical

## Next Steps

- Configure firewall rules if needed
- Set up monitoring/logging
- Integrate with your application
- Add systemd service (if using systemd instead of init.d)
