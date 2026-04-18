#include "Wire.h"
#include "KellerLD.h"
#include "TSYS01.h"
#include <SPI.h>
#include <SD.h>

// [NEW] Include NeoPixel Library
#include <Adafruit_NeoPixel.h>

TSYS01 sensorTemperature;
KellerLD sensorPressure;

// Chip Select for Feather M0 Adalogger is 4
const int chipSelect = 4;

// [NEW] Define LED Ring Settings
#define PIN 11
#define NUMPIXELS 12
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  // [NEW] Initialize LEDs and turn them all on permanently
  pixels.begin();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
  }
  pixels.setBrightness(255);
  pixels.show();

  Serial.begin(9600);
  unsigned long start = millis();
  //while (!Serial && millis() - start < 3000) { delay(10); } 
  // ----------------------------------------------------------------
  // [CRITICAL FIX] WAIT FOR SERIAL MONITOR
  // This loop pauses the code until you open the Serial Monitor.
  // without this, the Feather M0 runs too fast and you miss the output.
  //COMMENT OUT WHEN TESTING (optional, data still written on csv file)
  // ----------------------------------------------------------------

  Serial.println("Starting...");

  Wire.begin();

  // Initialize Sensors
  sensorPressure.init();
  sensorPressure.setFluidDensity(1029); // Changed density to seawater
  sensorTemperature.init();

  // Initialize SD Card
  Serial.print("Initializing SD card...");
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // If the card fails, we usually want to know, but we continue 
    // so we can still see sensor data on the screen.
  } else {
    Serial.println("card initialized.");
    
    // Write Header
    File dataFile = SD.open("datalog.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.println();
      dataFile.println("                    ||     START OF NEW DATALOG    ||");
      dataFile.println("1. Uptime (seconds), 2. Pressure (millibars), 3. Temperature (*C),  4. Depth (meters)");
      dataFile.close();
      Serial.println("Header written to CSV.");
    }
  }
}

void loop() {
  
  sensorPressure.read();
  sensorTemperature.read();
  unsigned long currentMillis = millis();

  // --- SERIAL MONITOR OUTPUT ---
  Serial.println("------------------- Sensor Readings -------------------");

  Serial.print("Time (seconds since boot): ");
  Serial.print(currentMillis/1000);
  Serial.println();

  Serial.print("Pressure: ");
  Serial.print(sensorPressure.pressure());
  Serial.println(" mbar");

  Serial.print("Depth: ");
  Serial.print(sensorPressure.depth());
  Serial.println(" m");

  Serial.print("Temperature: ");
  Serial.print(((sensorPressure.temperature()+sensorTemperature.temperature())/2));
  Serial.println(" deg C");

   
  Serial.println("-------------------------------------------------------");

  // --- SD CARD LOGGING ---
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  if (dataFile) {
    //print timestamp
    dataFile.print("        ");
    dataFile.print(currentMillis/1000);
    dataFile.print(",                  "); 
    
    dataFile.print(sensorPressure.pressure());
    dataFile.print("mbar,             ");
    dataFile.print(((sensorPressure.temperature()+sensorTemperature.temperature())/2));
    dataFile.print("*C,             ");
    dataFile.print(sensorPressure.depth());
    dataFile.println("m");

    
    //create space in dataFile

    dataFile.close();
    Serial.println("Data saved to SD.");
  } else {
    Serial.println("Error: Could not open datalog.csv for writing!");
  }
  
  // Create space in Serial Monitor
  for (int i = 0; i < 3; i++){
      Serial.println(); 
  }

  delay(360000); // 360 second delay
}