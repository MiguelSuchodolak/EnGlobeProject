#ifndef MQTT_HANDLER_H_
#define MQTT_HANDLER_H_

#include <Arduino.h> // Includes the Arduino framework
#include <WiFi.h> // Includes the WiFi library
#include <freertos/FreeRTOS.h> // Includes the FreeRTOS library for multitasking
#include <freertos/timers.h> // Includes the FreeRTOS timer library
#include <AsyncMqttClient.h> // Includes the Async MQTT Client library
#include <Wire.h> // Includes the Wire library for I2C communication
#include <sd_handler.h>

extern const char* ssid; // Declaration of the Wi-Fi network SSID
extern const char* password; // Declaration of the Wi-Fi network password
extern const char* mqtt_server; // Declaration of the MQTT broker server address
extern const char* mqtt_topic; // Declaration of the MQTT topic
extern SemaphoreHandle_t mutex;
extern AsyncMqttClient mqttClient; // Declaration of the MQTT client object
extern TimerHandle_t mqttReconnectTimer; // Declaration of the MQTT reconnect timer object
extern TimerHandle_t wifiReconnectTimer; // Declaration of the Wi-Fi reconnect timer object

void connectToWifi();
// Function to connect to the Wi-Fi network

void connectToMqtt();
// Function to connect to the MQTT broker

void WiFiEvent(WiFiEvent_t event);
// Callback function for handling Wi-Fi events

void onMqttConnect(bool sessionPresent);
// Callback function for handling MQTT connection

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
// Callback function for handling MQTT disconnection

void onMqttPublish(uint16_t packetId);
// Callback function for handling MQTT message publishing

void setupWifiAndMqttClient();
// Function to set up the Wi-Fi and MQTT client
void SendMqttMessage();
#endif
