/*
  Reading barometric pressure from the MS8607
  By: PaulZC
  Date: November 28th, 2019

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

  This example is used to test production units.
*/

#include <Wire.h>

#include <SparkFun_PHT_MS8607_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_PHT_MS8607

MS8607 barometricSensor;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Begin");

  Wire.begin();

  barometricSensor.begin(Wire);
}

void loop(void)
{

  if (barometricSensor.isConnected())
  {
    Serial.print("Good");

    float temperature = barometricSensor.getTemperature();
    float pressure = barometricSensor.getPressure();

    Serial.print(" Temp=");
    Serial.print(temperature, 1);
    Serial.print("(C)");

    Serial.print(" Press=");
    Serial.print(pressure, 3);
    Serial.print("(hPa)");

    Serial.println();
  }
  else
  {
    Serial.println("Not connected");
  }
  delay(500);
}
