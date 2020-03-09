#include "alarm.hpp"

alarmSystem::alarmSystem(mqttClient & client, String topic, piezoBuzzer & buzzer, String state):
    client(client),
    topic(topic), 
    state(state),
    buzzer(buzzer)
{}

void alarmSystem::disableActuators(){
    buzzer.disableSiren();
}

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
    if(state != "triggered"){
        state = "triggered";
        client.sendMessage(topic.c_str(), state.c_str());
        if(!nightAlarm){
            buzzer.enableSiren(2000, 500, 1000);
        }
    }
}

void alarmSystem::messageReceived(const String & receivedMessage, const char* topic){
    if(receivedMessage.startsWith("ARM") || receivedMessage.endsWith("ARM")){
        if(receivedMessage.endsWith("AWAY")){
            armAway();
        } else if(receivedMessage.endsWith("HOME")){
            armHome();
        } else if(receivedMessage.endsWith("NIGHT")){
            armNight();
        } else if(receivedMessage == "DISARM"){
            disarm();
        }
        disableActuators();
    }
}

void alarmSystem::motionDetected(){
    if(state != "disarmed"){
        trigger();
    }
}