#ifndef _ANALOGDEVICE_H
#define _ANALOGDEVICE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

class AnalogDevice {
    private: 
        PubSubClient* client;
        String topic;  
        String topicToPersist;
        StaticJsonDocument<200> doc;
        int output;
        int pin;
        int channel;

    public:
        AnalogDevice(PubSubClient& client, String topic);
        AnalogDevice(PubSubClient& client, String topic, int pin);
        AnalogDevice(PubSubClient& client, String topic, int pin, int output);
        AnalogDevice(PubSubClient& client, String topic, int pin, int output, int channel, int freq, int resolution);
        const char* getTopic();
        const char* getTopicToPersist();
        int getOutput();
        void setOutput(int output);
        void setOutputAndPublish(int output);
        void setOutputAndPersist(int output);
        int getPin();
        void setPin(int pin);
        void publish();
        void persist();
        void subscribe();
        boolean compareAndUpdate(char* topic, byte* payload, unsigned int length);
        void ledcWriteAndPublish();
        void ledcWriteAndPersist();
        void write();
        void writeAndPublish();
        void writeAndPersist();
};


#endif