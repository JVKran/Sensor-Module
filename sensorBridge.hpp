#ifndef __SENSOR_BRIDGE_HPP
#define __SENSOR_BRIDGE_HPP

#include <Manchester.h>

class SensorListener {
	public:
		virtual void dataReceived(const uint8_t sensorId, const bool motion, const int16_t temperature, const int16_t humidity, const int32_t pressure) = 0;
};

class SensorBridge {
	private:
		Manchester & receiver;
		uint8_t buffer[11];

		SensorListener * listeners[2] = {};
        uint8_t amountOfListeners = 0;

        void notifyListeners(const uint8_t sensorId, const bool motion, const int16_t temperature, const int16_t humidity, const int32_t pressure){
        	for(uint8_t i = 0; i < amountOfListeners; i++){
        		listeners[i]->dataReceived(sensorId, motion, temperature, humidity, pressure);
        	}
        }
	public:
		SensorBridge(const uint8_t receivePin, const uint8_t baudrate);

		void addListener(SensorListener & listenerToAdd){
			listeners[amountOfListeners++] = &listenerToAdd;
		}

		void operator()();
};

#endif //__SENSOR_BRIDGE_HPP