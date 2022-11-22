#include <AnalogDevice.h>

AnalogDevice::AnalogDevice(PubSubClient& client, String topic){
    this->client = &client;
    this->topic = topic;
    this->topicToPersist = topic + String("/persist");
}

AnalogDevice::AnalogDevice(PubSubClient& client, String topic, int pin){
    this->client = &client;
    this->topic = topic;
    this->topicToPersist = topic + String("/persist");
    this->setPin(pin);
}

AnalogDevice::AnalogDevice(PubSubClient& client, String topic, int pin, int output){
    this->client = &client;
    this->topic = topic;
    this->topicToPersist = topic + String("/persist");
    this->setPin(pin);
    this->setOutput(output);
}

AnalogDevice::AnalogDevice(PubSubClient& client, String topic, int pin, int output, int channel, int freq, int resolution){
    this->client = &client;
    this->topic = topic;
    this->topicToPersist = topic + String("/persist");
    this->setPin(pin);
    this->channel = channel;
    this->setOutput(output);
    ledcSetup(channel, freq, resolution);
    ledcAttachPin(pin, channel);
}

const char* AnalogDevice::getTopic(){
    return topic.c_str();
}

const char* AnalogDevice::getTopicToPersist(){
    return topicToPersist.c_str();
}

int AnalogDevice::getOutput(){
    return output;
}

void AnalogDevice::setOutput(int output) {
    if(output >= 0 && output <= 256){
        this->output = output;
    }else {
        Serial.printf("Value %d not allowed!");
    }
}

void AnalogDevice::setOutputAndPublish(int output){
    this->setOutput(output);
    this->publish();
}

void AnalogDevice::setOutputAndPersist(int output){
    if(this->output != output){
        this->setOutput(output);
        this->persist();
    }
}

int AnalogDevice::getPin(){
    return pin;
}

void AnalogDevice::setPin(int pin){
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void AnalogDevice::publish() {
    doc["output"] = output;
    char payload[128];
    serializeJson(doc, payload);
    client->publish(topic.c_str(), payload);
}

void AnalogDevice::persist() {
    doc["output"] = output;
    char payload[128];
    serializeJson(doc, payload);
    client->publish(this->topicToPersist.c_str(), payload);
}

void AnalogDevice::subscribe() {
    client->subscribe(this->getTopic());
}

boolean AnalogDevice::compareAndUpdate(char* topic, byte* payload, unsigned int length){
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
        this->setOutput(doc["output"]);
        return true;
    } else{
        return false;
    }
}

void AnalogDevice::ledcWriteAndPublish() {
    ledcWrite(channel, output);
    this->publish();
}

void AnalogDevice::ledcWriteAndPersist() {
    ledcWrite(channel, output);
    this->persist();
}

void AnalogDevice::write() {
    digitalWrite(pin, output);
}

void AnalogDevice::writeAndPublish() {
    digitalWrite(pin, output);
    this->publish();
}

void AnalogDevice::writeAndPersist() {
    digitalWrite(pin, output);
    this->persist();
}