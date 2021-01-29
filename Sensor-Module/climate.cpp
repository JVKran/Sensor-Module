#include "climate.hpp"

tempHumSensor::tempHumSensor(mqttClient & client, const unsigned int pollPeriod):
	pollPeriod(pollPeriod),
	client(client)
{}

void tempHumSensor::begin(){
	Wire.begin();
	climateSensor.begin();
}

void tempHumSensor::operator()(){
	if(millis() > lastMeasurement + pollPeriod){
		measureAndPublish();
		lastMeasurement = millis();
	}
}

void tempHumSensor::measureAndPublish(){
	climateSensor.takeForcedMeasurement();
	client.sendMessage("/woonkamer/vochtigheid", String(climateSensor.getRelativeHumidity()).c_str());
	client.sendMessage("/woonkamer/temperatuur", String(climateSensor.getTemperatureCelcius()).c_str());
}
