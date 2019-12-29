#ifndef __ALARM_HPP
#define __ALARM_HPP

#include "Arduino.h"
#include "mqttClient.hpp"
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
        void checkForMotion();
};

class alarmSystem : public messageListener, public motionListener {
    private:
        mqttClient & client;
        String topic;

        String state;
        bool nightAlarm = false;
    public:
        alarmSystem(mqttClient & client, String topic, String state);

        void armAway();
        void armHome();
        void armNight();
        void disarm();
        void trigger();

        String getState();

        virtual void messageReceived(const String & receivedMessage, const char* topic = "") override;
        virtual void motionDetected() override;
};

#endif //__ALARM_HPP