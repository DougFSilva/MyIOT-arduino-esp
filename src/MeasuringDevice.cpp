#include <MeasuringDevice.h>

MeasuringDevice::MeasuringDevice(PubSubClient& client, String topic){
    this->client = &client;
    this->topic = topic;
    this->topicToPersist = topic + String("/persist");
    this->values = this-> doc.createNestedArray("values");
}

const char* MeasuringDevice::getTopic(){
    return this->topic.c_str();
}

const char* MeasuringDevice::getTopicToPersist(){
    return this->topicToPersist.c_str();
}


MeasuringDevice MeasuringDevice::addValue(double value){
    this->values.add(value);
    return *this;
}

void MeasuringDevice::publish() {
    char payload[128];
    serializeJson(doc, payload);
    client->publish(this->topic.c_str(), payload);
}

void MeasuringDevice::publishAndClearValues() {
    char payload[128];
    serializeJson(doc, payload);
    client->publish(this->topic.c_str(), payload);
    this->clearValues();
}

void MeasuringDevice::persist() {
    char payload[128];
    serializeJson(doc, payload);
    client->publish(this->topicToPersist.c_str(), payload);
}

void MeasuringDevice::persistAndClearValues() {
    char payload[128];
    serializeJson(doc, payload);
    client->publish(this->topicToPersist.c_str(), payload);
    this->clearValues();
}

void MeasuringDevice::subscribe(){
    client->subscribe(this->getTopic());
}

void MeasuringDevice::clearValues() {
    this->doc.clear();
    this->values = this-> doc.createNestedArray("values");
}
