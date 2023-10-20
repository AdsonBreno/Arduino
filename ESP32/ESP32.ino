#include <Arduino.h>
#include <WiFi.h>

//Digital Inputs
#define dataPin 23
#define sPin 22 //Sensor
#define aPin 4 //Analogic
#define magPin 21 //Magnetic

//Digital Outputs
#define pulsePin 15
#define constPin 16

//Consts
const char* ssid = "Sensor_Hall";
const char* pass = "Hall1234";
const char* broker = "public.mqtthq.com";

int pState = HIGH;
int state, analogic, magnetic;
int port = 1883;

void setupWiFi(){
  delay(100);
  Serial.print("\nConnecting to");
  Serial.println(ssid);
  Serial.print("...");

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print("...");
  }

  Serial.print("\nConnected to");
  Serial.println(ssid);
  Serial.print(".");
}

void setup() {
  pinMode(pulsePin, OUTPUT);
  pinMode(constPin, OUTPUT);
  pinMode(magPin, INPUT_PULLUP);
  pinMode(sPin, INPUT_PULLUP);
  pinMode(dataPin, INPUT_PULLUP);
  digitalWrite(constPin, HIGH);
  setupWiFi();
}
void pulse500() {
  state = digitalRead(dataPin);

    if (pState == LOW && state == HIGH) {
    digitalWrite(pulsePin, HIGH);
    delay(500);
    if (state == HIGH)
      digitalWrite(pulsePin, LOW);
      delay(3000);
  }

  pState = state;
}
void magSensor(){
  magnetic = digitalRead(magPin);
  
  if (magnetic == HIGH){
    Serial.begin(9600);
    digitalWrite(pulsePin, HIGH);
    Serial.println("Porta Aberta");
    delay(3000);
  } else if (magnetic == LOW) {
    digitalWrite(pulsePin, LOW);
    Serial.println("Porta Fechada");
    delay(3000);
  }
}
void sensorPIR() {
  bool sPIR = digitalRead(sPin);

  if(sPIR) {
    Serial.begin(9600);
    digitalWrite(pulsePin, HIGH);
    Serial.println("ACIONADO");
    delay(1000);
  } else {
    digitalWrite(pulsePin, LOW);
    Serial.println("---");
  }
}
void loop() {

//  analogic = analogRead(aPin); //Retorna 0~4096 0,0080566406
//  digitalWrite(constPin, HIGH); //3,3V Constant
  magSensor();
}