#include "Wire.h"
#include "KellerLD.h"
#include "TSYS01.h"
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include <Adafruit_NeoPixel.h>

TSYS01 sensorTemperature;
KellerLD sensorPressure;
RTC_DS3231 rtc;

// SD card
const int chipSelect = 4;

// LED ring
#define PIN 11
#define NUMPIXELS 12
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  Serial.begin(115200);
  while (!Serial) { delay(1); }

  Serial.println("Starting...");

  // LEDs ON
  pixels.begin();
  pixels.setBrightness(255);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
  }
  pixels.show();

  Wire.begin();

  // Sensors
  sensorPressure.init();
  sensorPressure.setFluidDensity(1029);
  sensorTemperature.init();

  // RTC setup
  Serial.println("Initializing RTC...");
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // SD setup
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed!");
  } else {
    Serial.println("card initialized.");

    File dataFile = SD.open("datalog.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.println();
      dataFile.println("|| START OF NEW DATALOG ||");
      dataFile.println("Date, Time, Pressure (mbar), Temp (C), Depth (m)");
      dataFile.close();
    }
  }
}

void loop() {
  sensorPressure.read();
  sensorTemperature.read();

  DateTime now = rtc.now();

  float tempAvg = (sensorPressure.temperature() + sensorTemperature.temperature()) / 2;

  // -------- SERIAL OUTPUT --------
  Serial.println("----------- Sensor Readings -----------");

  Serial.print("Timestamp: ");
  Serial.print(now.year()); Serial.print('/');
  Serial.print(now.month()); Serial.print('/');
  Serial.print(now.day()); Serial.print(" ");
  int hour12 = now.hour() % 12;
  if (hour12 == 0) hour12 = 12;
  Serial.print(hour12);
  Serial.print(':');
  if (now.minute() < 10) Serial.print('0');
  Serial.print(now.minute());
  Serial.print(':');
  if (now.second() < 10) Serial.print('0');
  Serial.print(now.second());
  Serial.print(now.hour() >= 12 ? " PM" : " AM");

  Serial.println();

  Serial.print("Pressure: ");
  Serial.print(sensorPressure.pressure());
  Serial.println(" mbar");

  Serial.print("Depth: ");
  Serial.print(sensorPressure.depth());
  Serial.println(" m");

  Serial.print("Temperature: ");
  Serial.print(tempAvg);
  Serial.println(" C");

  Serial.println("--------------------------------------");

  // -------- SD LOGGING --------
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  if (dataFile) {

    // Date
    dataFile.print(now.year()); dataFile.print('/');
    dataFile.print(now.month()); dataFile.print('/');
    dataFile.print(now.day()); dataFile.print(", ");

    // Time
    dataFile.print(now.hour()); dataFile.print(':');
    if (now.minute() < 10) dataFile.print('0');
    dataFile.print(now.minute()); dataFile.print(':');
    if (now.second() < 10) dataFile.print('0');
    dataFile.print(now.second()); dataFile.print(", ");

    // Data
    dataFile.print(sensorPressure.pressure()); dataFile.print(", ");
    dataFile.print(tempAvg); dataFile.print(", ");
    dataFile.print(sensorPressure.depth());

    dataFile.println();
    dataFile.close();

    Serial.println("Data saved to SD.");
  } else {
    Serial.println("Error opening datalog.csv");
  }

  Serial.println();
  Serial.println();

  delay(3600); // 6 minutes
}