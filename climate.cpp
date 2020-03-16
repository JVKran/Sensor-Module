#include "climate.hpp"

tempHumSensor::tempHumSensor(mqttClient & client, const uint8_t sensorPin, const unsigned int pollPeriod):
	sensorPin(sensorPin),
	pollPeriod(pollPeriod),
	client(client)
{}

void tempHumSensor::operator()(){
	if(millis() > lastMeasurement + pollPeriod){
		measureAndPublish();
		lastMeasurement = millis();
	}
}

void tempHumSensor::measureAndPublish(){
	sensor.read(sensorPin);
	client.sendMessage("/woonkamer/vochtigheid", String(float(sensor.humidity)).c_str());
	client.sendMessage("/woonkamer/temperatuur", String(float(sensor.temperature)).c_str());
}
