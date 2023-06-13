#ifndef UTILS_H_
#define UTILS_H_

#include <Arduino.h> // Includes the Arduino framework

#define LED_PIN 2 // Defines the pin number for the LED
#define INTERRUPTION_PIN 12 // Defines the pin number for the interruption
#define TIMEOUT_INTERRUPTION 250 // Defines the timeout for the interruption

void blink_led(unsigned int times, unsigned int duration);
// Function to blink the LED
// Parameters:
// - times: The number of times the LED should blink
// - duration: The duration of each blink

String formatMessage(String batteryVoltage, 
                     String batteryTemperature, 
                     String pressure, 
                     String temperature, 
                     String timestamp );
// Function to format a message with sensor readings and timestamp
// Parameters:
// - batteryVoltage: The voltage of the battery
// - batteryTemperature: The temperature of the battery
// - pressure: The pressure reading
// - temperature: The temperature reading
// - flow: The flow reading
// - timestamp: The timestamp of the measurement

#endif
