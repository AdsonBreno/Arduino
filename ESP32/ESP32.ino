#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

//Digital Inputs
#define sPin 22 //Sensor
#define aPin 4 //Analogic
#define magPin 21 //Magnetic

//Digital Outputs
#define pulsePin 15
#define lockPin 2

//ID MQTT
#define pubMag "MAGS"
#define subLock "DOOR"
#define TOPIC_PUBLISH "HALLWAY"

WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);
PubSubClient MQTT2(wifiClient);

//Consts
const char* ssid = "INTELBRAS";
const char* pass = "marcia12345";
const char* broker = "public.mqtthq.com";

int port = 1883;
int pState = HIGH;
int state, analogic, magnetic;

void connectWiFi();
void connectMQTT();
void keepConnections();
void receivePacket(char* topic, byte* payload, unsigned int length);

void setup() {
  Serial.begin(9600);
  pinMode(pulsePin, OUTPUT);
  pinMode(magPin, INPUT_PULLUP);
  digitalWrite(lockPin, OUTPUT);
  
  connectWiFi();
  MQTT.setServer(broker, port);
  MQTT2.setServer(broker, port);
  MQTT2.setCallback(receivePacket);
}

void connectWiFi(){
  Serial.println("Connecting to");
  Serial.print(ssid);
  Serial.println("...");

  WiFi.begin(ssid, pass);

  Serial.print("\nConnecting to ");
  Serial.print(ssid);
  Serial.print(".");

  while(WiFi.status() != WL_CONNECTED){
  delay(500);
  Serial.print("...");
  }
  if (WiFi.status() == WL_CONNECTED){
    Serial.print("\nConnected to ");
    Serial.print(WiFi.localIP());
    return;
  }
 
}
void connectMQTT(){
  if (MQTT.connected() and MQTT2.connected()){
    return;
  }
  while (!MQTT.connected()){
    Serial.print("\nConnecting to MQTT Broker ");
    Serial.print(broker);
    Serial.println(".");
    if (MQTT.connect(pubMag)){
      Serial.print("\nConnected to Broker for pubMag.");
    }
    else if (MQTT2.connect(subLock)){
      Serial.print("\nConnected to Broker for subLock.");
      MQTT2.subscribe(TOPIC_PUBLISH);
      if (MQTT.connected() and MQTT2.connected()){
        break;
      }
    }
    else if(!MQTT.connected()) {
      Serial.print("\nCould not connect to broker. New attempt in 5 seconds.");
      delay(5000);
    }
  }
}
void keepConnections(){
  if(!MQTT.connected()){
      connectMQTT();
  }
  if(WiFi.status() != WL_CONNECTED){
    connectWiFi();
  }
}
void receivePacket(char* topic, byte* payload, unsigned int length){
  String msg;

  for(int i = 0; i < length; i++){
    char c = (char)payload[i];
    msg += c;
  }
  if (msg == "0"){
    digitalWrite(lockPin, HIGH);
    delay(500);
  }
  if (msg == "1"){
    digitalWrite(lockPin, LOW);
    delay(3000);
  }
}

void magSensor(){
  magnetic = digitalRead(magPin);
  
  if (magnetic == HIGH){
    digitalWrite(pulsePin, HIGH);
    MQTT.publish(TOPIC_PUBLISH, "1");
    Serial.print("\nPorta Aberta");
    delay(3000);

  } else if (magnetic == LOW) {
    digitalWrite(pulsePin, LOW);
    MQTT.publish(TOPIC_PUBLISH, "0");
    Serial.print("\nPorta Fechada");
    delay(3000);
  }
}

void loop() {
  magSensor();
  keepConnections();
  MQTT.loop();
  
}
