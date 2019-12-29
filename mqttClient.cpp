#include "mqttClient.hpp"

mqttClient::mqttClient(char* ssid, char* password, char* mqttServer, const char* topic, WiFiClient & espClient):
    ssid(ssid),
    password(password),
    mqttServer(mqttServer),
    topic(topic),
    client(espClient)
{
    WiFi.mode(WIFI_STA);
}

void mqttClient::addListener(messageListener & listener){
    if(amountOfListeners <= 19){
        listeners[amountOfListeners] = &listener;
        amountOfListeners++;
    }
}

void mqttClient::notifyListeners(const String & message, const char* topic){
	for (int i = 0; i < amountOfListeners; i++){
        listeners[i]->messageReceived(message, topic);
    }
}

void mqttClient::setupWifi() {
    delay(10);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void mqttClient::setupConnections(){
    client.setServer(mqttServer, 1883);
    client.setCallback(callback);
    client.subscribe(topic);
    reconnect();                //Establish a connection by signing in with credentials.
}

void mqttClient::reconnect() {
    while (!client.connected()) {
        if (client.connect("ESP8266Client", "Arduino", "Snip238!")) {
            client.subscribe(topic);
            for (int i = 0; i < amountOfListeners; i++){
                listeners[i]->messageReceived("CONNECTED");
            }
            break;
        } else {
            delay(1000);
        }
    }
}

void mqttClient::checkForMessages(){
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

void mqttClient::sendMessage(const char* topic, const char* messageToSend){
    client.publish(topic, messageToSend);
}