# EnGlobeProject

# Project Title

## Description

This project is an ESP32-based system that collects readings from various sensors and publishes these readings to an MQTT broker.

## Hardware Requirements

This project utilizes the following hardware components:

1. **ESP32:** This is the microcontroller running the main code. It communicates with all sensors and modules and sends the sensor data via MQTT.

2. **INA219 Bi-directional DC Current/Power Monitor Modules:** Two of these modules are used in this project to monitor power and current. They are connected to the ESP32 via the I2C interface. Be sure to connect VCC to 3.3V, GND to GND, SDA to GPIO21 (or the GPIO you have defined as SDA), and SCL to GPIO22 (or the GPIO you have defined as SCL).

3. **Temperature Sensor:** This sensor is used to measure the temperature. Connect its data pin to the appropriate GPIO pin on the ESP32 as defined in your code.

4. **DS1302 Real Time Clock (RTC) Module:** This module provides accurate timekeeping for timestamping the sensor data. Connect the CLK, DAT, and RST pins of the DS1302 module to the GPIO pins defined in your code, and VCC and GND to the power supply.

5. **Button**.

Please ensure all your wiring connections are secure and correct to prevent any malfunctioning or damage to your devices. If you're not using a breadboard or PCB for your project, it's a good idea to use a multimeter to check that your connections are correct before powering up the ESP32.


## Hardware Setup

The following hardware components are required for this project:

1. ESP32 development board
2. Two INA219 Current Sensor modules
3. One Temperature Sensor (Please replace with your specific model)
4. DS1302 Real-Time Clock (RTC) module

### ESP32 Connections

- Connect the LED to the GPIO 2 on the ESP32.-----------------------------------
- Connect the interruption input to GPIO 12 on the ESP32. ----------------------

### INA219 Current Sensor Module Connections

- Connect the INA219 modules to the I2C pins on the ESP32. 
- SCL pin 22
- SDA pin 21
- Make sure the power (VCC 3.3v) and ground (GND) pins are also connected.

### Temperature Sensor Connections

- Connect your specific temperature sensor to the appropriate pins on the ESP32, pin 39(somethimes called VN.)

### DS1302 Real-Time Clock (RTC) Module Connections

- Connect the DS1302 RTC module to the SPI pins on the ESP32. This typically involves connecting the MOSI, MISO, and SCK.
- CLK pin 32.
- DAT pin 26.
- RST pin 27.
- Make sure the power (VCC 3.3v) and ground (GND) pins are also connected.

### Button 
- Connect the **button** in GND and Pin 13.

## Required Software

- Visual Studio Code
- PlatformIO IDE extension for VS Code

## Software Setup

This project requires the following software tools:

1. Visual Studio Code
2. PlatformIO IDE extension for Visual Studio Code
3. Git
4. GitHub extension for Visual Studio Code

### Visual Studio Code Installation

1. Visit the [official Visual Studio Code download page](https://code.visualstudio.com/download).
2. Download the version of Visual Studio Code suitable for your operating system.
3. Run the downloaded file and follow the installation prompts.

### PlatformIO IDE Installation

1. Open Visual Studio Code.
2. Click on the Extensions view icon on the Sidebar (square icon on the side).
3. In the Extensions view search bar, type 'PlatformIO IDE'.
4. Click 'Install' on the PlatformIO IDE search result to install the extension.

### Git Installation

1. Visit the [official Git download page](https://git-scm.com/downloads).
2. Download the Git version suitable for your operating system.
3. Run the downloaded file and follow the installation prompts.
4. Once installed, you can confirm the installation by opening a new terminal window and typing `git --version`. This should return the installed version of Git.

### GitHub Extension for Visual Studio Code

1. Open Visual Studio Code.
2. Click on the Extensions view icon on the Sidebar.
3. In the Extensions view search bar, type 'GitHub'.
4. Look for the 'GitHub' extension provided by 'GitHub.vscode-pull-request-github' and click 'Install'.

### Cloning the Repository with GitHub Extension

1. Open Visual Studio Code.
2. Click on the Source Control view icon on the Sidebar (the icon looks like three connected dots).
3. Click on 'Clone Repository'.
4. In the input box that appears, enter the URL of this repository and press 'Enter'.
5. Select the local directory where you want to clone the repository and click 'Select Repository Location'.
6. Once cloned, a notification will appear asking if you would like to open the cloned repository. Click 'Open' to open the newly cloned repository in a new Visual Studio Code window.

### Library Installation

1. Open the cloned project in PlatformIO.
2. Navigate to "PIO Home" > "Libraries". -----------------------------------------------------------------------------------
3. In the search bar, type the name of the library you want to install (determined based on the #include statements in your code).
4. Click on the relevant search result and click "Add to Project". Select your project and click "Add".
5. Repeat this process for all required libraries.

### Running the Project

1. Open the main project file in PlatformIO.
2. Click on the checkmark icon in the lower toolbar to compile the code, and ensure there are no errors.
3. Connect your ESP32 to your computer via USB.
4. Click on the right arrow icon in the lower toolbar to upload the code to your ESP32.
5. Open the serial monitor to view the printed messages.
