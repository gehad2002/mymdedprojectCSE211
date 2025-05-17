#include "mbed.h"

// Define pins for 7-segment display control (for multifunction shield)
DigitalOut latch(D4);    // Latch pin (ST_CP)
DigitalOut clockPin(D7); // Clock pin (SH_CP)
DigitalOut data(D8);     // Data pin (DS)

// Define switches
DigitalIn S1(A1);        // S1 switch (reset button)
DigitalIn S3(A3);        // S3 switch (display toggle)

// Define ADC pin for potentiometer
AnalogIn pot(A0);        // Potentiometer input on analog pin A0

Timer timer;
Ticker displayTicker;    // Ticker for refreshing display

// 7-segment display maps (PRE-INVERTED for common anode)
const char SEGMENT_MAP[10] = {
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90  // 9
};

// Digit select patterns
const char DIGIT_SELECT[4] = { 0x08, 0x04, 0x02, 0x01 };

int minutes = 0, seconds = 0;
int current_digit = 0;

void shiftOut(char dataOut) {
    for (int i = 7; i >= 0; i--) {
        clockPin = 0;
        data = (dataOut >> i) & 0x01;
        clockPin = 1;
    }
}

// Called every few ms to update display
void updateDisplay() {
    // Check the state of S3 to determine whether to display time or voltage
    if (S3.read() == 0) { // If S3 is pressed
        // Read the potentiometer value (0.0 to 1.0) and convert to voltage
        float voltage = pot.read() * 3.3; // Assuming 3.3V reference voltage

        // Extract the digits for displaying voltage (in tenths)
        int volt_digits[4] = {
            (int)(voltage * 10) % 10,    // rightmost digit (tenths)
            (int)(voltage * 100) / 10 % 10, // second digit (hundredths)
            (int)voltage % 10,            // third digit (whole volts)
            (int)(voltage * 10) / 10      // leftmost digit (volts)
        };

        latch = 0;
        shiftOut(SEGMENT_MAP[volt_digits[current_digit]]);
        shiftOut(DIGIT_SELECT[current_digit]);
        latch = 1;

        current_digit = (current_digit + 1) % 4; // Move to next digit
    } else {
        // Normal time display
        int elapsed = (int)timer.read();
        minutes = elapsed / 60;
        seconds = elapsed % 60;

        // Flip the digits: seconds first, then minutes
        int digits[4] = {
            seconds % 10,    // rightmost
            seconds / 10,    // second
            minutes % 10,    // third
            minutes / 10     // leftmost
        };

        latch = 0;
        shiftOut(SEGMENT_MAP[digits[current_digit]]);
        shiftOut(DIGIT_SELECT[current_digit]);
        latch = 1;

        current_digit = (current_digit + 1) % 4; // Move to next digit
    }
}

int main() {
    S1.mode(PullUp); // Internal pull-up resistor for reset button
    S3.mode(PullUp); // Internal pull-up resistor for display toggle switch
    timer.start();
    displayTicker.attach_us(&updateDisplay, 3000); // update every 3ms

    while (true) {
        if (S1.read() == 0) { // Button pressed (LOW)
            timer.reset();   // Reset the timer immediately
            minutes = 0;     // Reset minutes and seconds display
            seconds = 0;
            wait_us(500000); // Adding a small delay to avoid multiple reads
        }
        // No blocking; everything runs smoothly
    }
}
