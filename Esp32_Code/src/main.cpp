#include <main.h>

// Variables
int last_interrupt = 0; // Timestamp of the last interrupt
int FLAG_INTERRUPT = 0; // Flag to indicate if an interrupt occurred
int FLAG_DATA_TO_SEND = 0;

// Interrupt Service Routine for the test interrupt
void IRAM_ATTR interrupt_test(){
  if ( (millis() - last_interrupt) >= TIMEOUT_INTERRUPTION ){
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
  setupSensors(); // Setup sensor functionality (commented out)
  setupSdCard();
    
  delay(5000);
}

void loop() {

  xSemaphoreTake(mutex, portMAX_DELAY);
  if ( FLAG_INTERRUPT == 1 ) { // Check if an interrupt occurred
    String BatteryVoltage  = readBatteryVoltage(); 
    String BatteryTemperature = readBatteryTemperature(); 
    String FurnacePressure =  readFurnacePressure();
    String FurnaceTemperature =  readFurnaceTemperature(); 
    String DateTimeFileName = Get_Date_Time_File_Name();

    // Format the message using sensor readings and timestamp
    String message = formatMessage( BatteryVoltage, 
                                    BatteryTemperature, 
                                    FurnacePressure,
                                    FurnaceTemperature, 
                                    DateTimeFileName);

    std::replace( DateTimeFileName.begin(), DateTimeFileName.end() , '/', '-' );
    std::replace( DateTimeFileName.begin(), DateTimeFileName.end() , ' ', '_' );
    std::replace( DateTimeFileName.begin(), DateTimeFileName.end() , ':', '-' );
    String filename = "/"+ DateTimeFileName + ".txt";

    writeFile(SD,filename.c_str(),message.c_str());
    SendMqttMessage();
    FLAG_INTERRUPT = 0;
  }
  xSemaphoreGive(mutex);
}
