#include "alarm.hpp"

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

alarmSystem::alarmSystem(mqttClient & client, String topic, String state):
    client(client),
    topic(topic), 
    state(state)
{}

void alarmSystem::armAway(){
    state = "armed_away";
    client.sendMessage(topic.c_str(), state.c_str());
    nightAlarm = false;
}

void alarmSystem::armHome(){
    state = "armed_home";
    client.sendMessage(topic.c_str(), state.c_str());
    nightAlarm = false;
}

void alarmSystem::armNight(){
    state = "armed_night";
    client.sendMessage(topic.c_str(), state.c_str());
    nightAlarm = true;
}

void alarmSystem::disarm(){
    state = "disarmed";
    client.sendMessage(topic.c_str(), state.c_str());
    nightAlarm = false;
}

String alarmSystem::getState(){
    return state;
}

void alarmSystem::trigger(){
    state = "triggered";
    client.sendMessage(topic.c_str(), state.c_str());
}

void alarmSystem::messageReceived(const String & receivedMessage, const char* topic){
    if(receivedMessage=="ARM_AWAY"){
        armAway();
    } else if(receivedMessage=="ARM_HOME"){
        armHome();
    } else if(receivedMessage=="ARM_NIGHT"){
        armNight();
    } else if(receivedMessage=="DISARM"){
        disarm();
    }
}

void alarmSystem::motionDetected(){
    if(state != "disarmed"){
        trigger();
    }
}