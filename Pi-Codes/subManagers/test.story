Feature: MQTTBroker connects to MQTT server and receives messages

  Scenario: Successfully connect to MQTT server and receive messages
    Given a MQTTBroker instance named "rpi_client1"
    When the broker connects to the server at "127.0.0.1" and port 1883
    Then the broker should be connected to the MQTT server
    And the broker should receive messages from the "esp32/sensor1" topic
    And the broker should receive messages from the "esp32/sensor2" topic
    And the broker should receive messages from the "rpi/broadcast" topic

  Scenario: Fail to connect to MQTT server and retry
    Given a MQTTBroker instance named "rpi_client1"
    When the broker tries to connect to the server at "127.0.0.1" and port 1883
    And the server is not available
    Then the broker should not be connected to the MQTT server
    And the broker should retry to connect after a certain interval
