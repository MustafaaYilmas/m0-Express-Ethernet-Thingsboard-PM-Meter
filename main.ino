//Note: /xyz/Arduino/libraries/ThingsBoard/src/Configuration.h -> line 48  define THINGSBOARD_ENABLE_OTA 1 to change  define THINGSBOARD_ENABLE_OTA 0

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>
#include <ThingsBoard.h>
#include <Arduino_MQTT_Client.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#define LENG 31 // 0x42 + 31 bytes equal to 32 bytes
unsigned char buf[LENG];

int PM01Value = 0;  // define PM1.0 value of the air detector module
int PM2_5Value = 0; // define PM2.5 value of the air detector module
int PM10Value = 0;  // define PM10 value of the air detector module

struct PMValues
{
    int PM1, PM25, PM10;
};

// Credentials for the ThingsBoard platform
#define TOKEN "pHYZ7VkKvoxTOyy6LDc1"
#define THINGSBOARD_SERVER "demo.thingsboard.io"
constexpr uint16_t THINGSBOARD_PORT = 1883;
constexpr uint16_t MAX_MESSAGE_SIZE = 128;

EthernetClient m0Client;
Arduino_MQTT_Client mqttClient(m0Client);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

void ethernetFirtsConnection()
{
    // Serial.println("Connection...");
    if ((Ethernet.begin() == 1))
    {
        Serial.println("Connected");
    }
    else
    {
        Serial.println("Not Connection");
    }
}

void setup()
{
    Serial1.begin(9600); // init pm sensor
    Serial1.setTimeout(1500);
    delay(1000);
    Serial.begin(9600);
    delay(1000);
    ethernetFirtsConnection();
}

void loop()
{

    if (!tb.connected())
    {
        if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT))
        {
            Serial.println("Failed to connect to ThingsBoard Server...");
            return;
        }
    }

    PMValues returnedData = readPMValues();

    // Sending PM data to ThingsBoard
    tb.sendTelemetryData("PM1", returnedData.PM1);
    tb.sendTelemetryData("PM2_5", returnedData.PM25);
    tb.sendTelemetryData("PM10_0", returnedData.PM10);
}

void readPMValues()
{
    if (PMSerial.find(0x42))
    {
        delay(100);
        PMSerial.readBytes(buf, LENG);

        if (buf[0] == 0x4d)
        {
            if (checkValue(buf, LENG))
            {
                PM01Value = transmitPM01(buf);   // count PM1.0 value of the air detector module
                PM2_5Value = transmitPM2_5(buf); // count PM2.5 value of the air detector module
                PM10Value = transmitPM10(buf);   // count PM10 value of the air detector module
            }
        }
    }

    PMValues data;
    data.PM1 = PM01Value;
    data.PM25 = PM2_5Value;
    data.PM10 = PM10Value;

    static unsigned long OledTimer = millis();
    if (millis() - OledTimer >= 1000)
    {
        OledTimer = millis();

        Serial.print("PM1.0: ");
        Serial.print(PM01Value);
        Serial.println("  ug/m3");

        Serial.print("PM2.5: ");
        Serial.print(PM2_5Value);
        Serial.println("  ug/m3");

        Serial.print("PM1 0: ");
        Serial.print(PM10Value);
        Serial.println("  ug/m3");
        Serial.println();
    }
}

char checkValue(unsigned char *thebuf, char leng)
{
    char receiveflag = 0;
    int receiveSum = 0;

    for (int i = 0; i < (leng - 2); i++)
    {
        receiveSum = receiveSum + thebuf[i];
    }
    receiveSum = receiveSum + 0x42;

    if (receiveSum == ((thebuf[leng - 2] << 8) + thebuf[leng - 1])) // check the serial data
    {
        receiveSum = 0;
        receiveflag = 1;
    }
    return receiveflag;
}

int transmitPM01(unsigned char *thebuf)
{
    int PM01Val;
    PM01Val = ((thebuf[3] << 8) + thebuf[4]); // count PM1.0 value of the air detector module
    return PM01Val;
}

// transmit PM Value to PC
int transmitPM2_5(unsigned char *thebuf)
{
    int PM2_5Val;
    PM2_5Val = ((thebuf[5] << 8) + thebuf[6]); // count PM2.5 value of the air detector module
    return PM2_5Val;
}

// transmit PM Value to PC
int transmitPM10(unsigned char *thebuf)
{
    int PM10Val;
    PM10Val = ((thebuf[7] << 8) + thebuf[8]); // count PM10 value of the air detector module
    return PM10Val;
}
