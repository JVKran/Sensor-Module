#include "mqttClient.hpp"

mqttClient::mqttClient(char* ssid, char* password, char* mqttServer, const char* topic, WiFiClient & espClient, piezoBuzzer & buzzer, const bool retainedMessages, const uint8_t qosLevel):
    ssid(ssid),
    password(password),
    mqttServer(mqttServer),
    topic(topic),
    client(espClient),
    retainedMessages(retainedMessages),
    qosLevel(qosLevel),
    buzzer(buzzer)
{
    WiFi.mode(WIFI_STA);
}

void mqttClient::addListener(messageListener & listener){
    if(amountOfListeners < 20){
        listeners[amountOfListeners++] = &listener;
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
    client.subscribe(topic, qosLevel);
    reconnect();                //Establish a connection by signing in with credentials.
}

void mqttClient::reconnect() {
    while (!client.connected()) {
        if (client.connect("ESP8266Client", "Arduino", "Snip238!")) {
            buzzer.turnOn(1000);
            delay(500);
            client.subscribe(topic);
            for (int i = 0; i < amountOfListeners; i++){
                listeners[i]->messageReceived("CONNECTED");
            }
            buzzer.turnOn(2000);
            delay(500);
            buzzer.turnOff();
            break;
        } else {
            delay(500);
        }
    }
}

void mqttClient::operator()(){
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

void mqttClient::sendMessage(const char* topic, const char* messageToSend){
    client.publish(topic, messageToSend, retainedMessages);
}

void mqttClient::dataReceived(const uint8_t sensorId, const bool motion, const bool reedState, const int16_t temperature, const int16_t humidity, const int32_t pressure, const uint16_t voltage, const uint16_t lightIntensity){
    String temp = String(temperature).substring(0, 2);
    String commaTemp = String(temperature).substring(2);

    String hum = String(humidity).substring(0, 2);
    String commaHum = String(humidity).substring(2);

    String press = String(pressure).substring(0, 4);
    String commaPress = String(pressure).substring(4);

    client.publish(String("/sensormodules/" + String(sensorId) + "/motion").c_str(),      String(motion).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/reed").c_str(),          String(reedState).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/temperature").c_str(),   String(temp + '.' + commaTemp).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/humidity").c_str(),   String(hum + '.' + commaHum).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/pressure").c_str(),     String(press + '.' + commaPress).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/voltage").c_str(),      String(voltage).c_str());
    client.publish(String("/sensormodules/" + String(sensorId) + "/illuminance").c_str(),  String(lightIntensity).c_str());
}
