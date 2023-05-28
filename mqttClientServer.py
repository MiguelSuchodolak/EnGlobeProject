import random
from paho.mqtt import client as mqtt_client

LOCAL_BROKER = '192.168.2.111'
LOCAL_PORT = 1883

TERMICA_BROKER = "3.16.161.137"
TERMICA_PORT = 1883

topic = "esp/#"          #Topic to subscribe from the ESP32
topic_termica = "oven 1" #Topic to publish on Termica broker

client_id = "RPI SUB"
client_id_RPI = "RPI PUB"

def connect_mqtt_termica():
    def on_connect_termica(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to Termica Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client_termica = mqtt_client.Client(client_id)
    client_termica.username_pw_set("data", "datapassword")
    client_termica.on_connect = on_connect_termica
    client_termica.connect(TERMICA_BROKER, TERMICA_PORT)
    return client_termica


def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to Local MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(LOCAL_BROKER, LOCAL_PORT)
    return client


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        # recebe e envia para broker da termiCA

        client_termica = connect_mqtt_termica() #connected with Termica broker

        #Trata mensagem, msg_sub ----> msg_pub

        msg_pub = '{"id" : "entry_box", "temperature" : 1, "pressure" : 5, "flow": 4, "battery_voltage":16, "timestamp": 1}'

        result = client.publish(topic= topic_termica, payload = msg_pub.encode('utf-8'), qos=2) #qos garante zero perda
        
        if result[0] == 0:
            print(f"Message published successfully to topic {topic_termica} on Termica Broker")
        else:
            print(f"Failed to publish message to topic {topic_termica} on Termica Broker")

        client_termica.disconnect()  #Connects and disconnects every time new message arrives

    client.subscribe(topic)
    client.on_message = on_message


def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()
