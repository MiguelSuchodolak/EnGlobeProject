#include <sensors.h>

// Initialize the INA219 objects for temperature, pressure, and flow
Adafruit_INA219 inaTemp(PressureAddress);
Adafruit_INA219 inaPress(TemperatureAddress);
Adafruit_INA219 inaFlow(FlowAddress);

void setupSensors(){

    // Initialize the INA219 sensors
    inaFlow.begin();
    inaTemp.begin();
    inaPress.begin();

    // Set the calibration values for each sensor
    inaTemp.setCalibration_16V_400mA();
    inaPress.setCalibration_16V_400mA();
    inaFlow.setCalibration_16V_400mA();

    // Set the pin modes for battery voltage and temperature pins
    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
    pinMode(BATTERY_TEMPERATURE_PIN, INPUT);
}

String readBatteryVoltage(){
    // Read the raw analog value from the battery voltage pin
    int rawValue = analogRead(BATTERY_VOLTAGE_PIN);

    // Convert the raw value to battery voltage using voltage divider calculation
    float batteryVoltage = rawValue * (VCC/4095) * ( ( R2 + R1 ) / R2 );

    // Convert the battery voltage to a string and return it
    return String(batteryVoltage).c_str();
}

String readBatteryTemperature(){
    // Read the raw analog value from the battery temperature pin
    int rawValue = analogRead(BATTERY_TEMPERATURE_PIN);

    // Convert the raw value to battery temperature using voltage divider calculation
    float batteryTemperature = rawValue * (VCC/4095) * ( ( R2 + R1 ) / R2 );

    // Convert the battery temperature to a string and return it
    return String(batteryTemperature).c_str();
}

String readFurnaceTemperature(){
    // Read the current in milliamperes from the temperature sensor
    float currentTemperature = inaTemp.getCurrent_mA();

    // Convert the current value to temperature using the calibration formula
    float temperature = currentTemperature * 11.875 - 87.5;

    // Convert the temperature to a string and return it
    return String(temperature).c_str();
}

String readFurnacePressure(){
    // Read the current in milliamperes from the pressure sensor
    float currentPressure = inaPress.getCurrent_mA();

    // Convert the current value to pressure using the calibration formula
    float pressure = currentPressure * 0.375 - 1.5;

    // Convert the pressure to a string and return it
    return String(pressure).c_str();
}

String readFurnaceFlow(){
    // Read the current in milliamperes from the flow sensor
    float currentFlow = inaFlow.getCurrent_mA();

    // Convert the current value to flow and return it as a string
    return String(currentFlow).c_str();
}
