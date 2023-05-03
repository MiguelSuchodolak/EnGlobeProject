import json


class JsonManager():
    def readJSON(self, filepath):
        try:
            with open(filepath,'r') as my_json:
                data = json.load(my_json)
                return data
        except Exception:    
            print("File does not exist, creating json files")
            self.createJSON(filepath)
            return self.readJSON(filepath)

    def writeJSON(self, filepath, dataToWrite):
        try:
            with open(filepath,'r+') as my_json:
                jsonFile = json.load(my_json)
                jsonFile["esp_data"].append(dataToWrite)
                my_json.seek(0)
                json.dump(jsonFile, my_json, indent=4)
        except Exception:    
            print("File does not exist, creating json files")
            self.createJSON(filepath)
            self.writeJSON(filepath, dataToWrite)

    def createJSON(self, filepath):
        data = {"esp_data": []}

        dataJSON = json.dumps(data, indent=4)
        with open(filepath, 'w') as my_json:
            my_json.write(dataJSON)
