#ifndef __CLIMATE_HPP
#define __CLIMATE_HPP

#include <dht11.h>
#include "mqttClient.hpp"

class tempHumSensor {
	private:
		dht11 sensor;
		const uint8_t sensorPin;

		const unsigned int pollPeriod;
		unsigned long lastMeasurement = 0;

		mqttClient & client;
	public:
		tempHumSensor(mqttClient & client, const uint8_t sensorPin, const unsigned int pollPeriod);

		void operator()();
		void measureAndPublish();
};

#endif //__CLIMATE_HPP