#ifndef _DISCRETEDEVICE_H
#define _DISCRETEDEVICE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

class DiscreteDevice {
    private: 
        PubSubClient* client;
        String topic;  
        String topicToPersist;
        StaticJsonDocument<200> doc;
        boolean status;
        int pin;

    public:
        DiscreteDevice(PubSubClient& client, String topic);
        DiscreteDevice(PubSubClient& client, String topic, int pin);
        DiscreteDevice(PubSubClient& client, String topic, int pin, boolean status);
        const char* getTopic();
        const char* getTopicToPersist();
        boolean getStatus();
        void setStatus(boolean status);
        void setStatusAndPublish(boolean status);
        void setStatusAndPersist(boolean status);
        int getPin();
        void setPin(int pin);
        void publish();
        void persist();
        void subscribe();
        boolean compareAndUpdate(char* topic, byte* payload, unsigned int length);
        void write();
        void writeAndPublish();
        void writeAndPersist();
};


#endif