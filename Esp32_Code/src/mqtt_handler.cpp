#include <mqtt_handler.h>

// Wi-Fi and MQTT configuration
const char* ssid = "Suchodolak2.4"; // Wi-Fi SSID
const char* password = "Doismaisdois2"; // Wi-Fi password
const char* mqtt_server = "192.168.15.11"; // MQTT broker server address
const char* mqtt_topic = "esp32/1"; // MQTT topic

AsyncMqttClient mqttClient; // MQTT client instance
TimerHandle_t mqttReconnectTimer; // Timer for MQTT reconnection
TimerHandle_t wifiReconnectTimer; // Timer for Wi-Fi reconnection 
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

void setupWifiAndMqttClient(){
  // Create timers for MQTT and Wi-Fi reconnection
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  // Set up Wi-Fi event handler
  WiFi.onEvent(WiFiEvent);

  // Set up MQTT event handlers
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);

  // Set MQTT server and port
  mqttClient.setServer(mqtt_server, 1883);

  // Connect to Wi-Fi
  connectToWifi();
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
      xTimerStop(mqttReconnectTimer, 0); // Stop MQTT reconnection while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0); // Start Wi-Fi reconnection timer
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
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0); // Start MQTT reconnection if Wi-Fi is still connected
  }
}

void onMqttPublish(uint16_t packetId) {
  xSemaphoreTake(mutex, portMAX_DELAY);
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.print(packetId);

  String filename = GetAFileName(SD, "/");
  filename = "/" + filename;
  deleteFile(SD, filename.c_str());
  
  filename = GetAFileName(SD, "/");
  if( filename != ERROR_OPENING_FILE ){
    FLAG_DATA_TO_SEND = 1;
  }
  else{
    FLAG_DATA_TO_SEND = 0;
  }
  xSemaphoreGive(mutex);
}
