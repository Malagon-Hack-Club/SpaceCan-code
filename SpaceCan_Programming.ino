#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "MQ7.h"
#include <Wire.h>
#include "BlueDot_BME280.h"
/*
   This sample code demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

BlueDot_BME280 bme1;
BlueDot_BME280 bme2;

int bme1Detected = 0;
int bme2Detected = 0;

MQ7 mq7(A0, 5.0);
int lectura_CO_anterior = 0;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(0, 1);

void setup()
{
  Serial.begin(9600);
  ss.begin(9600);

  bme1.parameter.communication = 0;
  bme2.parameter.communication = 0;

  bme1.parameter.I2CAddress = 0x77;
  bme2.parameter.I2CAddress = 0x76;

  bme1.parameter.sensorMode = 0b11;
  bme2.parameter.sensorMode = 0b11;


  bme1.parameter.IIRfilter = 0b100;                   //IIR Filter for Sensor 1
  bme2.parameter.IIRfilter = 0b100;


  bme1.parameter.humidOversampling = 0b101;            //Humidity Oversampling for Sensor 1
  bme2.parameter.humidOversampling = 0b101;

  bme1.parameter.tempOversampling = 0b101;              //Temperature Oversampling for Sensor 1
  bme2.parameter.tempOversampling = 0b101;              //Temperature Oversampling for Sensor 2


  bme1.parameter.pressOversampling = 0b101;             //Pressure Oversampling for Sensor 1
  bme2.parameter.pressOversampling = 0b101;

  bme1.parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 1)
  bme2.parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 2)

  bme1.parameter.tempOutsideCelsius = 15;               //default value of 15°C
  bme2.parameter.tempOutsideCelsius = 15;

  if (bme1.init() != 0x60)
  {
    Serial.println(F("Ops! First BME280 Sensor not found!"));
    bme1Detected = 0;
  }

  else
  {
    Serial.println(F("First BME280 Sensor detected!"));
    bme1Detected = 1;
  }

  if (bme2.init() != 0x60)
  {
    Serial.println(F("Ops! Second BME280 Sensor not found!"));
    bme2Detected = 0;
  }

  else
  {
    Serial.println(F("Second BME280 Sensor detected!"));
    bme2Detected = 1;
  }

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();

}

void loop() {
  if (bme1Detected)
  {
    Serial.print(bme1.readTempC());
    Serial.print(", ");
    Serial.print(bme1.readHumidity());
    Serial.print(", ");
    Serial.print(bme1.readPressure());
    Serial.print(", ");
  }

  else
  {

    Serial.println(F("****************************************"));
  }
  if (bme2Detected)
  {

    Serial.print(bme2.readTempC());
    Serial.print(", ");

    Serial.print(bme2.readHumidity());
    Serial.print(", ");
    Serial.print(bme2.readPressure());
    Serial.print(", ");
  }

  else
  {
    Serial.print(F("Temperature Sensor 2 [°C]:\t\t"));
    Serial.println(F("Null"));
  }

  Serial.print(mq7.getPPM());

  Serial.print(", ");

  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      if (gps.location.isUpdated()) {
        Serial.println(String(gps.location.lat() , 6) + " , " + String(gps.location.lng(), 6) + " , ");
      }
    }
  }
   
}
