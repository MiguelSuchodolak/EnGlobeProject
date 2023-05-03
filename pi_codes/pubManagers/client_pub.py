import time
import paho.mqtt.client as mqtt

def on_publish(client, userdata, mid):
    print("message published")


client = mqtt.Client("rpi_client_broadcast")
client.on_publish = on_publish
client.connect('192.168.15.2',1883)
client.loop_start()


while True:        
    try:
        msg = "Broadcast message"
        pubMsg = client.publish(
            topic='rpi/broadcast',
            payload=msg.encode('utf-8'),
            qos=0,
        )
        pubMsg.wait_for_publish()
        print(pubMsg.is_published())
    
    except Exception as e:
        print(e)
    time.sleep(2)
