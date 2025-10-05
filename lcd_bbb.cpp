#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <cstdlib>

using namespace std;

#define LCD_RS 66   // Example: P8_7 → GPIO66
#define LCD_RW 67   // Example: P8_8 → GPIO67
#define LCD_EN 69   // Example: P8_9 → GPIO69
#define LCD_D4 68   // Example: P8_10 → GPIO68
#define LCD_D5 45   // Example: P8_11 → GPIO45
#define LCD_D6 44   // Example: P8_12 → GPIO44
#define LCD_D7 26   // Example: P8_14 → GPIO26


void gpio_export(int pin) {
    ofstream exportFile("/sys/class/gpio/export");
    exportFile << pin;
    exportFile.close();
    usleep(100000);
}

// Set direction
void gpio_direction(int pin, string dir) {
    ofstream dirFile("/sys/class/gpio/gpio" + to_string(pin) + "/direction");
    dirFile << dir;
    dirFile.close();
}

// Write value
void gpio_write(int pin, int value) {
    ofstream valFile("/sys/class/gpio/gpio" + to_string(pin) + "/value");
    valFile << value;
    valFile.close();
}

// Pulse enable pin
void pulse_e() {
    gpio_write(LCD_EN, 1);
    usleep(1000);
    gpio_write(LCD_EN, 0);
    usleep(1000);
}

// Send 4 bits
void send_4bits(int bits) {
    gpio_write(LCD_D4, (bits & 0x1) ? 1 : 0);
    gpio_write(LCD_D5, (bits & 0x2) ? 1 : 0);
    gpio_write(LCD_D6, (bits & 0x4) ? 1 : 0);
    gpio_write(LCD_D7, (bits & 0x8) ? 1 : 0);
    pulse_e();
}

// Send byte to LCD
void send_byte(int data, bool mode) {
    gpio_write(LCD_RS, mode ? 1 : 0);
    send_4bits((data >> 4) & 0x0F);
    send_4bits(data & 0x0F);
    usleep(1000);
}

// LCD init sequence
void lcd_init() {
    send_4bits(0x03);
    usleep(5000);
    send_4bits(0x03);
    usleep(2000);
    send_4bits(0x03);
    send_4bits(0x02);   // 4-bit mode

    send_byte(0x28, false);  // 2 line, 5x8 font
    send_byte(0x0C, false);  // Display ON, cursor OFF
    send_byte(0x06, false);  // Increment cursor
    send_byte(0x01, false);  // Clear display
    usleep(2000);
}

// Write string to LCD
void lcd_write(string msg) {
    for (char c : msg) {
        send_byte((int)c, true);
    }
}

// Move cursor
void lcd_goto(int row, int col) {
    int addr = col + (row ? 0x40 : 0x00);
    send_byte(0x80 | addr, false);
}

int main() {
    // Export pins
    int pins[] = {LCD_RS, LCD_RW, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7};
    for (int pin : pins) {
        gpio_export(pin);
        gpio_direction(pin, "out");
    }

    // Always write mode
    gpio_write(LCD_RW, 0);

    lcd_init();
    lcd_write("Hello, BBB!");
    lcd_goto(1, 0);
    lcd_write("16x2 LCD Demo");

    return 0;
}
