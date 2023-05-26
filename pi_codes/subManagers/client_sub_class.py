# Importing the necessary libraries
import paho.mqtt.client as mqtt
import time

# Defining a class to handle MQTT broker
class MQTTBroker:
    def __init__(self, name):
        # Initializing the class instance with a given name and other attributes
        self.name = name
        self.flag_connected = 0
        self.client = mqtt.Client(name)
        # Assigning callback methods to handle various MQTT events
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        self.client.message_callback_add('esp32/#', self.callback_esp32_client) 
        self.client.message_callback_add('rpi/broadcast', self.callback_rpi_broadcast)

    # Method to handle the MQTT 'on_connect' event
    def on_connect(self, client, userdata, flags, rc):
        self.flag_connected = 1
        # Subscribing to the relevant MQTT topics
        self.client_subscriptions(client)
        # Printing a message on successful connection
        print("Connected to MQTT server")

    # Method to handle the MQTT 'on_disconnect' event
    def on_disconnect(self, client, userdata, rc):
        self.flag_connected = 0
        # Printing a message on disconnection
        print("Disconnected from MQTT server")

    # Callback method to handle the MQTT messages on 'esp32/#' topic
    def callback_esp32_client(self, client, userdata, msg):
        print(msg.topic, msg.payload.decode('utf-8'))

    # Callback method to handle the MQTT messages on 'rpi/broadcast' topic
    def callback_rpi_broadcast(self, client, userdata, msg):
        print('RPi Broadcast message:  ', msg.payload.decode('utf-8'))

    # Method to subscribe to the relevant MQTT topics
    def client_subscriptions(self, client):
        client.subscribe("esp32/#")
        client.subscribe("rpi/broadcast")

    # Method to connect to the MQTT broker
    def connect(self, host, port):
        # Establishing the connection with the given host and port
        self.client.connect(host, port)
        # Starting the MQTT client loop to handle incoming messages
        self.client.loop_start()
        # Subscribing to the relevant MQTT topics
        self.client_subscriptions(self.client)
        # Printing a message on successful connection setup
        print("......client setup complete............")

    # Method to check the connection status of the MQTT broker
    def is_connected(self):
        return self.flag_connected

# Main program to connect to the MQTT broker
if __name__ == "__main__":
    # Creating an instance of MQTTBroker class with name "rpi_client1"
    broker = MQTTBroker("rpi_client1")
    # Connecting to the MQTT broker with host IP address '127.0.0.1' and port number 1883
    broker.connect('192.168.229.13', 1883)

    # Loop to keep checking the MQTT server connection status
    while True:
        # Waiting for 4 seconds before checking the connection status again
        time.sleep(4)
        # If the broker is disconnected, trying to reconnect
        if not broker.is_connected():
            print("trying to connect MQTT server..")
