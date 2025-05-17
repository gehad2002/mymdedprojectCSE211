Real-Time Clock and Voltage Display using NUCLEO-F401RE
Overview
This project implements a basic real-time clock (RTC) system with analog voltage monitoring, displayed on a 4-digit 7-segment display. It uses the STM32 NUCLEO-F401RE development board paired with an Arduino Multifunction Shield. The system was developed as part of the CSE211s – Introduction to Embedded Systems course at Ain Shams University.

The system continuously tracks time in minutes and seconds. A built-in potentiometer allows the user to observe analog voltage readings when prompted. The user can switch between modes in real time without interrupting the system's functionality.

Features
Real-time clock starts from zero on power-up or reset

Reset switch (S1) restarts the clock at any time

Toggle switch (S3) displays the potentiometer’s voltage reading (0.00V to 3.30V)

Timer continues running even while voltage is being displayed

Smooth 4-digit 7-segment display via multiplexing using a shift register

ADC integration for real-time voltage input display

Hardware Requirements
STM32 NUCLEO-F401RE development board

Arduino Multifunction Shield

Onboard potentiometer (connected to analog pin A0)

Push-buttons S1 (A1) and S3 (A3)

4-digit 7-segment display (common anode)

Pins used for display control:

D4: Latch (ST_CP)

D7: Clock (SH_CP)

D8: Data (DS)

How It Works
Timer Functionality
Upon powering the system, the timer begins tracking elapsed time in seconds and minutes. The display is refreshed every 3 milliseconds using a Ticker interrupt to multiplex all four digits without visible flicker.

Voltage Reading Mode
Pressing S3 causes the system to temporarily switch to displaying the voltage read from the onboard potentiometer. The voltage is sampled through the ADC and scaled according to a 3.3V reference. When S3 is released, the display returns to showing the current time. The timer continues uninterrupted in the background.

Reset Behavior
Pressing S1 resets the timer and returns the display to 00:00. A short debounce delay prevents unintentional multiple triggers.

File Structure
plaintext
Copy
Edit
/project-root
 ├── main.cpp        // Main source code with full implementation and comments
 ├── README.md       // Project documentation
 └── /media          // Optional: Demo video or related images
Setup Instructions
Clone the repository:

bash
Copy
Edit
git clone https://github.com/your-username/rtc-voltage-display.git
cd rtc-voltage-display
Open the project in Mbed Studio, Keil uVision, STM32CubeIDE, or another compatible IDE.

Connect the pins as specified in the hardware section.

Build and flash the project to the NUCLEO-F401RE board.

Upon successful upload, the 7-segment display should begin counting. Press S1 to reset the timer and S3 to temporarily show the voltage reading from the potentiometer.

Demonstration
A short video demonstrating the project in operation is available in the /media folder or via the provided link.

License
This project is provided for educational and academic purposes. Contributions and enhancements are welcome with proper attribution.