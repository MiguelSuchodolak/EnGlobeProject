import random
from datetime import datetime
import json

from paho.mqtt import client as mqtt_client #paho-mqtt package must be installed

broker = '192.168.178.39' #specify broker address
port = 1883 #specify port
topic = "python/mqtt"
# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 1000)}'
# username = 'emqx'
# password = 'public'

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    # client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def publish(client):
        oven_ID = 1
        dt = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        temperature = random.randint(30, 400)
        pressure = random.randint(0, 1000)
        bat_V = random.randint(0, 12)
        msg = {"oven ID": oven_ID, "Timestamp": dt, "temperature": temperature, "pressure": pressure, "Battery_Voltage": bat_V}
        json_string = json.dumps(msg)
        result = client.publish(topic, json_string, qos=1) #qos=2 not working??

        # result: [0, 1]
        status = result[0]
        if status == 0:
            print(f"Send `{msg}` to topic `{topic}`")
        else:
            print(f"Failed to send message to topic {topic}")


def run():
    client = connect_mqtt()
    client.loop_start()
    publish(client)


if __name__ == '__main__':
    run()
