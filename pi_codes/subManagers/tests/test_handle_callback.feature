Scenario: I have a pressure sensor data, and i need to find the pressure
Given I need the pressure data in PSI
When I make a calculation to get it
Then I shoul obtain the right value

Scenario: I have a flow sensor data, and i need to find the flow
Given I need the data in L/m^2
When I make a calculation to get it
Then I shoul obtain the right value

Scenario: Need to fullfill a json file with the sensor's information
Given I have the json file's templates
And The sensor's data
When I send this data to json format
Then I shoud obtain the information in the template's format in a json file