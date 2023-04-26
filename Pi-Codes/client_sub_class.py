import paho.mqtt.client as mqtt
import time

class MQTTBroker:
    def __init__(self, name):
        self.name = name
        self.flag_connected = 0
        self.client = mqtt.Client(name)
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        self.client.message_callback_add('esp32/sensor1', self.callback_esp32_sensor1)
        self.client.message_callback_add('esp32/sensor2', self.callback_esp32_sensor2)
        self.client.message_callback_add('rpi/broadcast', self.callback_rpi_broadcast)

    def on_connect(self, client, userdata, flags, rc):
        self.flag_connected = 1
        self.client_subscriptions(client)
        print("Connected to MQTT server")

    def on_disconnect(self, client, userdata, rc):
        self.flag_connected = 0
        print("Disconnected from MQTT server")

    def callback_esp32_sensor1(self, client, userdata, msg):
        print('ESP sensor1 data: ', msg.payload.decode('utf-8'))

    def callback_esp32_sensor2(self, client, userdata, msg):
        print('ESP sensor2 data: ', str(msg.payload.decode('utf-8')))

    def callback_rpi_broadcast(self, client, userdata, msg):
        print('RPi Broadcast message:  ', str(msg.payload.decode('utf-8')))

    def client_subscriptions(self, client):
        client.subscribe("esp32/#")
        client.subscribe("rpi/broadcast")

    def connect(self, host, port):
        self.client.connect(host, port)
        self.client.loop_start()
        self.client_subscriptions(self.client)
        print("......client setup complete............")

    def is_connected(self):
        return self.flag_connected
    
    
if __name__ == "__main__":
    broker = MQTTBroker("rpi_client1")
    broker.connect('127.0.0.1', 1883)

    while True:
        time.sleep(4)
        if not broker.is_connected():
            print("trying to connect MQTT server..")
