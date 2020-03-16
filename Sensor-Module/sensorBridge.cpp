#include "sensorBridge.hpp"

SensorBridge::SensorBridge(const uint8_t receivePin, const uint8_t baudrate):
	receiver(man)
{
	receiver.setupReceive(receivePin, baudrate);
	receiver.beginReceiveArray(BUFFER_SIZE, buffer);
}

void SensorBridge::operator()(){
	if(receiver.receiveComplete()){
		uint8_t sensorId = buffer[1];
		bool motion = buffer[2];
		int16_t temperature = buffer[3] + (buffer[4] << 8);
		int16_t humidity = buffer[5] + (buffer[6] << 8);
		int32_t pressure = buffer[7] + ((buffer[8] << 8) + ((buffer[9] << 16) + (buffer[10] << 24)));
		uint16_t voltage = buffer[11] + (buffer[12] << 8);
		notifyListeners(sensorId, motion, temperature, humidity, pressure, voltage);
		receiver.beginReceiveArray(BUFFER_SIZE, buffer);
	}
}