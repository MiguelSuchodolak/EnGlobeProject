#include <mqtt_handler.h>

// Wi-Fi and MQTT configuration
const char* ssid = "Anna"; // Wi-Fi SSID
const char* password = "anna11011"; // Wi-Fi password
const char* mqtt_server = "192.168.238.219"; // MQTT broker server address
const char* mqtt_topic = "esp32/1"; // MQTT topic

AsyncMqttClient mqttClient; // MQTT client instance
TimerHandle_t mqttReconnectTimer; // Timer for MQTT reconnection
TimerHandle_t wifiReconnectTimer; // Timer for Wi-Fi reconnection 
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

void setupWifiAndMqttClient(){
  connectToWifi();
  // Set up MQTT event handlers
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(mqtt_server, 1883);

}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.mode(WIFI_STA); //init wifi mode
  WiFi.onEvent(WiFiEvent);
  WiFi.begin(ssid, password);
}

void connectToMqtt() {
  if( ! mqttClient.connected() ){
    Serial.println("Connecting to MQTT...");
    mqttClient.connect();
  }
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
    if( event == SYSTEM_EVENT_STA_GOT_IP){
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt(); // Connect to MQTT after getting an IP address
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
}

void onMqttPublish(uint16_t packetId) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  Serial.print("Publish acknowledged.");

  String filename = GetAFileName(SD, "/");
  filename = "/" + filename;
  deleteFile(SD, filename.c_str());
  SendMqttMessage();
  
  xSemaphoreGive(mutex);
}

void SendMqttMessage(){
  String filename = GetAFileName(SD, "/");
  if( filename != ERROR_OPENING_FILE && filename != "/"){
    filename = "/" + filename;
    String message = GetReadFromFile(SD, filename.c_str());
    if(message != ERROR_OPENING_FILE && message != "/"){
      Serial.print("Trying to send message: ");
      Serial.println(message);
      mqttClient.publish(mqtt_topic, 2, true, message.c_str());
    }
  }
}