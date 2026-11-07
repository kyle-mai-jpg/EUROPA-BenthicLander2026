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
- `TSYS01.h` — TSYS01 temperature sensor interface
- `SPI.h` — SPI communication
- `SD.h` — microSD card file operations
- `Adafruit_NeoPixel.h` — NeoPixel LED control
- `RTClib.h` — DS3231 real-time clock interface (RTC version only)

Install the required libraries and the Adafruit SAMD board support package before uploading either sketch.

## Program Versions

### Version 1: Basic Data Logger

The basic data logger records sensor measurements alongside the elapsed time since startup.

**Operation:**

1. Initializes the NeoPixel LED ring and turns all LEDs white.
2. Establishes serial communication at 9600 baud.
3. Initializes the Keller pressure and TSYS01 temperature sensors.
4. Configures the pressure sensor for seawater density.
5. Initializes the microSD card and creates or appends to `datalog.csv`.
6. Reads pressure, depth, and temperature measurements.
7. Calculates the average temperature from both sensors.
8. Displays the results through the Serial Monitor and saves them to the microSD card.
9. Waits six minutes before repeating the measurement cycle.

**Example CSV output:**

```text
1. Uptime (seconds), 2. Pressure (millibars), 3. Temperature (*C),  4. Depth (meters)
360, 2013.25mbar, 18.45*C, 10.02m
720, 2013.30mbar, 18.46*C, 10.03m
```

The actual output includes additional spaces and session-header lines.

**Limitation:** Because elapsed time is measured using `millis()`, the timestamp resets when the microcontroller restarts.

### Version 2: RTC Data Logger

The RTC version extends the original data logger by integrating a DS3231 real-time clock.

Instead of measuring elapsed time since startup, the RTC provides a calendar date and time for each measurement.

**Additional functionality:**

- Initializes and verifies communication with the DS3231 RTC.
- Checks whether the RTC has lost power.
- Initializes the clock using the sketch's compilation date and time if power loss is detected.
- Records timestamps alongside pressure, temperature, and depth measurements.
- Displays timestamps in 12-hour format on the Serial Monitor.
- Stores timestamps in 24-hour format on the microSD card.

**Example CSV output:**

```csv
Date, Time, Pressure (mbar), Temp (C), Depth (m)
2026/11/6, 14:30:00, 2013.25, 18.45, 10.02
2026/11/6, 14:36:00, 2013.30, 18.46, 10.03
```

These are illustrative measurements at the intended six-minute interval.

**Advantage:** The DS3231 RTC maintains time independently of the microcontroller, allowing measurements to be associated with real-world dates and times across system restarts, provided the clock remains powered and correctly set.

## Measurement Calculations

### Temperature

The system obtains temperature measurements from both the Keller pressure sensor and the TSYS01 temperature sensor.

The recorded temperature is calculated as:

\[
T_{\text{avg}}=\frac{T_{\text{Keller}}+T_{\text{TSYS01}}}{2}
\]

This provides a simple average of the two sensor readings.

### Depth

The Keller sensor library calculates depth using pressure measurements and the configured fluid density.

The density is set to:

```cpp
sensorPressure.setFluidDensity(1029);
```

This represents an assumed seawater density of 1029 kg/m³.

Depth calculations depend on the pressure sensor library's handling of reference atmospheric pressure and the relationship between hydrostatic pressure and depth.

### Data Collection Interval

The intended interval between measurement cycles is six minutes:

```cpp
delay(360000);
```

This is equivalent to 360,000 milliseconds.

Actual time between measurements also includes sensor-reading, serial-output, and SD-card-writing operations.

## Getting Started

1. Install the Arduino IDE and the appropriate Adafruit Feather M0 board support package.
2. Install the required Arduino libraries.
3. Connect the pressure sensor, temperature sensor, NeoPixel ring, and optional RTC module.
4. Insert a compatible microSD card formatted for use with the Arduino SD library.
5. Connect the Feather M0 to a computer.
6. Select the correct board and serial port in the Arduino IDE.
7. Open and upload the desired data logger sketch.
8. Open the Serial Monitor at the corresponding baud rate:
   - Basic version: `9600`
   - RTC version: `115200`
9. Verify that the sensors initialize and that data is successfully written to `datalog.csv`.

For autonomous operation, ensure the sketch does not wait indefinitely for a USB Serial Monitor connection.

## Known Limitations and Improvements

- **RTC logging interval:** The supplied RTC sketch currently uses `delay(3600)`, corresponding to 3.6 seconds rather than six minutes. Change this to `delay(360000)` for the intended interval.
- **Serial dependency:** The RTC sketch waits indefinitely for a Serial connection during startup. This should be removed or replaced with a timeout before standalone deployment.
- **CSV formatting:** Session markers and extra formatting may require preprocessing before importing data into analysis tools.
- **Clock initialization:** After RTC power loss, the clock is set to the sketch compilation time, which may differ from the actual current time.
- **Error handling:** Additional checks could detect sensor initialization failures, invalid readings, and SD card write errors.
- **Power consumption:** Keeping all NeoPixels at full white brightness continuously increases power consumption.
- **Timing precision:** Blocking delays do not guarantee an exact six-minute sampling interval.

## Applications

This project can be used as a foundation for:

- Underwater temperature monitoring
- Water pressure and depth measurements
- Marine environmental data collection
- Long-duration sensor logging
- Time-series analysis of underwater conditions
- Prototyping autonomous oceanographic instrumentation

## Future Development

Potential improvements include:

- Non-blocking sampling using `millis()` or RTC-based scheduling.
- Improved sensor initialization and error detection.
- Lower-power LED behavior for battery-powered deployments.
- Consistent CSV formatting with standardized timestamps.
- Additional environmental sensors.
- Automated data processing and visualization.

## Project Summary

This project demonstrates the integration of multiple I²C sensors, microSD data storage, real-time clock functionality, and LED control within an embedded environmental monitoring system.

The RTC-based implementation extends the basic logger by providing persistent real-world timestamps, making the recorded measurements more useful for environmental monitoring and time-series analysis.
