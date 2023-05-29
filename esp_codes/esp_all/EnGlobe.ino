/*
enGlobe 2023 - Termica group

This code is meant to be run in an ESP32. 
It has the purpose of monitoring the gas consumption of a furnace from the Termica group.
*/

#include "MessageBuffer.h"
#include "ServerConnection.h"
#include "Sensors.h"

#define FLOW_PIN 0
#define PRES_PIN 0
#define TEMP_PIN 0

#define BCUR_PIN 0
#define BTEMP_PIN 0


ServerConnection server("Anna","anna11011","192.168.229.13"); //Setup server connection
MessageBuffer buffer; //Setup message buffer

PressureSensor presSensor(PRES_PIN); //Setup pressure sensor
FlowSensor flowSensor(FLOW_PIN); //Setup flow sensor
TemperatureSensor tempSensor(TEMP_PIN); //Setup temperature sensor
BatterySensor batSensor(BCUR_PIN,BTEMP_PIN);

void interruptHandler(){
  /*float p = presSensor.getPressure();
  float t = tempSensor.getTemperature();
  float bt = batSensor.getTemperature();
  float bv = batSensor.getVoltage();
  float f = flowSensor.getFlow();
  buffer.createMessage(bv,bt,p,t,f);
  */
}

void setup(){
  attachInterrupt(FLOW_PIN,interruptHandler, RISING); //Interruption is set for when the flow pin is rising
  esp_sleep_enable_ext0_wakeup((gpio_num_t)FLOW_PIN, 1); //Wake up the ESP32 when the flow pin is high
}

void loop(){
  /*
  if(buffer.getSize() == 0){ 
    esp_deep_sleep_start(); //Enter deep sleep if buffer empty
  }
  

  while(!(server.isConnected())){
    server.connectMQTTServer();
    delay(1000);
  }
*/
  buffer.createMessage(0.1,0.1,0.1,0.1,0.1);
  char* message = buffer.lastMessage();

  Serial.println(message);
  int status = server.publishMessage(message,"esp32/sensor1");
  Serial.print("STATUS:"); Serial.println(status);
  delay(10000);

  //char* message = buffer.lastMessage(); //Take last message from buffer
  //server.publishMessage(message,"esp32/sensor1"); //Send it to the correct topic

}