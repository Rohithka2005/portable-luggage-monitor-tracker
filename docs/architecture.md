# System Architecture

The prototype described in the project report uses the following data path:

```text
NEO-6M GPS ──serial──> Arduino UNO <──I²C── MPU6050
                            │
                            │ UART / serial record
                            ▼
                          ESP32 ── Wi-Fi ──> Browser dashboard
                                             ├─ Coordinates
                                             ├─ Motion status
                                             └─ Embedded map
```

## Data flow

1. The GPS module supplies location and UTC date/time data.
2. The Arduino parses GPS data and samples acceleration from the MPU6050.
3. A simple angle threshold labels the luggage as `Secured` or `Tampered`.
4. The Arduino sends a newline-terminated status record over serial.
5. The ESP32 parses the record and serves a local web dashboard.

This is a high-level description based on the supplied academic report. It does not imply independent validation of accuracy, security, or reliability.
