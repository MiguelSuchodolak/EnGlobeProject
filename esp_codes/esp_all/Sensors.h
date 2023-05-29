#ifndef Sensors_h
#define Sensors_h

#include <Wire.h>
#include <Adafruit_INA219.h>

#define INA219 Adafruit_INA219

class Sensor{
  protected:
    INA219 ina219;
  public:
    Sensor(int pin);
    float getCurrent();
    float getVoltage();
    float getShuntVoltage();
    float getLoadVoltage();
};


class PressureSensor: public Sensor{  
  private:
    const float _R = 1.0;
    const float _maxPres = 10.0;  //Maximum pressure in Pa
    const float _minPres = 100.0; //Minimum pressure in Pa
    const float _maxCur = 20.0; //Maximum current in mA
    const float _minCur = 4.0;  //Minimum current in mA
    float _pin;
  public:
    PressureSensor(int pin);
    float getPressure();
};

class FlowSensor: public Sensor{
  private:
    const float _volume = 1.0;
    int _tLastMeas;
    int _counter;
    int _pin;
  public:
    FlowSensor(int pin);
    void pulseHandler();
    float getFlow();
};

class TemperatureSensor: public Sensor{
  private:
    const float _R0 = 1.0;
    const float _a = 1.0;
    float _R1;
    int _pin;
  public:
    TemperatureSensor(int pin);
    float getTemperature();
};

class BatterySensor{
  private:

  public:
    BatterySensor(int pinC,int pinT);
    float getVoltage();
    float getTemperature();
};

#endif