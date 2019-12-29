#include "speaker.hpp"

speaker::speaker(const uint8_t & irPin, mqttClient & client, bool state): 
	client(client),
	irPin(irPin),
	state(state),
	irTransmitter(irPin)
{
	irTransmitter.begin();
}
		
void speaker::togglePower(){
	state = !state;
	if (state){
		client.sendMessage("/woonkamer/BOSE", "on");
	} else {
		client.sendMessage("/woonkamer/BOSE", "off");
	}
	for (int i = 0; i < 5; i++) {
		irTransmitter.sendNEC(0x5D0532CD, 32);
		delay(40);
	}
}

bool speaker::turnedOn(){
	return  state;
}

void speaker::increaseVolume(){
	for (int i = 0; i < 3; i++) {
		irTransmitter.sendNEC(0x5D05C03F, 32);
		delay(40);
	}
}

void speaker::lowerVolume(){
	for (int i = 0; i < 3; i++) {
		irTransmitter.sendNEC(0x5D0540BF, 32);
		delay(40);
	}
}

void speaker::muteVolume(){
	for (int i = 0; i < 3; i++) {
		irTransmitter.sendNEC(0x5D05807F, 32);
		delay(40);
	}
}

void speaker::messageReceived(const String & receivedMessage, const char* topic){
	if (receivedMessage == "verst_aan"){
		togglePower();
	} else if(receivedMessage=="verst_laag"){
		lowerVolume();
	} else if(receivedMessage=="verst_hoog"){
		increaseVolume();
	} else if(receivedMessage=="verst_demp"){
		muteVolume();
	}
}