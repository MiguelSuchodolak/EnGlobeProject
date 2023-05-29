#include "ServerConnection.h"

#define ONBOARD_LED 2

ServerConnection::ServerConnection(char* _ssid,char* _password,char* _MQTTserver){
  Serial.begin(115200);
  pinMode(ONBOARD_LED, OUTPUT);
  
  this->ssid = _ssid;
  this->password = _password;
  this->MQTTserver = _MQTTserver;
  //this->espClient = WiFiClient;
  this->client = PubSubClient((this->espClient));

  client.setServer(this->MQTTserver,1883);//1883 is the default port for MQTT server
  client.setCallback(messageCallback);
}

bool ServerConnection::isConnected(){
  return client.connected();
}

void ServerConnection::blinkLED(unsigned int times, unsigned int duration){
  for (int i = 0; i < times; i++) {
    digitalWrite(ONBOARD_LED, HIGH);
    delay(duration);
    digitalWrite(ONBOARD_LED, LOW); 
    delay(duration);
  }
}

int ServerConnection::setupWifi() {
  // Return 0 if successfully connected, else 1

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    delay(50);
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    delay(1000);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection failed");
    blinkLED(2,200); //blink LED twice (for 200ms ON time) to indicate that wifi not connected
  }else{
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  return (WiFi.status() == WL_CONNECTED)? 0 : 1;
}

int ServerConnection::connectMQTTServer() {
  // Return 0 if able to connect successfully, else 1
  int wifiStatus = 1;

  if (!client.connected()) {
    //First check if connected to wifi
    if(WiFi.status() != WL_CONNECTED){
      //If not connected, then first connect to wifi
      wifiStatus = setupWifi();
    }else{
      wifiStatus = 0;
    }

    if(!wifiStatus){
      //Now attemt to connect to MQTT server
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("ESP32_client1")) { // Change the name of client here if multiple ESP32 are connected
        //Attempt successful
        Serial.println("Connected");
        // Subscribe to topics here
        //client.subscribe("rpi/broadcast");
        //client.subscribe("rpi/xyz"); //subscribe more topics here
        delay(1000);
      } else {
        //Attempt not successful
        Serial.print("Failed, rc=");
        Serial.print(client.state());

        blinkLED(3,200); //Blink LED three times (200ms on duration) to show that MQTT server connection attempt failed
        // Wait 2 seconds before retrying
        delay(1000);
      }
    }
  }
  return (client.connected())? 0 : 1;
  
}

void ServerConnection::messageCallback(char* topic, byte* message, unsigned int length) {
  //This function will be executed whenever there is data available on subscribed topics
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Check if a message is received on the topic "rpi/broadcast"
  if (String(topic) == "rpi/broadcast") {
    if(messageTemp == "10"){
      blinkLED(1,1250); //blink LED once (for 1250ms ON time)
    }
  }

  //Similarly add more if statements to check for other subscribed topics 
}

int ServerConnection::publishMessage(char* message, char* topic){
  Serial.print("Publish message on ");
  Serial.println(topic);

  if (!(this->client).connected()) {
    connectMQTTServer();
  }
  if ((this->client).connected()) {
    //topic = "esp32/sensor1"
    (this->client).publish(topic,message);
    delay(1000);
    //(this->client).loop();
    return 0;
  }else{
    return 1;
  }

}
