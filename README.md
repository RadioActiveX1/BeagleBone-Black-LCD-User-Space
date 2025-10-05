# BeagleBone-Black-LCD-User-Space

This guide walks you through uploading, wiring, compiling, and running a **user-space LCD driver** (not a kernel module) for a **16x2 LCD** on the BeagleBone Black using GPIO in **Python** and **C++**.

---

## Pin Map

| LCD Pin | Signal | BBB Header | GPIO | Description |
|----------|---------|-------------|--------|--------------|
| 1 | GND | P8_1, P9_1 | - | Power ground |
| 2 | VCC (+5V) | P9_7, P9_8 | - | Power +5V |
| 3 | V0 (Contrast) | - | - | Potentiometer Recommended |
| 4 | RS | P8_7 | GPIO2_2 | Command/Data register select |
| 5 | RW (tie to GND) | P8_8* | GPIO2_3 | Usually always GND (write) |
| 6 | E (Enable) | P8_9 | GPIO2_5 | Enable signal |
| 11 | D4 | P8_10 | GPIO2_4 | Data line (4-bit mode) |
| 12 | D5 | P8_11 | GPIO1_13 | Data line |
| 13 | D6 | P8_12 | GPIO1_12 | Data line |
| 14 | D7 | P8_14 | GPIO0_26 | Data line |
| 15 | LED+ (Backlight) | P9_7 via 220Ω | - | Optional |
| 16 | LED− (Backlight) | P8_1, P9_1 | - | Optional |

> *If you’re not reading from the LCD, tie RW permanently to GND.*

---

## Hardware Setup

1. Connect LCD pins to BeagleBone Black headers as listed above.  
2. Use a **10 kΩ potentiometer** for LCD contrast between `V0` and GND/VCC.  
3. Power the LCD with **5 V (P9_7 or P9_8)**.  
4. Double-check wiring before powering up.

---

## Python (User-Space LCD Control)

### Prerequisites

```
sudo apt-get update
sudo pip install Adafruit_BBIO
sudo python user_space/lcd_user_python.py
```
Key Points:

Uses Adafruit_BBIO.GPIO for pin control.

No kernel modules or device tree overlays required.

Must run as root to access GPIOs.

---

## C++ (User-Space LCD Control)

This version uses standard file I/O via /sys/class/gpio to control GPIOs in user space.

🔧 Prerequisites (on BeagleBone Black)

No extra libraries needed — just the built-in GCC compiler.

Compile directly on the BeagleBone:

```
g++ lcd_user_cpp.cpp -o lcd_user_cpp
sudo ./lcd_user_cpp
```

### Cross-Compiling from Host (e.g. Ubuntu PC)

If you prefer to build on your PC and then transfer the binary:

1. Install the ARM cross-compiler
```
sudo apt update
sudo apt install g++-arm-linux-gnueabihf
```
2. Compile for BeagleBone Black (ARM target)
```
arm-linux-gnueabihf-g++ lcd_user_cpp.cpp -o lcd_user_cpp
```
3. Copy to board via SSH/SCP then run on board using
```
ssh debian@192.168.7.2
chmod +x lcd_user_cpp
sudo ./lcd_user_cpp
```
### Usage Notes

Always run with root privileges to access GPIOs.

Adjust GPIO numbers in your code if wiring changes.

Use a 220 Ω resistor for the LCD backlight LED.

GPIO pins are 3.3 V logic — not for powering loads directly.

For LEDs, use a current-limiting resistor.

For motors/relays, use a transistor or MOSFET driver circuit.
