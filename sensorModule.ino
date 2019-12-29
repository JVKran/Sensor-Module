#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NewRemoteTransmitter.h>
#include "speaker.hpp"
#include "alarm.hpp"
#include "switch.hpp"
#include "array.hpp"
#include "mqttClient.hpp"

WiFiClient espClient;
mqttClient client("SSID", "WPA", "IP", "/raspberrypi/hassio", espClient);

NewRemoteTransmitter kakuTransmitter(20589486, D3, 257, 3);
NewRemoteTransmitter actionTransmitter(54973440, D3, 261, 3);

speaker bose(D0, client);

alarmSystem alarm(client, "/woonkamer/alarm", "disarmed");
motionSensor sensor(D1);

wallSwitch kaku0(kakuTransmitter, 0, client, "/woonkamer/kaku0", "kaku0");
wallSwitch kaku1(kakuTransmitter, 1, client, "/woonkamer/kaku1", "kaku1");
wallSwitch kaku2(kakuTransmitter, 2, client, "/woonkamer/kaku2", "kaku2");
wallSwitch kaku3(kakuTransmitter, 3, client, "/woonkamer/kaku3", "kaku3");
wallSwitch action1(actionTransmitter, 1, client, "/woonkamer/action1", "action1");
wallSwitch action2(actionTransmitter, 2, client, "/woonkamer/action2", "action2");
wallSwitch action3(actionTransmitter, 3, client, "/woonkamer/action3", "action3");
array<wallSwitch, 7> wallSwitches = {kaku0, kaku1, kaku2, kaku3, action1, action2, action3};

void callback(char* topic, byte* payload, unsigned int length) {
    static String message;
    for (int i = 0; i < length; i++) {
        message.concat((char)payload[i]);
    }
    client.notifyListeners(message, topic);
    message = "";  
}

void setup() {
    WiFi.mode(WIFI_STA);
    client.setupWifi();
    client.setupConnections();
    for(wallSwitch & wallSocket : wallSwitches){
        client.addListener(wallSocket);
    }
    client.addListener(alarm);
    sensor.addListener(alarm);
}

void loop() {
    client.checkForMessages();
    sensor.checkForMotion();
}
