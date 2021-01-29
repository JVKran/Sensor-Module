#ifndef __CLIMATE_HPP
#define __CLIMATE_HPP

#include <forcedClimate.h>
#include "mqttClient.hpp"

class tempHumSensor {
	private:
		ForcedClimate climateSensor = ForcedClimate();

		const unsigned int pollPeriod;
		unsigned long lastMeasurement = 0;

		mqttClient & client;
	public:
		tempHumSensor(mqttClient & client, const unsigned int pollPeriod);
		void begin();

		void operator()();
		void measureAndPublish();
};

#endif //__CLIMATE_HPP