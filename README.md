# Portable Luggage Monitor and Tracker

An IoT-based luggage monitoring prototype that combines GPS location reporting with motion-based tamper indication. The prototype uses an Arduino UNO to read a NEO-6M GPS module and MPU6050 IMU, then sends a status line over UART to an ESP32. The ESP32 connects to Wi-Fi and serves a browser dashboard showing location, status, and a map.

> **Project status:** Academic prototype. The firmware is based on the accompanying project report and should be checked against your exact wiring and library versions before use.

## Features

- GPS latitude and longitude using the NEO-6M module
- Tilt-based movement/tamper indication using MPU6050 acceleration readings
- Arduino UNO to ESP32 serial communication
- ESP32-hosted web page with location, status, and embedded map
- Refreshing dashboard for local-network monitoring

## Hardware

- Arduino UNO
- ESP32 Wi-Fi development board
- NEO-6M GPS module with antenna
- MPU6050 accelerometer/gyroscope module
- Breadboard, jumper wires, and suitable power supply

## Software

- Arduino IDE
- C++
- TinyGPSPlus
- SoftwareSerial
- Wire (I²C)
- MPU6050 library compatible with the included API
- ESP32 Wi-Fi and WebServer libraries

## System overview

1. The NEO-6M receives satellite data.
2. The Arduino UNO parses GPS data and reads MPU6050 acceleration values.
3. The Arduino formats coordinates, date/time when available, and a simple tilt-based status.
4. The data is sent over serial/UART to the ESP32.
5. The ESP32 hosts a web page that displays the latest values and an embedded map when coordinates are available.

See [docs/architecture.md](docs/architecture.md) for the high-level data flow and [docs/wiring.md](docs/wiring.md) for connections transcribed from the report.

## Prototype demonstration

The project demonstration video supplied with this repository discussion shows the assembled electronics and the mobile browser interface. The visible interface includes GPS/location and status panels, along with a map area. This is presented as a demonstration of the prototype, not as a benchmark or independent validation of tracking accuracy.

**Media note:** The demonstration video is not currently hosted in this GitHub repository. Add a shareable video link or upload approved still images to the `images/` folder to display the evidence directly on this page.

## Repository layout

- `firmware/arduino-uno/` — Arduino UNO firmware
- `firmware/esp32/` — ESP32 dashboard firmware
- `docs/` — architecture and wiring notes
- `PATENT.md` — application details from the supplied status record

## Setup

1. Install the Arduino IDE and the required libraries listed above.
2. Open `firmware/arduino-uno/luggage_tracker_arduino.ino` and upload it to the Arduino UNO.
3. Open `firmware/esp32/luggage_tracker_esp32.ino` and configure your Wi-Fi credentials locally in the marked section. **Do not commit real credentials.**
4. Confirm the UART wiring and baud rate match your board. Check the wiring guide and voltage compatibility before powering the circuit.
5. Upload the ESP32 firmware and open the local IP address printed in the ESP32 serial monitor from a device on the same Wi-Fi network.

## Important implementation notes

- The tilt threshold in the Arduino code is a simple demonstration heuristic, not a validated security detector. Tune and test it for the enclosure and use case.
- GPS reception can be unreliable indoors and may take time to obtain a fix.
- The dashboard is served on the local Wi-Fi network; this implementation does not provide internet-wide access, encrypted transport, or authenticated access.
- Check the board's UART voltage levels and wiring before connecting boards. Use a suitable level shifter or interface where required.
- Date/time handling and serial parsing should be validated on the actual hardware.
- The firmware in this repository has not been independently compiled or retested on the physical boards as part of this documentation update.

## Patent application

The supplied Intellectual Property India status document lists application number **202641049368**, title **Portable Luggage Monitor and Tracker**, publication date **1 May 2026**, and status **Application Awaiting Examination**. See [PATENT.md](PATENT.md).

## Credits

Academic project report submitted for the course BECE204L — Microprocessors and Microcontrollers, School of Electronics Engineering, Vellore Institute of Technology, Chennai. Contributors are listed in the original report.

---

This repository documents an academic prototype and is not a commercial-grade security product.
