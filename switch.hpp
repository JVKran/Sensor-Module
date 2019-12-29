#ifndef __SWITCH_HPP
#define __SWITCH_HPP

#include "Arduino.h"
#include "mqttClient.hpp"
#include <NewRemoteTransmitter.h>

class wallSwitch : public messageListener {
	private:
		NewRemoteTransmitter & transmitter;
		mqttClient & client;

		const int unit;
		bool state;
		const String topic;
		const String switchName;
	public:
		wallSwitch(NewRemoteTransmitter & transmitter, int unit, mqttClient & client, String topic, String switchName, bool state = false);

		void turnOn();
		void turnOff();
		void toggle();

		int getUnit();
		bool turnedOn();

		virtual void messageReceived(const String & receivedMessage, const char* topic = "") override;
};

#endif //__SWITCH_HPP