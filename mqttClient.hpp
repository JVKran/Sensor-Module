#ifndef __CONNECTIONS_HPP
#define __CONNECTIONS_HPP

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "array.hpp"
#include "buzzer.hpp"

class messageListener {
    public:
        virtual void messageReceived(const String & receivedMessage, const char* topic = "") = 0;
};

void callback(char* topic, byte* payload, unsigned int length);

class mqttClient {
    private:
        char* ssid;
        char* password;
        char* mqttServer;
        
        const char* topic;
        PubSubClient client;
        const bool retainedMessages;
        const uint8_t qosLevel;

        piezoBuzzer & buzzer;
    public:
        messageListener * listeners[20] = {};
        uint8_t amountOfListeners = 0;

        mqttClient(char* ssid, char* password, char* mqttServer, const char* topic, WiFiClient & espClient, piezoBuzzer & buzzer, const bool retainedMessages = true, const uint8_t qosLevel = 1);

        void addListener(messageListener & listener);
        void notifyListeners(const String & mesage, const char* topic);

        void setupWifi();
        void setupConnections();
        void reconnect();

        void checkForMessages();
        void sendMessage(const char* topic, const char* messageToSend);
};

#endif //__CONNECTIONS_HPP