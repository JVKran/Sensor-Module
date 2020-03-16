#include "led.hpp"

color::color(const uint16_t red, const uint16_t green, const uint16_t blue):
	red(red),
	green(green),
	blue(blue)
{}

notificationLed::notificationLed(const uint8_t redPin, const uint8_t greenPin, const uint8_t bluePin, const uint16_t updatePeriod, const color defaultColor): 
	redPin(redPin),
	greenPin(greenPin),
	bluePin(bluePin),
	updatePeriod(updatePeriod)
{
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    setColor(defaultColor);
}

void notificationLed::setColor(const color & wantedColor) {
    if(wantedColor.red <= 1023 && wantedColor.green <= 1023 && wantedColor.blue <= 1023){
        analogWrite(redPin, 1023-wantedColor.red);
        analogWrite(greenPin, 1023-wantedColor.green);
        analogWrite(bluePin, 1023-wantedColor.blue);
        lastColor = color(wantedColor.red, wantedColor.green, wantedColor.blue);
    } else {
    	setColor(color(1023, 1023, 1023));
    }
}

void notificationLed::turnOff(){
    setColor(color(0,0,0));
}

void notificationLed::turnOn(){
	setColor(lastColor);
}

void notificationLed::enableFlashing(const color & desiredFlashColor, const unsigned int flashDuration, const unsigned int desiredFlashPeriod, const color & desiredOffColor){
	flashColor = desiredFlashColor;
	offColor = desiredOffColor;
	flashPeriod = desiredFlashPeriod;
	flashEndTime = millis() + flashDuration;
	flashingEnabled = true;
	lastFlash = 0;
}

void notificationLed::disableFlashing(){
	flashingEnabled = false;
	turnOff();
}

void notificationLed::update(){
	lastUpdate = millis();
	if(flashingEnabled){
		if(millis() > lastFlash + flashPeriod){
			if(lastFlashOn){
				setColor(offColor);
			} else {
				setColor(flashColor);
			}
			lastFlashOn = !lastFlashOn;
			lastFlash = millis();
		}
		if(millis() > flashEndTime){
			flashingEnabled = false;
			setColor(color(0, 0, 0));
		}
	}
}

bool notificationLed::timeToUpdate(){
	return (millis() > lastUpdate + updatePeriod);
}
