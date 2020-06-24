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

  This example demonstrates how to use the alternate I2C ports on the Artemis family
*/

#include <Wire.h>

#include <SparkFun_PHT_MS8607_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_PHT_MS8607

//Define the Artemis TwoWire port for the MS8607
//See the Wire/Example2_MoreI2CPorts example for more details
//TwoWire myWire(1); //Will use Artemis pads 8/9
//TwoWire myWire(2); //Will use Artemis pads 27/25
TwoWire myWire(3); //Will use Artemis pads 42/43, SCL1/SDA1 on the Artemis Thing Plus
//TwoWire myWire(4); //Will use Artemis pads 39/40, Qwiic SCL/SDA on the Artemis ATP and Thing Plus
//TwoWire myWire(5); //Will use Artemis pads 48/49

//Create an instance of the object
MS8607 barometricSensor;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Qwiic PHT Sensor MS8607 Example");

  myWire.begin();

  if (barometricSensor.begin(myWire) == false)
  {
    Serial.println("MS8607 sensor did not respond. Trying again...");
    if (barometricSensor.begin(myWire) == false)
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

  Serial.print("Temperature=");
  Serial.print(temperature, 1);
  Serial.print("(C)");

  Serial.print(" Pressure=");
  Serial.print(pressure, 3);
  Serial.print("(hPa or mbar)");

  Serial.println();

  delay(500);
}
