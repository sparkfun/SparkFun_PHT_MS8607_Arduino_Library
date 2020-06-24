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

  This example shows how to take the absolute pressure from the sensor and convert it to
  sea level adjusted pressure, or the pressure that you would see from a weather underground station
  (or the pressure you would post to your own weather underground station).

  See "Mean Sea Level Pressure": https://en.wikipedia.org/wiki/Atmospheric_pressure#Mean_sea_level_pressure

  For this example you'll need to know the altitude of your workbench. We used
  https://www.freemaptools.com/elevation-finder.htm and estimated our local alittude at: 5373.6ft or 1637.9m

  Then change the localAltitude variable to be your local altitude. Then load this code
  onto your RedBoard or Uno. Open the terminal at 9600bps to see the various data. The final number
  is called "Weather Pressure" in inHg (inches Mercury).

  Next search the internet for a weather station near your home with roughly the same altitude. The
  pressure on your desk should be approximately the same pressure as reported by the local weather station.
  For example, the weather in Boulder, CO: https://www.wunderground.com/weather/us/co/boulder
  reported "30.01 in" as pressure and the sensor "Weather Pressure" was 30.027

  Now you can build your own weather station and report the pressure at your house
*/

#include <Wire.h>

#include <SparkFun_PHT_MS8607_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_PHT_MS8607

MS8607 barometricSensor;

const float localAltitude = 1637.9; //Estimated local altitude 5373.6ft, 1637.9m from https://www.freemaptools.com/elevation-finder.htm

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Qwiic PHT Sensor MS8607 - Weather Pressure Conversion Example");

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
}

void loop(void)
{

  float temperature = barometricSensor.getTemperature();
  float pressure = barometricSensor.getPressure();

  Serial.print(" Temperature=");
  Serial.print(temperature, 1);
  Serial.print("(C)");

  Serial.print(" Pressure=");
  Serial.print(pressure, 3);
  Serial.print("(hPa or mbar)");

  //float inHg = pressure * 0.0295301; //32 degrees F
  float inHg = pressure * 0.029613; //60 degrees F
  Serial.print(" Pressure=");
  Serial.print(inHg, 3);
  Serial.print("(inHg)");

  //Convert the current pressure to sea-level corrected pressure
  float adjustedSeaLevel = barometricSensor.adjustToSeaLevel(pressure, localAltitude);
  Serial.print(" Weather Pressure=");
  Serial.print(adjustedSeaLevel, 3);
  Serial.print("(hPa)");

  inHg = adjustedSeaLevel * 0.029529980164712;
  Serial.print(" Weather Pressure=");
  Serial.print(inHg, 3);
  Serial.print("(inHg)");

  Serial.println();

  delay(500);
}
