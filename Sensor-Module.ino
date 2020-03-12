#include <Manchester.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <NewRemoteTransmitter.h>
#include "speaker.hpp"
#include "alarm.hpp"
#include "buzzer.hpp"
#include "switch.hpp"
#include "array.hpp"
#include "mqttClient.hpp"
#include "climate.hpp"
#include "sensorBridge.hpp"

piezoBuzzer buzzer(D8);

WiFiClient espClient;
mqttClient client("KraanBast2.4", "Snip238!", "192.168.178.81", "/raspberrypi/hassio", espClient, buzzer);

NewRemoteTransmitter kakuTransmitter(20589486, D3, 257, 3);
NewRemoteTransmitter actionTransmitter(54973440, D3, 261, 3);
NewRemoteTransmitter nextActionTransmitter(8463360, D3, 261, 3);

speaker bose(D0, client);
alarmSystem alarm(client, "/woonkamer/alarm", buzzer);
motionSensor movementSensor(D1);
tempHumSensor climateSensor(client, D7, 60000);

wallSwitch kaku0(kakuTransmitter, 0, client, "/woonkamer/kaku0", "kaku0");
wallSwitch kaku1(kakuTransmitter, 1, client, "/woonkamer/kaku1", "kaku1");
wallSwitch kaku2(kakuTransmitter, 2, client, "/woonkamer/kaku2", "kaku2");
wallSwitch kaku3(kakuTransmitter, 3, client, "/woonkamer/kaku3", "kaku3");

wallSwitch action1(actionTransmitter, 1, client, "/woonkamer/action1", "action1");
wallSwitch action2(actionTransmitter, 2, client, "/woonkamer/action2", "action2");
wallSwitch action3(actionTransmitter, 3, client, "/woonkamer/action3", "action3");

wallSwitch action4(nextActionTransmitter, 1, client, "/woonkamer/action4", "action4");
array<wallSwitch, 8> wallSwitches = {kaku0, kaku1, kaku2, kaku3, action1, action2, action3, action4};

SensorBridge sensorBridge = SensorBridge(D5, MAN_1200);

void callback(char* topic, byte* payload, unsigned int length) {
    static String message;
    for (int i = 0; i < length; i++) {
        message.concat((char)payload[i]);
    }
    client.notifyListeners(message, topic);
    message = "";  
}

void setup() {
    for(wallSwitch & wallSocket : wallSwitches){
        client.addListener(wallSocket);
    }
    client.setupWifi();
    client.setupConnections();
    client.addListener(alarm);
    client.addListener(bose);
    movementSensor.addListener(alarm);
    sensorBridge.addListener(client);

    ArduinoOTA.setHostname("Sensor Module");
    ArduinoOTA.setPassword((const char *)"c-S!*b52yU_QzcAr");
    ArduinoOTA.begin();

}

void loop() {
    ArduinoOTA.handle();

    client.checkForMessages();
    movementSensor.checkForMotion();

    if(buzzer.timeToUpdate()){
        buzzer.update();
    }

    if(climateSensor.timeToMeasure()){
        climateSensor.measureAndPublish();
    }

    sensorBridge();
}