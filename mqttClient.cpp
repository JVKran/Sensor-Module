#include "mqttClient.hpp"

mqttClient::mqttClient(char* ssid, char* password, char* mqttServer, const char* topic, WiFiClient & espClient, notificationLed & led, piezoBuzzer & buzzer, const bool retainedMessages, const uint8_t qosLevel):
    ssid(ssid),
    password(password),
    mqttServer(mqttServer),
    topic(topic),
    client(espClient),
    retainedMessages(retainedMessages),
    qosLevel(qosLevel),
    led(led),
    buzzer(buzzer)
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
    led.setColor(color(1023, 0, 0));
    delay(10);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void mqttClient::setupConnections(){
    client.setServer(mqttServer, 1883);
    client.setCallback(callback);
    client.subscribe(topic, qosLevel);
    reconnect();                //Establish a connection by signing in with credentials.
}

void mqttClient::reconnect() {
    led.setColor(color(1023, 0, 0));
    while (!client.connected()) {
        if (client.connect("Client-ID", "Client-Username", "Client-Password")) {
            buzzer.turnOn(1000);
            delay(500);
            client.subscribe(topic);
            for (int i = 0; i < amountOfListeners; i++){
                listeners[i]->messageReceived("CONNECTED");
            }
            buzzer.turnOn(2000);
            delay(500);
            buzzer.turnOff();
            led.enableFlashing(color(0, 1023, 0), 5500, 1000);
            break;
        } else {
            delay(500);
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
    client.publish(topic, messageToSend, retainedMessages);
}
