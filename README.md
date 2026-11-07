# Underwater Environmental Data Logger

An Arduino-based underwater environmental monitoring system built using the **Adafruit Feather M0 Adalogger**, a Keller pressure sensor, and a TSYS01 temperature sensor. The system collects pressure, temperature, and depth measurements and stores them on a microSD card for subsequent analysis.

Two versions of the data logger are provided: a basic version that tracks elapsed time since startup and an upgraded version that uses a DS3231 real-time clock (RTC) to record actual dates and times.

## Features

- Measures underwater pressure in millibars (mbar).
- Calculates depth in meters using a seawater density of 1029 kg/m³.
- Collects temperature readings from two independent sensors and calculates their average.
- Records measurements to a microSD card in CSV format.
- Displays live sensor measurements through the Arduino Serial Monitor.
- Operates a 12-pixel NeoPixel LED ring at full white brightness.
- Supports two timestamping methods:
  - **Basic:** Elapsed time since the microcontroller started.
  - **RTC:** Calendar date and time using a DS3231 clock.
- Designed for periodic, autonomous underwater environmental data collection.

## Hardware Components

| Component | Purpose |
|---|---|
| Adafruit Feather M0 Adalogger | Main microcontroller and SD card interface |
| Keller Pressure Sensor | Measures pressure, temperature, and calculates depth |
| TSYS01 Temperature Sensor | Provides an additional temperature measurement |
| DS3231 RTC Module | Maintains date and time (RTC version only) |
| microSD Card | Stores collected sensor data |
| 12-Pixel NeoPixel LED Ring | Provides visual illumination or indication |
| Power Supply | Powers the data logger during operation |

### Pin Configuration

| Connection | Pin |
|---|---|
| microSD Chip Select | 4 |
| NeoPixel Data | 11 |
| Keller Pressure Sensor | I²C (SDA/SCL) |
| TSYS01 Temperature Sensor | I²C (SDA/SCL) |
| DS3231 RTC | I²C (SDA/SCL) |

## Software Dependencies

The project uses the Arduino framework and the following libraries:

- `Wire.h` — I²C communication
- `KellerLD.h` — Keller pressure sensor interface
- `TSYS01
