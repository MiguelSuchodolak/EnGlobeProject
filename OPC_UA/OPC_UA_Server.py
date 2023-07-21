import asyncio
import logging
import json
from asyncua import Server
from asyncio_mqtt import Client, MqttError

import asyncio
import asyncio_mqtt as aiomqtt

# Setting local Broker IP address (= Raspberry Pi)
LOCAL_BROKER = "192.168.131.219"
# Default port for MQTT
LOCAL_PORT = 1883

# OPC UA server configuration
OPC_SERVER_IP = "192.168.131.219"
OPC_SERVER_PORT = 4840
OPC_NAMESPACE = "EnGlobeNamespace"

# Topic to subscribe from, data coming from the ESP32 + PLC
topic_esp = "esp32/1"

async def listen():
    async with aiomqtt.Client(LOCAL_BROKER, LOCAL_PORT) as client:
        async with client.messages() as messages:
            await client.subscribe(topic_esp)
            async for message in messages:
                print(message.payload)
                print(type(message.payload))
                print(str(message.payload))
                msg_json = json.loads(message.payload.decode("utf-8","ignore"))

                await temp.write_value(msg_json["FurnaceTemperature"])
                await pressure.write_value(msg_json["FurnacePressure"])
                await time_stamp.write_value(msg_json["Timestamp"])

async def handle():
    # Wait for messages in (unawaited) asyncio task
    loop = asyncio.get_event_loop()
    task = loop.create_task(listen())
    await task

async def main():
    global temp
    global pressure
    global time_stamp
    _logger = logging.getLogger(__name__)
    # setup our server
    server = Server()
    await server.init()
    server.set_endpoint(f"opc.tcp://{OPC_SERVER_IP}:{OPC_SERVER_PORT}")

    idx = await server.register_namespace(OPC_NAMESPACE)

    # populating our address space
    obj = await server.nodes.objects.add_object(idx, "EnGlobe")
    temp = await obj.add_variable(idx, "Temperature", 50.0)
    pressure = await obj.add_variable(idx, "Pressure", 0.0)
    time_stamp = await obj.add_variable(idx, "Timestamp", "")

    # Set MyVariable to be writable by clients
    await temp.set_writable()
    await pressure.set_writable()
    await time_stamp.set_writable()

    _logger.info("Starting server!")

    async with server:
        while True:
            await asyncio.sleep(1)

async def main_coroutine():
    await asyncio.gather(handle(), main())

if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    asyncio.run(main_coroutine())
