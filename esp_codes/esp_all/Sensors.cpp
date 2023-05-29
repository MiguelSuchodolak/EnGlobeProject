#include "Sensors.h"

Sensor::Sensor(int pin){
  Serial.begin(115200);
  ina219 = INA219(pin);
}

float Sensor::getCurrent(){
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    return -1;
  }
  return ina219.getCurrent_mA(); //Returns current in mA
}

float Sensor::getVoltage(){
  return ina219.getBusVoltage_V();
}

float Sensor::getShuntVoltage(){
  return ina219.getShuntVoltage_mV();
}

float Sensor::getLoadVoltage(){
  return ina219.getBusVoltage_V() + (ina219.getShuntVoltage_mV() / 1000);
}

//PRESSURE SENSOR

PressureSensor::PressureSensor(int pin) : Sensor(pin){
  Serial.println("Pressure sensor was initiated");
}

float PressureSensor::getPressure(){
  return _minPres + ((getCurrent() - _minPres)*(_maxPres - _minPres)/(_maxPres - _minPres));
}

FlowSensor::FlowSensor(int pin) : Sensor(pin){
  Serial.println("Flow sensor was initiated");
}

void FlowSensor::pulseHandler(){

}

float FlowSensor::getFlow(){

}

TemperatureSensor::TemperatureSensor(int pin) : Sensor(pin){
  Serial.println("Temperature sensor was initiated");
}

float TemperatureSensor::getTemperature(){

  return 0.1;
}

BatterySensor::BatterySensor(int pinC,int pinT){

}

float BatterySensor::getVoltage(){

}

float BatterySensor::getTemperature(){

}