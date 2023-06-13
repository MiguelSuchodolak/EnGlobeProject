#ifndef SENSORS_H_
#define SENSORS_H_

#include <Adafruit_INA219.h> // Includes the Adafruit INA219 library
#include <Wire.h> // Includes the Wire library for I2C communication
#include <string> // Includes the string library for string operations

#define R1 100600.0 // Value of resistor R1
#define R2 21963.0 // Value of resistor R2
#define VCC 3.3 // Supply voltage

#define BATTERY_VOLTAGE_PIN 39 // Pin for reading battery voltage
#define BATTERY_TEMPERATURE_PIN 34 // Pin for reading battery temperature

const uint8_t PressureAddress = 0x40; // I2C address of the pressure sensor
const uint8_t TemperatureAddress = 0x44; // I2C address of the temperature sensor

extern Adafruit_INA219 inaTemp; // Declare an external Adafruit INA219 object for temperature
extern Adafruit_INA219 inaPress; // Declare an external Adafruit INA219 object for pressure

void setupSensors(); // Function declaration for setting up the sensors

String readBatteryVoltage(); // Function declaration for reading the battery voltage

String readBatteryTemperature(); // Function declaration for reading the battery temperature

String readFurnaceTemperature(); // Function declaration for reading the furnace temperature

String readFurnacePressure(); // Function declaration for reading the furnace pressure

#endif
