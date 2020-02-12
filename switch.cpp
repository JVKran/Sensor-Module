#include "switch.hpp"

wallSwitch::wallSwitch(NewRemoteTransmitter & transmitter, int unit, mqttClient & client, String topic, String switchName, bool state):
	transmitter(transmitter),
	client(client),
	unit(unit), 
	state(state), 
	topic(topic),
	switchName(switchName)
{}

void wallSwitch::turnOn(){
	if(!turnedOn()){
		state = true;
		client.sendMessage(topic.c_str(), "on");
		transmitter.sendUnit(unit, state);	
	}
}

void wallSwitch::turnOff(){
	if(turnedOn()){
		state = false;
		client.sendMessage(topic.c_str(), "off");
		transmitter.sendUnit(unit, state);
	}
}

void wallSwitch::toggle(){
	state = !state;
	if(state){
		client.sendMessage(topic.c_str(), "on");
	} else {
		client.sendMessage(topic.c_str(), "off");
	}
	transmitter.sendUnit(unit, state);
}

int wallSwitch::getUnit(){
	return unit;
}

bool wallSwitch::turnedOn(){
	return state;
}

void wallSwitch::messageReceived(const String & receivedMessage, const char* topic){
	if(receivedMessage.startsWith(switchName)){
		if(receivedMessage.endsWith("aan")){
			turnOn();
		} else {
			turnOff();
		}
	}
}