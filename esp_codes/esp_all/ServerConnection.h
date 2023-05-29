#ifndef ServerConnection_h
#define ServerConnection_h

#include <WiFi.h>
#include <PubSubClient.h>

class ServerConnection{
  private:
    const char* ssid;
    const char* password;
    const char* MQTTserver;
    WiFiClient espClient;
    PubSubClient client;

    static void messageCallback(char* topic, byte* message, unsigned int length);
    static void blinkLED(unsigned int times, unsigned int duration);
  public:
    ServerConnection(char* _ssid,char* _password,char* _MQTTserver);
    bool isConnected();
    int setupWifi();
    int connectMQTTServer();
    int publishMessage(char* message, char* topic);
};

#endif