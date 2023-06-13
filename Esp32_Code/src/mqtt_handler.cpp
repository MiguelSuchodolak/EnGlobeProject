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
  // Create timers for MQTT and Wi-Fi reconnection
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));

  WiFi.mode(WIFI_STA); //init wifi mode
  WiFi.onEvent(WiFiEvent);
  connectToWifi();

  // Set up MQTT event handlers
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(mqtt_server, 1883);
}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt(); // Connect to MQTT after getting an IP address
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      if( WiFi.localIP().toString() == "0.0.0.0"){
        xTimerStop(mqttReconnectTimer, 0);
        xTimerStart(wifiReconnectTimer, 0); // Start Wi-Fi reconnection timer
      }
      else{
        xTimerStart(mqttReconnectTimer, 0);
      }
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  xTimerStart(mqttReconnectTimer, 0); // Start MQTT reconnection if Wi-Fi is still connected
}

void onMqttPublish(uint16_t packetId) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.print(packetId);

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