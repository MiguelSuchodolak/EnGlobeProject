# This code publish data from the PLC to the local Broker running at the RPI 
from datetime import datetime
import json
import asyncio 
import logging
from asyncua import Client
import paho.mqtt.client as paho
from paho.mqtt import client as mqtt_client

broker = "localhost"
port = 1883

topic_PLC_data = "plc/1"

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
    """

    def __init__(self, nodes):
        self.nodes = nodes 
    
    async def datachange_notification(self, node, val, data):
        print("Python: New data change event", node, val)
        if(node == self.nodes[0]): #change in the flowmeter, send to the Broker

            client = connect_mqtt() #connected with Termica broker
            client.loop_start()

            flow = float(await self.nodes[0].get_value())
            pressure = float(await self.nodes[1].get_value())
            temp = float(await self.nodes[2].get_value())
            current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

            msg_pub = {
                "Id": "PLC1",
                "FurnaceTemperature": temp,
                "FurnacePressure" : pressure,
                "Timestamp": str(current_time)
            }
            #payload = '{{"id":"{}", "temperature":{}, "pressure":{}, "flow":{}, "timestamp":"{}"}}'.format("PLC/1", temp, pressure, 1, current_time)
            #msg_pub = "{\"Id\":" + "\"PLC1\"" + "," + "\"FurnaceTemperature\":" + str(temp) + "," + "\"FurnancePressure\":" + str(pressure) + "," + "\"Timestamp\":" + str(current_time) + "}"
            pay = json.dumps(msg_pub)

            result = client.publish(topic= topic_PLC_data, payload = pay, qos=2) 
           
            client.loop(timeout=5)

            if result[0] == 0:
                print(f"Message published successfully to topic on Local Broker")
            else:
                print(f"Failed to publish message to topic on Local Broker")\
                
            client.loop_stop()
            client.disconnect()  #Connects and disconnects every time new message arrives

    def event_notification(self, event):
        print("Python: New event", event)


async def main():
    url = "opc.tcp://192.168.131.206:53530/OPCUA/SimulationServer/" #Change IP according to the computer running the Simulator Prosys
    async with Client(url=url) as client:
        nsidx = await client.get_namespace_index("enGlobe_test")
        var_flow = await client.nodes.root.get_child(["0:Objects", f"{nsidx}:enGlobe_test","{}:{}".format(nsidx, "Flowmeter_sensor")])
        var_pressure = await client.nodes.root.get_child(["0:Objects", f"{nsidx}:enGlobe_test", "{}:{}".format(nsidx, "Pressure_sensor")])
        var_temp = await client.nodes.root.get_child(["0:Objects", f"{nsidx}:enGlobe_test", "{}:{}".format(nsidx, "Temperature_sensor")])
        nodes = [
            var_flow,
            var_pressure,
            var_temp,
        ]
        handler = SubHandler(nodes)
        subscription = await client.create_subscription(500,handler)
        await subscription.subscribe_data_change(nodes)
        stop_event =  asyncio.Event()
        await stop_event.wait()
  #      await asyncio.sleep(10)

if __name__ == "__main__":
    logging.basicConfig(level=logging.WARN)
    asyncio.run(main())
