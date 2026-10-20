# Wiring Notes

These connections are transcribed from the supplied project report. Check the pinout of your specific modules and boards before powering the circuit.

## MPU6050 to Arduino UNO

| MPU6050 | Arduino UNO |
|---|---|
| SDA | A4 |
| SCL | A5 |
| GND | GND |
| VCC | Use the voltage supported by your specific breakout board |

## NEO-6M GPS to Arduino UNO

| GPS module | Arduino UNO |
|---|---|
| TX | D3 (SoftwareSerial RX) |
| RX | D4 (SoftwareSerial TX) |
| GND | GND |
| VCC | Use the voltage supported by your specific GPS board |

The firmware uses `SoftwareSerial gpsSerial(3, 4)`, so Arduino pin 3 is receive and pin 4 is transmit.

## Arduino UNO to ESP32

- Arduino serial TX is intended to feed ESP32 GPIO16 (RX2), according to the report.
- Connect grounds together.
- Confirm voltage compatibility before connecting the Arduino UNO's 5 V TX signal to an ESP32 input; use appropriate level shifting where needed.
- The ESP32 code initializes Serial2 at 9600 baud with RX2 on GPIO16 and TX2 on GPIO17.

## Power

Use a suitable stable supply and verify each module's voltage requirements. Do not assume every breakout board accepts 5 V on VCC.
