# This code is responsible for:
#     1. Get the data de ESP32 sended to the Local Broker on the RPI (MQTT Subscribe mode)
#     2. Send this data to Termica Broker (MQTT Publish mode)
# The broker IPs are set in the first lines, the LOCAL_BROKER IP will change according to the local connected network

from paho.mqtt import client as mqtt_client

#Setting local Broker IP address (= Raspberry Pi)
LOCAL_BROKER = '192.168.2.111'
#Default port for MQTT
LOCAL_PORT = 1883

#Setting Termica Broker IP address
TERMICA_BROKER = "3.16.161.137"
#Default port for MQTT
TERMICA_PORT = 1883

#Topic to subscribe from, data comming from the ESP32 with sensors data
topic = "esp/#"         
#Topic to publish on Termica broker
topic_termica = "oven 1" 

#Clients IDs
client_id = "RPI_SubscribeMode"
client_id_RPI = "RPI_PublishMode"

def connect_mqtt(broker_id, client_id, broker, port) -> mqtt_client:
    def on_connect(client, userdata, flags, rc,broker):
        if rc == 0:
            print(f"Connected to {broker}!")
        else:
            print("Failed to connect to {client}, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    if broker_id == "Termica Broker":
        client.username_pw_set("data", "datapassword")
    client.on_connect = lambda client, userdata, flags, rc: on_connect(client, userdata, flags, rc, broker_id)
    client.connect(broker, port)
    return client

def subscribe(client_local: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        # recebe e envia para broker da termiCA

        client_termica = connect_mqtt("Termica Broker", client_id_RPI, TERMICA_BROKER, TERMICA_PORT) #connected with Termica broker

        #Trata mensagem, msg_sub ----> msg_pub

        msg_pub = '{"id" : "entry_box", "temperature" : 1, "pressure" : 5, "flow": 4, "battery_voltage":16, "timestamp": 1}'
        

        result = client_termica.publish(topic= topic_termica, payload = msg_pub.encode('utf-8'), qos=2) #qos garante zero perda

        if result[0] == 0:
            print(f"Message published successfully to topic {topic_termica} on Termica Broker")
        else:
            print(f"Failed to publish message to topic {topic_termica} on Termica Broker")

        client_termica.disconnect()  #Connects and disconnects every time new message arrives

    client_local.subscribe(topic)
    client_local.on_message = on_message

def run():
    client_local = connect_mqtt("Local Broker", client_id, LOCAL_BROKER, LOCAL_PORT)
    subscribe(client_local)
    client_local.loop_forever()

if __name__ == '__main__':
    run()
