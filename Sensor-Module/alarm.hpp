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

        piezoBuzzer & buzzer;

        void disableActuators();
    public:
        alarmSystem(mqttClient & client, String topic, piezoBuzzer & buzzer, String state = "disarmed");

        virtual void messageReceived(const String & receivedMessage, const char* topic = "") override;
        virtual void motionDetected() override;

        String getState() const;

        void armAway();
        void armHome();
        void armNight();
        void disarm();
        void trigger();

};

#endif //__ALARM_HPP
