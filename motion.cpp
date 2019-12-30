#include "motion.hpp"

motionSensor::motionSensor(const uint8_t motionPin):
    motionPin(motionPin)
{}

void motionSensor::checkForMotion(){
    if(digitalRead(motionPin) == 1){
        for (uint8_t i = 0; i < amountOfListeners; i++){
            listeners[i]->motionDetected();
        }
    }
}

void motionSensor::addListener(motionListener & listener){
    listeners[amountOfListeners++] = &listener;
} 