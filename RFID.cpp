//https://mundoprojetado.com.br/modulo-rfid-rc522/
#include <Arduino.h>
#include <EEPROM.h>
#include <MFRC522.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <WiFi.h>

// Pino para colocar no modo "power-down"
// Não estamos utilizando no circuito proposto
#define PINO_RST 15
// Pino SS (no módulo está escrito SDA neste pino)
#define PINO_SS 5
#define pinBuzz 32
#define pinGND 13
#define timenegado 300
#define timeacesso 1000
#define EEPROM_SIZE 512 //Define o tamanho da EEPROM (1 - 512)

#define ID_PUBLISH "DOOR"
#define ID_SUBSCRIBE "RFID"

#define TOPIC_PUBLISH "DOOR"
#define TOPIC_SUBSCRIBE "RFID"

const char* ssid = "LIEC_119";
const char* pass = "0987ABCDEF";
const char* broker = "public.mqtthq.com"; //Test
int port = 1883;

//Global constants
int cont = 0;
int liberado = 0;
int reactRFID = 0;//5;

WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);
PubSubClient MQTT2(wifiClient);

void receivePacket(char* topic, byte* payload, unsigned int length);

// UID da tag responsável por liberar o acesso
uint8_t uid_tag_desejada[3] = {0x0F, 0xD8, 0xC8, 0x43};
uint8_t uid_tag_nova[3] = mfrc522.uid.uidByte;

// Instância do módulo
MFRC522 mfrc522(PINO_SS, PINO_RST);

//Operações RFID
void rfid(){
  if(reactRFID == 0){
    delay(5000);
    if (mfrc522.PICC_IsNewCardPresent()){
      Serial.println("     Tag identified     ");
      Serial.println ("Writing the new tag on the system...");
      
        for (int i = 0; i < 4; i++){
          // Printa o byte atual
          Serial.print([i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
          EEPROM.write(cont, mfrc522.uid.uidByte[i]);
          EEPROM.commit();
          cont++;

        }
    }
  }
  if(reactRFID == 1){
    for (int i = 0, i < 4, cont--){
      EEPROM.write(cont, 00);
    }
  }
}
void setup(){

  // Gravando Cartão na memoria EEPROM
  EEPROM.begin(EEPROM_SIZE); //inicia a memória EEPROM
  EEPROM.write(0, 0x0F);
  EEPROM.commit();
  EEPROM.write(1, 0xD8);
  EEPROM.commit();
  cont ++;
  EEPROM.write(2, 0xC8);
  EEPROM.commit();
  cont ++;
  EEPROM.write(3, 0x43);
  EEPROM.commit();
  cont ++;

  pinMode(pinBuzz,OUTPUT); // PINO BUZZER (+ NO 5V E GND NO PINO 13. LOGICA INVERTIDA.)
  digitalWrite(pinGND,LOW);
  
  // Inicia a comunicação serial (monitor serial)
  Serial.begin(9600);
  Serial.println(". . .Booting System   ");

  // Inicia a comunicação SPI
  SPI.begin();

  // Inicia o módulo
  mfrc522.PCD_Init();
  for(int i = 0, i < 4, i++){
    Serial.print(EEPROM.read(i), HEX);
  }
  Serial.println();

  Serial.println("Aguardando tag correta para abrir a porta...");

  MQTT.setServer(broker, port);
  MQTT2.setServer(broker, port);
  MQTT2.setCallback(receivePacket);
}
void receivePacket(char* topic, byte* payload, unsigned int length){
  String msg;

  for(int i = 0; i < length; i++){
    char c = (char)payload[i];
    msg += c;
  }
  if (msg == "0"){
    Serial.print("\nMessage Received '0'");
    for (e = 0, e < 4, e++){
     EEPROM.write(e, uid_tag_nova[e])
     EEPROM.commit();
    }
  }
}
void buzz(){
  delay(50);
  for(int b = 0, b < 6, b++){
    digitalWrite(pinBuzz, LOW);
    delay(timeNegado);
    digitalWrite(pinBuzz, HIGH);
    delay(timeNegado);
  }
  Serial.println("Acesso negado!");
  delay(50);
}

void connectMQTT2(){
  if (MQTT2.connected()){
    return;
  }
  while (!MQTT2.connected()){
    Serial.print("\nConnecting to ");
    Serial.print(broker);
    if (MQTT2.connect(subLock)){
      Serial.print("\nConnected to Broker for subLock.");
      MQTT2.subscribe(TOPIC_SUBSCRIBE);
      if (MQTT.connected()){
        magnetic = digitalRead(magPin);
          if (liberado == 3){
            MQTT.publish(TOPIC_PUBLISH, "1");
            Serial.print("\nAccess Granted");

          } else if (liberado == 0) {
              MQTT.publish(TOPIC_PUBLISH, "0");
              Serial.print("\nAccess Denied");
              delay(3000);
          } 
      }
      if (MQTT2.connected()){
        break;
      }
    }
    else if(!MQTT2.connected()) {
      Serial.print("\nCould not connect to sublock. New attempt in 3 seconds.");
      delay(3000);
    }
  }
}

void keepConnections(){
  if(!MQTT.connected()){
      connectMQTT();
  }
  if(!MQTT2.connected()){
    connectMQTT2();
  }
  if(WiFi.status() != WL_CONNECTED){
    connectWiFi();
  }
}

void loop(){
  // Verifica se existe um cartão presente para leitura
  if (mfrc522.PICC_IsNewCardPresent()){

  int liberado = 0; 

   if (mfrc522.PICC_ReadCardSerial()){ // Verifica os 4 bytes da UID
      Serial.print("Tag identificada: ");
      for (byte j = 0; j < 3; j=j+3){
        for (byte i = 0; i < 3; i++){
          // Printa o byte atual
          Serial.print((mfrc522.uid.uidByte[i] < 0x10) ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
          //Inverter + somador
          if(EEPROM.read(i) != mfrc522.uid.uidByte[i]){
            liberado = 0;
          }
        }
      }
    for(i = 0, i < 4, i++){
      if(uid_tag_desejada[i] == mfrc522.uid.uidByte[i]){
        liberado++; 
      }     
    }
      if(liberado == 3) {
        delay(50);
        digitalWrite(pinBuzz,HIGH);
        Serial.println("Acesso liberado!");
        delay(timeacesso);
        digitalWrite(pinBuzz,LOW);
        delay(50);
        // Executa outras ações, como abrir uma porta
        // Send packet to broker, so it'll open the door
      }
      else {
        buzz();
      }
    }
    // Delay para não ficar lendo rapidamente
    delay(2000);
    Serial.println("Aguardando tag correta para abrir a porta...");
  }
}
