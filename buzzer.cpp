#include "buzzer.hpp"

piezoBuzzer::piezoBuzzer(const uint8_t buzzerPin):
	buzzerPin(buzzerPin)
{
	pinMode(buzzerPin, OUTPUT);
}

void piezoBuzzer::turnOn(const uint16_t frequency){
	tone(buzzerPin, frequency);
}

void piezoBuzzer::turnOff(){
	noTone(buzzerPin);
}

void piezoBuzzer::update(){
	if(sirenEnabled){
		if(millis() > lastFrequencyChange + changeFrequencyPeriod){
			lastFrequencyChange = millis();
			if(lastFrequencyHigh){
				tone(buzzerPin, lowFrequency);
			} else {
				tone(buzzerPin, highFrequency);
			}
			lastFrequencyHigh = !lastFrequencyHigh;
		}
	}
}

void piezoBuzzer::enableSiren(const uint16_t desiredHighFrequency, const unsigned long changePeriod, const uint16_t desiredLowFrequency){
	highFrequency = desiredHighFrequency;
	lowFrequency = desiredLowFrequency;
	sirenEnabled = true;
	changeFrequencyPeriod = changePeriod;
}

void piezoBuzzer::disableSiren(){
	turnOff();
	sirenEnabled = false;
}
