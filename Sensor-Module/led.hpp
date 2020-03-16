#ifndef __LED_HPP
#define __LED_HPP

#include "Arduino.h"

class color {
    public:
        uint16_t red;
        uint16_t green;
        uint16_t blue;

        color(const uint16_t red = 0, const uint16_t green = 0, const uint16_t blue = 0);
};

class notificationLed {
    private:
        const uint8_t redPin;
        const uint8_t greenPin;
        const uint8_t bluePin;

        color lastColor = color();
        const uint16_t updatePeriod;
        unsigned long lastUpdate = 0;

        unsigned int flashEndTime;
        unsigned int flashPeriod;
        color flashColor = color();
        color offColor = color();
        bool flashingEnabled = false;
        bool lastFlashOn = false;
        unsigned long lastFlash;
    public:
        notificationLed(const uint8_t redPin = 3, const uint8_t greenPin = 1, const uint8_t bluePin = D5, const uint16_t updatePeriod = 250, const color defaultColor = color(0, 0, 0));

        void setColor(const color & wantedColor);

        void turnOff();
        void turnOn();

        void enableFlashing(const color & desiredFlashColor, const unsigned int flashDuration, const unsigned int desiredFlashPeriod, const color & desiredOffColor = color());
        void disableFlashing();
        
        void update();
        bool timeToUpdate();
};

#endif //__LED_HPP