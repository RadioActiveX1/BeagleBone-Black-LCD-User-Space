import Adafruit_BBIO.GPIO as GPIO
import time

# LCD GPIO pin mapping
LCD_RS = "P8_7"
LCD_RW = "P8_8"    # Tie to GND if using write-only
LCD_EN = "P8_9"
LCD_D4 = "P8_10"
LCD_D5 = "P8_11"
LCD_D6 = "P8_12"
LCD_D7 = "P8_14"

# Setup pins as output
pins = [LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7]
for pin in pins:
    GPIO.setup(pin, GPIO.OUT)
GPIO.setup(LCD_RW, GPIO.OUT)
GPIO.output(LCD_RW, GPIO.LOW)   # Always Write mode

def pulse_e():
    GPIO.output(LCD_EN, GPIO.HIGH)
    time.sleep(0.001)
    GPIO.output(LCD_EN, GPIO.LOW)
    time.sleep(0.001)

def send_4bits(bits):
    GPIO.output(LCD_D4, GPIO.HIGH if bits & 0x1 else GPIO.LOW)
    GPIO.output(LCD_D5, GPIO.HIGH if bits & 0x2 else GPIO.LOW)
    GPIO.output(LCD_D6, GPIO.HIGH if bits & 0x4 else GPIO.LOW)
    GPIO.output(LCD_D7, GPIO.HIGH if bits & 0x8 else GPIO.LOW)
    pulse_e()

def send_byte(data, mode):
    GPIO.output(LCD_RS, GPIO.HIGH if mode else GPIO.LOW)
    send_4bits((data >> 4) & 0x0F)
    send_4bits(data & 0x0F)
    time.sleep(0.001)

def lcd_init():
    send_4bits(0x03)
    time.sleep(0.005)
    send_4bits(0x03)
    time.sleep(0.002)
    send_4bits(0x03)
    send_4bits(0x02)         # Set 4-bit mode

    send_byte(0x28, 0)       # 2 line, 5x8 matrix
    send_byte(0x0C, 0)       # Display on, cursor off
    send_byte(0x06, 0)       # Entry mode set
    send_byte(0x01, 0)       # Clear display
    time.sleep(0.002)

def lcd_write(msg):
    for char in msg:
        send_byte(ord(char), 1)

def lcd_goto(row, col):
    # LCD rows: 0 or 1
    addr = col + (0x40 if row else 0x00)
    send_byte(0x80 | addr, 0)

if __name__ == "__main__":
    lcd_init()
    lcd_write("Hello, BBB!")
    lcd_goto(1,0)
    lcd_write("16x2 LCD Demo")
