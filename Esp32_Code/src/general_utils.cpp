#include <general_utils.h>

// Function to blink an LED
void blink_led(unsigned int times, unsigned int duration){
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH); // Turn on the LED
    delay(duration); // Wait for the specified duration
    digitalWrite(LED_PIN, LOW); // Turn off the LED
    delay(200); // Wait for 200 milliseconds before the next iteration
  }
}

// Function to format a message string
String formatMessage(String batteryVoltage, 
                     String batteryTemperature, 
                     String pressure, 
                     String temperature, 
                     String flow, 
                     String timestamp ){

  String formattedMessage = "{"; // Start the message string with an opening brace
  String mqtt_name("esp32/1"); // MQTT name

  formattedMessage += "\"Id\": "+ mqtt_name + ','; // Add MQTT name to the message as an "Id" field
  formattedMessage += "\"BateryVoltage\": "+ batteryVoltage + ','; // Add battery voltage to the message as a "BatteryVoltage" field
  formattedMessage += "\"BateryTemperature\": "+ batteryTemperature + ','; // Add battery temperature to the message as a "BatteryTemperature" field
  formattedMessage += "\"FurnacePressure\": "+ pressure + ','; // Add furnace pressure to the message as a "FurnacePressure" field
  formattedMessage += "\"FurnaceTemperature\": "+ temperature + ','; // Add furnace temperature to the message as a "FurnaceTemperature" field
  formattedMessage += "\"FurnaceFlow\": "+ flow + ','; // Add furnace flow to the message as a "FurnaceFlow" field   
  formattedMessage += "\"Timestamp\": "+ timestamp; // Add timestamp to the message as a "Timestamp" field
  formattedMessage += "}"; // End the message string with a closing brace

  return formattedMessage; // Return the formatted message
}
