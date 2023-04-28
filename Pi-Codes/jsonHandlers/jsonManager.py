import json

class JsonManager():
    def readJSON(self):
        with open("data.json",'r') as my_json:
            data = json.load(my_json)
        return data

    def writeJSON(self, dataToWrite , whereToWrite ):
        jsonFile = self.readJSON()
        jsonFile[whereToWrite] = dataToWrite
        with open("data.json",'w') as my_json:
            json.dump( jsonFile, my_json, indent=4)

    def createJSONfile(self):
        data = {
            "x1": "-1000",
            "x2": "1000",
            "x3": "-1000",
            "fx1": "",
            "fx3": "",
            "FT": "0",
            "Counter": "0"
        }

        dataJSON = json.dumps(data, indent=4)
        with open("data.json", 'w+') as my_json:
            my_json.write(dataJSON)