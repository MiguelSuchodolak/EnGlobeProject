import paho.mqtt.client as mqtt
import time
from broker import MQTTBroker

def test_broker_is_not_connected_on_startup():
    broker = MQTTBroker("test_broker")
    assert not broker.is_connected()

def test_broker_can_connect_to_mqtt_server():
    broker = MQTTBroker("test_broker")
    broker.connect('127.0.0.1', 1883)
    assert broker.is_connected()

def test_broker_can_subscribe_to_topics():
    broker = MQTTBroker("test_broker")
    broker.connect('127.0.0.1', 1883)

    assert broker.is_connected()

    # Send test messages to the topics that the broker is subscribed to
    client = mqtt.Client()
    client.connect('127.0.0.1', 1883)
    client.publish("esp32/sensor1", "test message")
    client.publish("esp32/sensor2", "test message")
    client.publish("rpi/broadcast", "test message")
    client.disconnect()

    time.sleep(30) # Wait for messages to be received by the broker

    # Check if the messages were received by the callbacks
    assert broker.last_esp32_sensor1_message == "test message"
    assert broker.last_esp32_sensor2_message == "test message"
    assert broker.last_rpi_broadcast_message == "test message"

def test_broker_can_disconnect_from_mqtt_server():
    broker = MQTTBroker("test_broker")
    broker.connect('127.0.0.1', 1883)

    assert broker.is_connected()

    broker.disconnect()

    assert not broker.is_connected()
