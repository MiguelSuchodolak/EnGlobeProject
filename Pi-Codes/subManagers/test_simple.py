import unittest
import paho.mqtt.client as mqtt
from unittest.mock import Mock, call
import time
from mqtt_broker import MQTTBroker


class TestMQTTBroker(unittest.TestCase):
    def setUp(self):
        self.broker = MQTTBroker("rpi_client1")
        self.mock_client = Mock(spec=mqtt.Client)
        self.broker.client = self.mock_client

    def test_connect_to_mqtt_server_and_receive_messages(self):
        # Given
        expected_calls = [
            call("esp32/#"),
            call("rpi/broadcast"),
        ]

        # When
        self.broker.connect('127.0.0.1', 1883)
        self.broker.client.on_connect(self.mock_client, None, None, None)
        time.sleep(1)

        # Then
        self.assertTrue(self.broker.is_connected())
        self.mock_client.subscribe.assert_has_calls(expected_calls)

    def test_fail_to_connect_to_mqtt_server_and_retry(self):
        # Given
        self.mock_client.connect.side_effect = ConnectionRefusedError
        expected_calls = [
            call("esp32/#"),
            call("rpi/broadcast"),
        ]

        # When
        self.broker.connect('127.0.0.1', 1883)

        # Then
        self.assertFalse(self.broker.is_connected())
        self.mock_client.subscribe.assert_not_called()
        time.sleep(5)
        self.mock_client.connect.assert_has_calls([call('127.0.0.1', 1883)])
        self.mock_client.subscribe.assert_has_calls(expected_calls)
