/*
  Reading barometric pressure from the MS8607
  By: PaulZC
  Date: January 28th, 2020

  Based extensively on:
  Reading barometric pressure from the MS5637
  By: Nathan Seidle
  SparkFun Electronics
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  The original library and example code was written by TEConnectivity,
  the company that made the sensor. Way to go TE! May other companies
  learn from you.

  Feel like supporting open source hardware?
  Buy a board from SparkFun!

  This example shows how to detect a local altitude change. At power up the sensor will
  take a series of readings, average them, and use that average pressure as a baseline.
  Moving up or down a flight of stairs will show a change in altitude.
*/

#include <Wire.h>

#include <SparkFun_PHT_MS8607_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_PHT_MS8607

MS8607 barometricSensor;

float startingPressure = 0.0;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Qwiic PHT Sensor MS8607 Example");

  Wire.begin();

  if (barometricSensor.begin() == false)
  {
    Serial.println("MS8607 sensor did not respond. Trying again...");
    if (barometricSensor.begin() == false)
    {
      Serial.println("MS8607 sensor did not respond. Please check wiring.");
      while (1)
        ;
    }
  }

  //Set the resolution of the sensor to the highest level of resolution: 0.016 mbar
  barometricSensor.set_pressure_resolution(MS8607_pressure_resolution_osr_8192);

  //Take 16 readings and average them
  startingPressure = 0.0;
  for (int x = 0; x < 16; x++)
    startingPressure += barometricSensor.getPressure();
  startingPressure /= (float)16;

  Serial.print("Starting pressure=");
  Serial.print(startingPressure);
  Serial.println("hPa");
}

void loop(void)
{

  float currentPressure = barometricSensor.getPressure();

  Serial.print("Pressure=");
  Serial.print(currentPressure, 3);
  Serial.print("(hP or mbar)");

  float altitudeDelta = barometricSensor.altitudeChange(currentPressure, startingPressure);
  Serial.print(" Change in Altitude=");
  Serial.print(altitudeDelta, 1);
  Serial.print("m");

  float altitudeFeet = altitudeDelta * 3.28084;
  Serial.print("/");
  Serial.print(altitudeFeet, 1);
  Serial.print("ft");

  Serial.println();

  delay(500);
}
