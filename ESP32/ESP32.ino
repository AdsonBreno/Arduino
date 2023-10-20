#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

//Digital Inputs
#define sPin 22 //Sensor
#define aPin 4 //Analogic
#define magPin 21 //Magnetic

//Digital Outputs
#define pulsePin 15
#define constPin 16

//ID MQTT
#define pubMag "MAGS"
#define subLock "DOOR"
#define TOPIC_PUBLISH "HALLWAY"

WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);
PubSubClient MQTT2(wifiClient);

//Consts
const char* ssid = "LAT-VISITAS";
const char* pass = "@lat@public";
const char* broker = "public.mqtthq.com";

int port = 1883;
int pState = HIGH;
int state, analogic, magnetic;

void setup() {
  Serial.begin(9600);
  pinMode(pulsePin, OUTPUT);
  pinMode(magPin, INPUT_PULLUP);

  digitalWrite(constPin, HIGH);
  connectWiFi();
  MQTT.setServer(broker, port);
  MQTT2.setServer(broker, port);
  MQTT2.setCallback(receivePacket);
}

void connectWiFi(){
  Serial.println("Connecting to");
  Serial.println(ssid);
  Serial.print("...");

  WiFi.begin(ssid, pass);

  if (WiFi.status() == WL_CONNECTED){
    Serial.print("...");
    return;
  }

  Serial.println("Connecting to ");
  Serial.print(ssid);
  Serial.print(".");

  while(WiFi.status() != WL_CONNECTED){
  delay(100);
  Serial.print("...");
  }
  Serial.println("Connected to ");
  Serial.print(ssid);
  Serial.print(".");
  Serial.println(WiFi.localIP());
}
void connectMQTT(){
  while (!MQTT.connected()){
    Serial.print("Connecting to MQTT Broker ");
    Serial.print(broker);
    Serial.print(".");
    if (MQTT.connect(pubMag)){
      Serial.println("Connected to Broker.");
    }
    if (MQTT2.connect(subLock)){
      Serial.println("Connected to Broker.");
      MQTT2.subscribe(TOPIC_PUBLISH);
    }
    else if(!MQTT.connected()){
      Serial.println("Could not connect to broker. New attempt in 10 seconds.");
      delay(10000);
    }
  }
}
void keepConnections(){
  if(!MQTT.connected()){
      connectMQTT();
  }
  connectWiFi();
}
void receivePacket(char* topic, byte* payload, unsigned int length){
  String msg;

  for(int i = 0; i < length; i++){
    char c = (char)payload[i];
    msg += c;
  }
  if (msg == "0"){
    digitalWrite(pulsePin, HIGH);
    delay(500);
  }
  if (msg == "1"){
    digitalWrite(pulsePin, LOW);
    delay(3000);
  }
}

void magSensor(){
  magnetic = digitalRead(magPin);
  
  if (magnetic == HIGH){
    digitalWrite(pulsePin, HIGH);
    MQTT.publish(TOPIC_PUBLISH, "1");
    Serial.println("Porta Aberta");
    delay(3000);

  } else if (magnetic == LOW) {
    digitalWrite(pulsePin, LOW);
    MQTT.publish(TOPIC_PUBLISH, "0");
    Serial.println("Porta Fechada");
    delay(3000);
  }
}

void loop() {
  keepConnections();
  MQTT.loop();
  magSensor();
}