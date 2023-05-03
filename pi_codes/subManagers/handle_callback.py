import numpy, random, datetime
from jsonHandlers.jsonManager import JsonManager

class handleCallback():
    
    def calculate_pressure(self, read_pressure_value):
        return read_pressure_value * 1.732223
    
    def calculate_flow(self, read_flow_value):
        return numpy.log(read_flow_value + 2.9872)
    
    def fill_json_file(self, esp_id, read_pressure_value, read_flow_value, filepath):
        jsonManager = JsonManager()
        temperature_value = random.randint(1,10)
        
        dataToWrite = {
            "esp_id": esp_id,
            "temperature_sensor_value": temperature_value,
            "pressure_sensor_value": self.calculate_pressure(read_pressure_value),
            "flow_sensor_value": self.calculate_flow(read_flow_value),
            "timestamp": datetime.datetime.now(),
            }

        jsonManager.writeJSON(filepath, dataToWrite)
        
        