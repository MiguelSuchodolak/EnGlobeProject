import asyncio
import logging

from asyncua import Server, ua
from asyncua.common.methods import uamethod


async def main():
    _logger = logging.getLogger(__name__)
    # setup our server
    server = Server()
    await server.init()
    server.set_endpoint("opc.tcp://localhost:4840/freeopcua/server/")

    # set up our own namespace, not really necessary but should as spec
    uri = "EnGlobeNamespace"
    idx = await server.register_namespace(uri)

    #Espera até receber o ESP

    # populating our address space
    # server.nodes, contains links to very common nodes like objects and root
    obj = await server.nodes.objects.add_object(idx, "EnGlobe")
    temp = await obj.add_variable(idx, "Temperature", 6.7)
    # Set MyVariable to be writable by clients
    await temp.set_writable()
    _logger.info("Starting server!")
    
    async with server:
        while True:
            await asyncio.sleep(1)
            new_val = await temp.get_value() + 0.1
            _logger.info("Set value of %s to %.1f", temp, new_val)
            await temp.write_value(new_val)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    asyncio.run(main(), debug=True)
