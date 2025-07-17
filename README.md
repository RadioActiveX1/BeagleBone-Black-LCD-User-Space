# BeagleBone-Black-LCD-User-Space
This guide walks you through uploading, documenting, wiring, and running a user-space LCD driver (not a kernel module) for a 16x2 LCD on the BeagleBone Black using simple GPIO access in Python and C.

## Pin Map
1	  GND	  P8_1, P9_1	-	Power ground

2	  VCC   (+5V)	P9_7, P9_8	-	Power +5V

3	  V0    (Contrast)	-	-	Potentiometer Recommended

4  	RS	  P8_7	GPIO2_2	Command/Data register select

5	  RW    (tie to GND)	P8_8*	GPIO2_3	Usually always GND (write)

6	  E     (Enable)	P8_9	GPIO2_5	Enable signal

11	D4	  P8_10	GPIO2_4	Data line (4-bit mode)

12	D5	  P8_11	GPIO1_13	Data line

13	D6	  P8_12	GPIO1_12	Data line

14	D7	  P8_14	GPIO0_26	Data line

15	LED+  (Backlight)	P9_7 via 220Ω	-	Optional

16	LED-  (Backlight)	P8_1, P9_1	-	Optional

*If you're not reading from the LCD, tie RW to GND.

## Hardware Setup
Connect LCD pins to BeagleBone Black as listed above.

Use a potentiometer (typically 10kΩ) for contrast between LCD V0 and GND/VCC.

Ensure LCD VCC receives 5V supply.

## For Python (lcd_user_python.py)
sudo apt-get update

sudo pip install Adafruit_BBIO

sudo python user_space/lcd_user_python.py

## Code Example (Summary)
These scripts/software use user-space GPIO libraries:

Python uses Adafruit_BBIO.GPIO for pin control

No special kernel modules or device tree overlays are required for these simple methods.

## Usage Notes
Run with root privileges to access GPIO.

If you change wiring, update the pin definitions in your script.

Wiring is critical: double-check connections before powering the circuit.
