# Hardware Setup Guide

Physical connections and serial console setup for SAMA5D3 Xplained board.

## Required Hardware

- Atmel SAMA5D3 Xplained evaluation board
- USB-to-UART adapter (e.g., FTDI FT232)
- microSD card (4GB minimum, 8GB+ recommended)
- Ethernet cable
- 5V power supply
- Jumper wires for UART connection

## Board Overview

### SAMA5D3 Xplained Key Components

- **CPU:** ARM Cortex-A5 @ 536 MHz
- **RAM:** 256 MB DDR2
- **Storage:** microSD slot
- **Network:** 2x Gigabit Ethernet ports
- **Serial:** UART debug header (J23)

## Serial Console Connection

### UART Pinout (J23 Header)
```
Pin 1: GND
Pin 2: RXD (board receives)
Pin 3: TXD (board transmits)
Pin 4: VCC (3.3V) - DO NOT CONNECT
```

### Wiring

Connect USB-UART adapter to board:
```
Adapter      Board (J23)
-------      -----------
GND    <-->  Pin 1 (GND)
TXD    <-->  Pin 2 (RXD)  <- Cross these
RXD    <-->  Pin 3 (TXD)  <- Cross these
```

⚠️ **Important:** TX/RX must be crossed! Adapter TX → Board RX, Adapter RX → Board TX

⚠️ **Voltage:** Ensure adapter is set to 3.3V logic levels (not 5V)

### Serial Terminal Settings

**Baud Rate:** 115200  
**Data Bits:** 8  
**Parity:** None  
**Stop Bits:** 1  
**Flow Control:** None

### Connecting on macOS
```bash
# Find device
ls /dev/tty.usbserial*

# Connect with screen
screen /dev/tty.usbserial-XXXXXXXX 115200
```

**Exit screen:** Press `Ctrl-A` then `K`, confirm with `y`

### Connecting on Linux
```bash
# Find device
ls /dev/ttyUSB*

# Connect with screen
screen /dev/ttyUSB0 115200

# Alternative: minicom
minicom -D /dev/ttyUSB0 -b 115200
```

## Network Setup

### Ethernet Connection

**Board has 2 Ethernet ports:**
- **eth0** - Lower port (use this one)
- **eth1** - Upper port

Connect eth0 to your router/switch with standard Ethernet cable.

### Getting IP Address (on board)
```bash
# Enable interface
ifconfig eth0 up

# Get DHCP address
udhcpc -i eth0

# Check assigned IP
ifconfig eth0
```

## Power-Up Sequence

1. Insert programmed microSD card
2. Connect serial console
3. Connect Ethernet cable
4. Connect power supply
5. Watch boot sequence on serial console

## Expected Boot Output
```
RomBOOT

AT91Bootstrap...
U-Boot 2021.10...
Loading kernel...
Starting kernel...

[    0.000000] Booting Linux on physical CPU 0x0
...
Welcome to Buildroot
buildroot login:
```

**Default login:** `root` (no password)

## Troubleshooting

### No Serial Output

- Check wiring (TX/RX crossed?)
- Verify baud rate (115200)
- Check adapter voltage (3.3V)
- Try different USB port

### Board Won't Boot

- Check SD card insertion
- Verify SD card image is properly flashed
- Check power supply (5V, adequate current)
- Look for LED activity on board

### No Network

- Check cable connection
- Verify router link lights
- Try different Ethernet cable
- Use `dmesg | grep eth0` to check for errors

## Next Steps

After hardware is connected and working, see:
- [Deployment Guide](DEPLOYMENT.md) - Deploy the network monitor application
