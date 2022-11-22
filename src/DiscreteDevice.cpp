#include <DiscreteDevice.h>


DiscreteDevice::DiscreteDevice(PubSubClient& client, String topic){
    this->client = &client;
    this->topic = topic;
    this->topicToPersist = topic + String("/persist");
}

DiscreteDevice::DiscreteDevice(PubSubClient& client, String topic, int pin){
    this->client = &client;
    this->topic = topic;
    this->topicToPersist = topic + String("/persist");
    this->setPin(pin);
}

DiscreteDevice::DiscreteDevice(PubSubClient& client, String topic, int pin, boolean status){
    this->client = &client;
    this->topic = topic;
    this->topicToPersist = topic + String("/persist");
    this->setPin(pin);
    this->setStatus(status);
}

const char* DiscreteDevice::getTopic(){
    return topic.c_str();
}

const char* DiscreteDevice::getTopicToPersist(){
    return topicToPersist.c_str();
}

boolean DiscreteDevice::getStatus(){
    return status;
}

void DiscreteDevice::setStatus(boolean status) {
    this->status = status;
}

void DiscreteDevice::setStatusAndPublish(boolean status){
    this->setStatus(status);
    this->publish();
}

void DiscreteDevice::setStatusAndPersist(boolean status){
    this->setStatus(status);
    this->persist();
}

int DiscreteDevice::getPin(){
    return pin;
}

void DiscreteDevice::setPin(int pin){
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void DiscreteDevice::publish() {
    doc["status"] = status;
    char payload[128];
    serializeJson(doc, payload);
    client->publish(this->topic.c_str(), payload);
}

void DiscreteDevice::persist() {
    doc["status"] = status;
    char payload[128];
    serializeJson(doc, payload);
    client->publish(this->topicToPersist.c_str(), payload);
}

void DiscreteDevice::subscribe() {
    client->subscribe(this->getTopic());
}

boolean DiscreteDevice::compareAndUpdate(char* topic, byte* payload, unsigned int length){
    if(this->topic == String(topic)){
        String msg;
        for(int i = 0; i < length; i++) 
        {
            char c = (char)payload[i];
            msg += c;
        }
        DeserializationError error = deserializeJson(doc, msg.c_str());
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
        return false;
        }
        this->status = doc["status"];
        return true;
    } else{
        return false;
    }
}

void DiscreteDevice::write(){
    digitalWrite(pin, status);
}

void DiscreteDevice::writeAndPublish(){
    this->write();
    this->publish();
}

void DiscreteDevice::writeAndPersist(){
    this->write();
    this->persist();
}

