#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <ThreeWire.h> // Includes the three wire library to create an object with three wires
#include <RtcDS1302.h> // Includes the library to use the DS1302

extern ThreeWire myWire; // Declare an external three wire type object
extern RtcDS1302<ThreeWire> Rtc; // Declare an external RTC object

#define TIMESTAMP_DATA_PIN 26 // Define the data pin for the timestamp
#define TIMESTAMP_RESET_PIN 27 // Define the reset pin for the timestamp
#define TIMESTAMP_CLOCK_PIN 32 // Define the clock pin for the timestamp

void setupTimestamp(); // Function declaration for setting up the timestamp
String Get_Date_Time(); // Function declaration for getting the date and time
String Get_Date_Time_File_Name();

#endif
