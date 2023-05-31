#include <main.h>

// Variables
int last_msg = 0; // Timestamp of the last message sent
int interruption_counter = 0; // Counter for the number of interruptions
int last_interrupt = 0; // Timestamp of the last interrupt

int FLAG_INTERRUPT = 0; // Flag to indicate if an interrupt occurred

// Interrupt Service Routine for the test interrupt
void IRAM_ATTR interrupt_test(){
  if ( (millis() - last_interrupt) >= TIMEOUT_INTERRUPTION ){
    interruption_counter++; // Increment the interruption counter
    last_interrupt = millis(); // Update the timestamp of the last interrupt
    FLAG_INTERRUPT = 1; // Set the interrupt flag to indicate an interrupt occurred
  }
}
 
void setup() {
  Serial.begin(115200); // Initialize the serial communication
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output
  pinMode(INTERRUPTION_PIN, INPUT_PULLUP); // Set the interruption pin as an input with internal pull-up resistor
  attachInterrupt(INTERRUPTION_PIN, interrupt_test, RISING); // Attach the interrupt_test function to the interrupt pin for rising edge detection
  setupWifiAndMqttClient(); // Setup Wi-Fi and MQTT client
  setupTimestamp(); // Setup timestamp functionality
  //setupSensors(); // Setup sensor functionality (commented out)
}

void loop() {
  if ( FLAG_INTERRUPT == 1 ) { // Check if an interrupt occurred
    // Format the message using sensor readings and timestamp
    String message = formatMessage( readBatteryVoltage(), 
                                    readBatteryTemperature(), 
                                    readFurnacePressure(),
                                    readFurnaceTemperature(), 
                                    readFurnaceFlow(), 
                                    Get_Date_Time());

    Serial.println(message); // Print the message to the serial monitor
    last_msg = millis(); // Update the timestamp of the last message sent
    uint16_t packetIdPub1 = mqttClient.publish(mqtt_topic, 2, true, message.c_str()); // Publish the message to the MQTT broker at QoS 2
    Serial.printf("Publishing on topic %s at QoS 2, packetId: %i", mqtt_topic, packetIdPub1); // Print the MQTT publishing details to the serial monitor
    Serial.printf("Message: %.2f \n", 4); // Print a dummy message to the serial monitor
    FLAG_INTERRUPT = 0; // Reset the interrupt flag
  }
}
