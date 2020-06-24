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

  This example shows:
    How to change the measurement resolution
    How to use a different wire port
    How to communicate at 400kHz I2C
    Take rolling average across 8 readings
*/

#include <Wire.h>

#include <SparkFun_PHT_MS8607_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_PHT_MS8607

MS8607 barometricSensor;

//Store distance readings to get rolling average
#define HISTORY_SIZE 8
float history[HISTORY_SIZE];
byte historySpot;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Qwiic PHT Sensor MS8607 Example");

  //If your platform has multiple Wire ports you can pass the port to
  //the library and the sensor will communicate on it
  //Wire1.begin();
  //Wire1.setClock(400000); //Communicate at faster 400kHz I2C
  //if (barometricSensor.begin(Wire1) == false)
  //{
  //  Serial.println("MS8607 sensor did not respond. Trying again...");
  //  if (barometricSensor.begin(Wire1) == false)
  //  {
  //    Serial.println("MS8607 sensor did not respond. Please check wiring.");
  //    while(1)
  //      ;
  //  }
  //}

  //Default is Wire and is optional when you call .begin()
  Wire.begin();
  Wire.setClock(400000); //Communicate at faster 400kHz I2C
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

  //The sensor has 6 resolution levels. The higher the resolution the longer each
  //reading takes to complete.
  //  barometricSensor.set_pressure_resolution(MS8607_pressure_resolution_osr_256); //1ms per reading, 0.11mbar resolution
  //  barometricSensor.set_pressure_resolution(MS8607_pressure_resolution_osr_512); //2ms per reading, 0.062mbar resolution
  //  barometricSensor.set_pressure_resolution(MS8607_pressure_resolution_osr_1024); //3ms per reading, 0.039mbar resolution
  //  barometricSensor.set_pressure_resolution(MS8607_pressure_resolution_osr_2048); //5ms per reading, 0.028mbar resolution
  //  barometricSensor.set_pressure_resolution(MS8607_pressure_resolution_osr_4096); //9ms per reading, 0.021mbar resolution
  barometricSensor.set_pressure_resolution(MS8607_pressure_resolution_osr_8192); //17ms per reading, 0.016mbar resolution
}

void loop(void)
{

  float temperature = barometricSensor.getTemperature();
  float pressure = barometricSensor.getPressure();

  history[historySpot] = pressure;
  if (historySpot++ == HISTORY_SIZE)
    historySpot = 0;

  float avgPressure = 0.0;
  for (int x = 0; x < HISTORY_SIZE; x++)
    avgPressure += history[x];
  avgPressure /= (float)HISTORY_SIZE;

  Serial.print("Temperature=");
  Serial.print(temperature, 1);
  Serial.print("(C)");

  Serial.print(" Pressure=");
  Serial.print(avgPressure, 3);
  Serial.print("(hPa or mbar)");

  Serial.println();

  delay(500);
}
