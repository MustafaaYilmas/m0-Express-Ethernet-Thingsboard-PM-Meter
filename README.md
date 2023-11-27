

# Particle Matter Sensor Data Transmission with Adafruit M0 Express and Ethernet

This project involves using the Adafruit M0 Express and an Ethernet module to collect data from a particle matter sensor and transmit it to the ThingsBoard IoT platform.

## Requirements

- Adafruit M0 Express
- Ethernet Module
- Particle Matter Sensor (SEN0177)
- Arduino IDE
- ArduinoJson, Ethernet, SPI, Wire, ThingsBoard, and Arduino_MQTT_Client libraries

## Setup

### Installing Libraries

Install the `ArduinoJson`, `Ethernet`, `SPI`, `Wire`, `ThingsBoard`, and `Arduino_MQTT_Client` libraries via the Arduino IDE.

### OTA Setting

In the file `/xyz/Arduino/libraries/ThingsBoard/src/Configuration.h`, set the `THINGSBOARD_ENABLE_OTA` value to `0`. This step is necessary for the operation of the code.

### Loading the Code

Upload the provided code to the Arduino IDE. The code reads PM1.0, PM2.5, and PM10 values from the particle matter sensor and sends them to ThingsBoard.

#### Configuration

- `TOKEN`: Enter your ThingsBoard device access token here.
- `THINGSBOARD_SERVER`: Enter your ThingsBoard server address (example: `"demo.thingsboard.io"`).
- `THINGSBOARD_PORT`: Enter the ThingsBoard server port (typically 1883).

## Operation

Once the code is uploaded, you can observe the PM values read from the sensor and the successful connection to ThingsBoard through the serial monitor.

### Reading and Sending Data

The code periodically reads data from the particle matter sensor. After establishing an Ethernet connection, this data is sent to the ThingsBoard server using the MQTT protocol.

### Debugging

If you encounter any issues with data transmission or connection, first check your Ethernet connection, ThingsBoard token settings, and network configurations.

