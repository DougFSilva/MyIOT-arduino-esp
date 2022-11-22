#ifndef _MEASURINGDEVICE_H
#define _MEASURINGDEVICE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

class MeasuringDevice {
    private: 
        PubSubClient* client;
        String topic;  
        String topicToPersist;
        StaticJsonDocument<200> doc;
        JsonArray values;

    public:
        MeasuringDevice(PubSubClient& client, String topic);
        MeasuringDevice addValue(double value);
        const char* getTopic();
        const char* getTopicToPersist();
        void publish();
        void publishAndClearValues();
        void persist();
        void persistAndClearValues();
        void subscribe();
        void clearValues();
        
};


#endif