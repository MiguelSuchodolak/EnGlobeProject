import unittest
from unittest.mock import Mock, patch
import paho.mqtt.client as mqtt
from my_module import MQTTBroker


class TestMQTTBroker(unittest.TestCase):
    def setUp(self):
        self.broker = MQTTBroker("test_client")
        self.host = "127.0.0.1"
        self.port = 1883

    def tearDown(self):
        self.broker.client.disconnect()
        self.broker.client.loop_stop()

    def test_on_connect(self):
        # test that flag_connected is set to 1 after connecting
        self.assertEqual(self.broker.flag_connected, 0)
        self.broker.on_connect(None, None, None, None)
        self.assertEqual(self.broker.flag_connected, 1)

    def test_on_disconnect(self):
        # test that flag_connected is set to 0 after disconnecting
        self.broker.flag_connected = 1
        self.broker.on_disconnect(None, None, None)
        self.assertEqual(self.broker.flag_connected, 0)

    def test_callback_esp32_sensor1(self):
        # test that the correct message is printed for esp32/sensor1
        msg = Mock()
        msg.payload.decode.return_value = "test message"
        self.broker.callback_esp32_sensor1(None, None, msg)
        self.assertEqual(
            self.broker.last_message,
            "ESP sensor1 data: test message"
        )

    def test_callback_esp32_sensor2(self):
        # test that the correct message is printed for esp32/sensor2
        msg = Mock()
        msg.payload.decode.return_value = "test message"
        self.broker.callback_esp32_sensor2(None, None, msg)
        self.assertEqual(
            self.broker.last_message,
            "ESP sensor2 data: test message"
        )

    def test_callback_rpi_broadcast(self):
        # test that the correct message is printed for rpi/broadcast
        msg = Mock()
        msg.payload.decode.return_value = "test message"
        self.broker.callback_rpi_broadcast(None, None, msg)
        self.assertEqual(
            self.broker.last_message,
            "RPi Broadcast message: test message"
        )

    def test_client_subscriptions(self):
        # test that the client is subscribed to the correct topics
        self.broker.client.subscribe = Mock()
        self.broker.client_subscriptions(self.broker.client)
        self.broker.client.subscribe.assert_called_with("esp32/#")
        self.broker.client.subscribe.assert_called_with("rpi/broadcast")

    @patch.object(mqtt.Client, "connect")
    def test_connect(self, mock_connect):
        # test that the client is connected to the correct host and port
        self.broker.connect(self.host, self.port)
        mock_connect.assert_called_with(self.host, self.port)
        self.assertTrue(self.broker.is_connected())

    @patch.object(mqtt.Client, "connect")
    def test_connect_failure(self, mock_connect):
        # test that the client does not connect on failure
        mock_connect.side_effect = Exception("connection failed")
        with self.assertRaises(Exception):
            self.broker.connect(self.host, self.port)
        self.assertFalse(self.broker.is_connected())

    def test_is_connected(self):
        # test that is_connected returns the correct value
        self.broker.flag_connected = 1
        self.assertTrue(self.broker.is_connected())
        self.broker.flag_connected = 0
        self.assertFalse(self.broker.is_connected())


if __name__ == "__main__":
    unittest.main()
