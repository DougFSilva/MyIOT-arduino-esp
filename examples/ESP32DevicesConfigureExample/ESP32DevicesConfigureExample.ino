#include <WiFi.h>
#include <PubSubClient.h>
#include <MeasuringDevice.h>
#include <DiscreteDevice.h>
#include <AnalogDevice.h>
#include <dht.h>
#define dht_dpin 15
#define ESP 32
dht dht;

//------------------------- Wifi properties -------------------------//
char ssid[] = "your wifi SSID";         // your SSID
char pass[] = "yout wifi password";     // your SSID Password

//------------------------- Mqtt Properties -------------------------//
const char* mqttBroker = "your broker address";
const char* mqttUser = "your user Myiot";
const char* mqttPassword = "your password Myiot";
int mqttPort = 1883; //your mqtt broker port

WiFiClient espClient;
PubSubClient mqtt(espClient);

//-------------------------- Devices configure ----------------------//
MeasuringDevice tempUmidity(mqtt, "iot/MeasuringDevice/6359913cee0ef433e441b2ee");
DiscreteDevice lamp(mqtt, "iot/lamp/63599283ee0ef433e441b35b", 13, true); // constructor(PubSubClient, topic, pin, status)
AnalogDevice motor(mqtt, "iot/AnalogOutputDevice/63599265ee0ef433e441b344", 2, 150, 0, 5000, 8); // constructor for using PWM in ESP32 
                                                                                                        //(PubSubClient, topic, pin, output, channel, freq, resolution)

unsigned long dhtDelay;
unsigned long loopDelay;

void setup()
{
  Serial.begin(115200);
  wifiConnect();
  mqttConnect();
  motor.subscribe();
  lamp.subscribe();
  tempUmidity.subscribe();
  motor.ledcWriteAndPersist();//Execute ledcWrite and persist
  lamp.writeAndPersist();//Execute digitalWrite and persist
}

void loop(){
  dhtMeasure();
  mqtt.loop();
  if((millis() - loopDelay)>30000){
    verifyConnection();
  }
}

void dhtMeasure(){
  if((millis() - dhtDelay)>60000) {
    dht.read11(dht_dpin);
    double temp = dht.temperature;
    double umid = dht.humidity;
    tempUmidity.addValue(temp);
    tempUmidity.addValue(umid);
    tempUmidity.persistAndClearValues();
    Serial.print("Temperatura: ");
    Serial.println(temp);
    Serial.print("Umidade: ");
    Serial.println(umid);
    dhtDelay = millis();
  }
}
    
void wifiConnect() {
  Serial.printf("\nConnecting to %s", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to network");
  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());
}

void mqttConnect() {
  mqtt.setServer(mqttBroker, mqttPort);
  mqtt.setKeepAlive(300);
  mqtt.setCallback(callback);
  String clientId = "ClientId-";
  clientId += String(random(0xffff), HEX);
  while (!mqtt.connected()) {
    Serial.println("Connecting to MQTT…");
    if (mqtt.connect(clientId.c_str(), mqttUser, mqttPassword )) {
        Serial.println("connected");
    } else {
        Serial.print("failed with state ");
        Serial.print(mqtt.state());
        delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  if(motor.compareAndUpdate(topic, payload,length)){
     motor.ledcWriteAndPersist();
  } else if(lamp.compareAndUpdate(topic, payload,length)){
     lamp.writeAndPersist();
  }
}

void verifyConnection(){
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Wifi disconnected!");
    wifiConnect();
    mqttConnect();
  }
    if(!mqtt.connected()){
    Serial.println("Mqtt disconnected!");
    mqttConnect();
  }
}
