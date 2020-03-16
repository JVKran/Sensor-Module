# Sensor-Module
This repository contains the code for a Multi-Sensor and 433MHz bridge in one! Specifically, it's written to support [this](https://github.com/JVKran/Wireless-Multisensor) Wireless Battery-Powered Multi-Sensor. 

## Functions
- Wireless Multi-Sensor Bridge (433MHz to MQTT). The temperature, humidity, pressure and movement readings are sent to this Sensor-Module which then publishes that to Home Assistant.
- Sensor-Hub. Temperature, humidity and movement are all sent to Home Assistant through MQTT.
- Alarmsystem. Effortlesly integrates with the Alarm-Component in Home Assistant. Currently features a buzzer and motion detector.
- Wireless Wall-Plug Controller. Controls 433MHz enabled Smart-Plugs.

## Schematic
To-Do