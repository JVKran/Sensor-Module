#ifndef __ALARM_HPP
#define __ALARM_HPP

#include "Arduino.h"
#include "mqttClient.hpp"
#include "array.hpp"
#include "motion.hpp"
#include "buzzer.hpp"

class alarmSystem : public messageListener, public motionListener {
    private:
        mqttClient & client;
        String topic;

        String state;
        bool nightAlarm = false;

        notificationLed & led;
        piezoBuzzer & buzzer;

        void disableActuators();
    public:
        alarmSystem(mqttClient & client, String topic, notificationLed & led, piezoBuzzer & buzzer, String state = "disarmed");

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