#ifndef __BUZZER_HPP
#define __BUZZER_HPP

#include "Arduino.h"

class piezoBuzzer {
	private:
		const uint8_t buzzerPin;

		uint16_t highFrequency = 0;
		uint16_t lowFrequency = 0;
		bool sirenEnabled = false;
		bool lastFrequencyHigh = false;
		unsigned long lastFrequencyChange = 0;
		unsigned int changeFrequencyPeriod;

		const uint16_t updatePeriod;
        unsigned long lastUpdate = 0;
	public:
		piezoBuzzer(const uint8_t buzzerPin, const uint16_t updatePeriod = 250);

		void turnOn(const uint16_t frequency = 1000);
		void turnOff();

		void handleSirenChange();
		void enableSiren(const uint16_t desiredHighFrequency, const unsigned long changePeriod = 1000, const uint16_t desiredLowFrequency = 0);
		void disableSiren();

		void operator()();
};

#endif //__BUZZER_HPP
