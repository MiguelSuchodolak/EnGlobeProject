Scenario: Create a .json file using a custom JSON Manager
    Given I need to create a json file, with a certain set of rules
    When I create that json file
    Then I should be able to open it

Scenario: Read a .json file that doesnt exist
    Given I need to read a json file that doesnt exist
    When I try to read it
    Then The software shoud automaticaly create
    And read that json files

Scenario: Write on a json file that doesnt exist
    Given I need to write on a json file that doesnt exist
    When I try to write on it
    Then The software shoud automaticaly create
    And write on that json file

#Repetitive test with a very similar goal is not ideal
#The best practice is to write braoder test withou sacrificing specificity

#The low level specification bellow are not necessary and will only increase 
#complexity, make use of time and budget
Scenario: Read a .json file that  exist
    Given I need to read a json file that exist
    When I try to read it
    Then The software shoud read that json file

Scenario: Write on a json file that exist
    Given I need to write on a json file that exist
    When I try to write on it
    Then The software shoud write on that json file
