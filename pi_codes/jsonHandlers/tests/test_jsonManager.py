import json, os

from jsonHandlers.jsonManager import JsonManager

def test_createJSON(tmpdir):
    filepath = os.path.join(tmpdir, "test.json")
    
    jsonManager = JsonManager()
    jsonManager.createJSON(filepath=filepath)

    assert json.load(open(filepath,'r'))

def test_writeJSON(tmpdir):
    filepath = os.path.join(tmpdir, "test.json")
    filepath2 = os.path.join(tmpdir, "test2.json")
    jsonManager = JsonManager()

    dataToWrite = {
        "esp_data": [
            {
            "esp_id": "1",
            "temperature_sensor_value": "1",
            "pressure_sensor_value": "1",
            "flow_sensor_value": "1",
            "timestamp": "1",
            "id_of_read" : "1"
            }
        ]
    }

    jsonManager.writeJSON(filepath, dataToWrite)
    
    data = {"esp_data":[]}
    data_json = json.dumps(data, indent=4)
    with open(filepath2,'w+') as my_json:
        my_json.write(data_json)
    with open(filepath2, 'r+') as my_json:
        jsonFile = json.load(my_json)
        jsonFile["esp_data"].append(dataToWrite)
        my_json.seek(0)
        json.dump(jsonFile, my_json, indent=4)
    
    jsonTest = json.load(open(filepath, 'r'))
    jsonModel = json.load(open(filepath2, 'r'))
    
    assert jsonTest["esp_data"] == jsonModel["esp_data"]
    
def test_readJSON(tmpdir):
    filepath = os.path.join(tmpdir, "test.json")
    
    jsonManager = JsonManager()
    jsonTest = jsonManager.readJSON(filepath)
    
    jsonModel = json.load(open(filepath, 'r'))
    
    assert jsonTest == jsonModel
