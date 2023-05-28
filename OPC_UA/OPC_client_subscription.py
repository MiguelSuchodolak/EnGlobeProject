# This code publish data from the PLC to the local Broker running at the RPI 
import asyncio
import logging
from datetime import datetime
from asyncua import Client
import paho.mqtt.client as paho
from paho.mqtt import client as mqtt_client

broker = "localhost"
port = 1883

topic_PLC_data = "sensors/PLC/"

data_variables = ["Flowmeter_sensor", "Pressure_sensor", "Temperature_sensor"]

def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print(f"Connected to Local Broker!")
        else:
            print(f"Failed to connect to Local Broker, return code %d\n", rc)

    client = mqtt_client.Client("PLC_client")
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def on_publish(client, userdata, result):
    print("Data published on broker \n")
    pass

class SubHandler(object):
    """
    Subscription Handler. To receive events from server for a subscription
        FOR PROSYS SIMULATOR NEEDS TO BE HERE
    """

    def __init__(self, nodes):
        self.nodes = nodes 
    
    def datachange_notification(self, node, val, data):
        print("Python: New data change event", node, val)
        if(node == self.nodes[0]): #change in the flowmeter, send to the Broker

            #Trata mensagem, msg_sub ----> msg_pub MESMO FORMATO QUE O ESP MANDA

            client = connect_mqtt() #connected with Termica broker
            client.loop_start()
            msg_pub = '{"id" : "entry_box", "temperature" : 1, "pressure" : 5, "flow": 4, "battery_voltage":16, "timestamp": 1}'

            result = client.publish(topic= topic_PLC_data, payload = msg_pub.encode('utf-8'), qos=2) #qos garante zero perda
            client.loop(timeout=10)           
            if result[0] == 0:
                print(f"Message published successfully to topic on Local Broker")
            else:
                print(f"Failed to publish message to topic on Local Broker")\
                
            client.loop_stop()
            client.disconnect()  #Connects and disconnects every time new message arrives

    def event_notification(self, event):
        print("Python: New event", event)


async def main():
    url = "opc.tcp://192.168.2.100:53530/OPCUA/SimulationServer/"
    async with Client(url=url) as client:
        nsidx = await client.get_namespace_index("enGlobe_test")
        var_flow = await client.nodes.root.get_child(["0:Objects", f"{nsidx}:enGlobe_test","{}:{}".format(nsidx, "Flowmeter_sensor")])
        var_pressure = await client.nodes.root.get_child(["0:Objects", f"{nsidx}:enGlobe_test", "{}:{}".format(nsidx, "Pressure_sensor")])
        var_temp = await client.nodes.root.get_child(["0:Objects", f"{nsidx}:enGlobe_test", "{}:{}".format(nsidx, "Temperature_sensor")])
       # subscription = await client.create_subscription(500,handler)
        nodes = [
            var_flow,
            var_pressure,
            var_temp,
        ]
        
#        handler = SubHandler(nodes)
        handler = SubHandler(nodes)
        subscription = await client.create_subscription(500,handler)
        await subscription.subscribe_data_change(nodes)
        stop_event =  asyncio.Event()
        await stop_event.wait()
  #      await asyncio.sleep(10)

if __name__ == "__main__":
    logging.basicConfig(level=logging.WARN)
    asyncio.run(main())
