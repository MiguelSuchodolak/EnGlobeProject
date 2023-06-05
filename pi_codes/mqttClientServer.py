# This code is responsible for:
#     1. Get the data de ESP32 sended to the Local Broker on the RPI (MQTT Subscribe mode)
#     2. Send this data to Termica Broker (MQTT Publish mode)
# The broker IPs are set in the first lines, the LOCAL_BROKER IP will change according to the local connected network

from paho.mqtt import client as mqtt_client

#Setting local Broker IP address (= Raspberry Pi)
LOCAL_BROKER = 'localhost'
#Default port for MQTT
LOCAL_PORT = 1883

#Setting Termica Broker IP address
TERMICA_BROKER = "3.16.161.137"
#Default port for MQTT
TERMICA_PORT = 1883

#Topic to subscribe from, data comming from the ESP32 + PLC
topic_esp = "esp32/#"
topic_plc = "plc/#"
#Topic to publish on Termica broker
topic_termica = "oven"

#Clients IDs
client_id = "RPI_SubscribeMode"
client_id_RPI = "RPI_PublishMode"

#Function that creates clients 
#Used to create the local client connect to the Local Broker and the client connected with Termica Broker 
def connect_mqtt(broker_id, client_id, broker, port):
    def on_connect(client, userdata, flags, rc, broker_id):
        if rc == 0:
            print(f"Connected to {broker_id}!")
        else:
            print(f"Failed to connect to {broker_id}, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    if broker_id == "Termica Broker":
        client.username_pw_set("data", "datapasswd")
    client.on_connect = lambda client, userdata, flags, rc: on_connect(client, userdata, flags, rc, broker_id)
    client.connect(broker, port)
    return client


#Function responsible for detecting updates in certain topics
#Every time the topic has an update and the local client receives a message 
#the on_message function is called and the message received is forward 
#to the broker

def subscribe(client_local: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

        client_termica = connect_mqtt("Termica Broker", client_id_RPI, TERMICA_BROKER, TERMICA_PORT) #connected with Termica broker
        client_termica.loop_start()
        result = client_termica.publish(topic= topic_termica, payload = msg.payload, qos=2) 
        client_termica.loop(timeout=2)
        if result[0] == 0:
            print(f"Message published successfully to topic {topic_termica} on Termica Broker")
        else:
            print(f"Failed to publish message to topic {topic_termica} on Termica Broker")
        client_termica.loop_stop()
        client_termica.disconnect()  #Connects and disconnects every time new message arrives

    client_local.subscribe(topic_esp)
    client_local.subscribe(topic_plc)
    client_local.on_message = on_message

def run():
    client_local = connect_mqtt("Local Broker", client_id, LOCAL_BROKER, LOCAL_PORT)
    subscribe(client_local)
    client_local.loop_forever()

if __name__ == '__main__':
    run()
