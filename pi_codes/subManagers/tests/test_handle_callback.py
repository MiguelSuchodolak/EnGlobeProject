import pytest, os, json, re
from unittest import mock
from subManagers.handle_callback import handleCallback

@pytest.mark.parametrize("read_value,expected_result,is_correct", [(1,1.732223, True),(2,3.464446, True),(7,10, False)],ids=("Pass","Pass","Error"))
def test_calculate_pressure( read_value, expected_result, is_correct):
    handle = handleCallback()
    assert (round(handle.calculate_pressure(read_value), 6 ) == expected_result) == is_correct

@pytest.mark.parametrize("read_value,expected_result,is_correct", [(1,1.383, True),(2,1.607, True),(7,7, False)],ids=("Pass","Pass","Error"))
def test_calculate_flow( dummy_handler, read_value, expected_result, is_correct):
    assert (round(dummy_handler.calculate_flow(read_value), 3 ) == expected_result) == is_correct
    

@mock.patch("subManagers.handle_callback.handleCallback.calculate_flow")
@mock.patch("subManagers.handle_callback.handleCallback.calculate_pressure")
@mock.patch("subManagers.handle_callback.random.randint")
@mock.patch("subManagers.handle_callback.datetime")
def test_fill_json_file(mock_datetime,mock_randint,mock_pressure,mock_flow, tmpdir):
    mock_flow.return_value = 5
    mock_pressure.return_value = 4
    mock_randint.return_value = 3
    mock_datetime.datetime.now.return_value = 10
    
    handle = handleCallback()
    filepath = os.path.join(os.getcwd(), "test.json")
    
    handle.fill_json_file(1,1,1,filepath)
    
    data = {
        "esp_data": [
            {
            "esp_id": 1,
            "temperature_sensor_value": 3,
            "pressure_sensor_value": 4,
            "flow_sensor_value": 5,
            "timestamp": 10,
            }
        ]
    }
    
    assert re.search( str(data["esp_data"][0]), str(json.load(open(filepath,'r'))))
