# Water Level Detection 🚰

This project provides a simple system for monitoring water levels. It contains two main implementations:

1. **Blynk-Based Solution**  
   - Utilizes the Blynk IoT platform to display water level data remotely.
   - Allows users to monitor the water level via the Blynk mobile app or dashboard.
   - Ideal for users who want remote access and notifications.

2. **Arduino-Based Solution**  
   - Uses Arduino hardware and code for local water level detection.
   - Displays water level information through directly connected components (e.g., LEDs, LCD).
   - Suitable for standalone or offline monitoring.

## Files

- `water_level_blynk.ino`  
  Implementation using the Blynk platform.
- `water_level_arduino.ino`  
  Implementation using standard Arduino code.

## Getting Started

1. **For Blynk Implementation:**
   - Set up the Blynk app and obtain your authentication token.
   - Install the Blynk library in your Arduino IDE.
   - Upload `water_level_blynk.ino` to your Arduino board.
   - Configure your hardware according to the comments in the code.

2. **For Arduino-Only Implementation:**
   - Open `water_level_arduino.ino` in your Arduino IDE.
   - Upload the code to your Arduino board.
   - Connect the water level sensor and other components as described in the code.

## Hardware Requirements

- ESP8266
- Ultrasonic sensor
- Optional: Buzzer, LEDs, LCD display

## License

This project is open source and available for modification and distribution.

---

**Note:**  
- Choose the file that best suits your use-case: `water_level_blynk.ino` for remote IoT monitoring, or `water_level_arduino.ino` for local detection.
- For detailed wiring and setup instructions, refer to code comments in each file.
