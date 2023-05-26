import asyncio
import logging
from datetime import datetime
from asyncua import Client
import paho.mqtt.client as paho

broker = "localhost"
port = 1883

def on_publish(client, userdata, result):
    print("Data published on broker \n")
    pass

data_variables = ["Flowmeter_sensor", "Pressure_sensor", "Temperature_sensor"]

class SubHandler(object):
    """
    Subscription Handler. To receive events from server for a subscription
        FOR PROSYS SIMULATOR NEEDS TO BE HERE
    """
    def datachange_notification(self, node, val, data):
        print("Python: New data change event", node, val)

    def event_notification(self, event):
        print("Python: New event", event)


async def main():
    client1 = paho.Client("Local")
    client1.on_publish = on_publish
    client1.connect(broker,port)
    while True:
        url = "opc.tcp://192.168.229.206:53530/OPCUA/SimulationServer/"
        async with Client(url=url) as client:
            nsidx = await client.get_namespace_index("enGlobe_test")
          #  print(f"Namespace Index for :{nsidx}")
            for i in range(len(data_variables)):
                value = await client.nodes.root.get_child(["0:Objects", f"{nsidx}:enGlobe_test","{}:{}".format(nsidx, data_variables[i])])
                value_data = await value.get_value()
                print(f"{str(datetime.now()):<25} {data_variables[i]:>25}: {value_data:>25.2f}")
                client1.publish("value/{data_variables[i]}", str(value_data))
            print(" \n")
            subHandler = SubHandler()
            await asyncio.sleep(5)

if __name__ == "__main__":
    logging.basicConfig(level=logging.WARN)
    asyncio.run(main())