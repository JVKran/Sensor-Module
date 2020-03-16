#ifndef __MOTION_HPP
#define __MOTION_HPP

#include "Arduino.h"
#include "array.hpp"

class motionListener {
    public:
        virtual void motionDetected() = 0;
};

class motionSensor {
    private:
        const uint8_t motionPin;

        array<motionListener*, 5> listeners;
        uint8_t amountOfListeners = 0;
    public:
        motionSensor(const uint8_t motionPin);

        void addListener(motionListener & listener);
        void operator()();
};

#endif //__MOTION_HPP