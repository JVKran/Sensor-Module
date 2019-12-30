#ifndef __SPEAKER_HPP
#define __SPEAKER_HPP

#include "Arduino.h"
#include "mqttClient.hpp"
#include <IRsend.h>

class speaker : public messageListener {
	private:
		mqttClient & client;

		const uint8_t & irPin;
		bool state;
		IRsend irTransmitter;
	public:
		speaker(const uint8_t & irPin, mqttClient & client, bool state = false);

		void togglePower();
		bool turnedOn();

		void increaseVolume();
		void lowerVolume();
		void muteVolume();

		virtual void messageReceived(const String & receivedMessage, const char* topic = "");
};

#endif //__SPEAKER_HPP