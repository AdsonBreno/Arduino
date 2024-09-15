#include <Arduino.h>
#include <EEPROM.h>
//#include <freertos/FreeRTOS.h>
//#include <freertos/task.h>
#include <MFRC522.h> //https://mundoprojetado.com.br/modulo-rfid-rc522/
#include <PubSubClient.h>
#include <SPI.h>
#include <WiFi.h>

#define PINO_RST  15 //SDA no módulo RFID
#define PINO_SS   5  
#define pinBuzz   32
#define pinGND    13

#define timeNegado 300
#define timeAcesso 750

#define EEPROM_SIZE 512 //Define o tamanho da EEPROM (1 - 512)
#define NUM_CARTOES 10 // Defina o número máximo de cartões

#define ID_PUBLISH    "EstadoPorta"
#define ID_SUBSCRIBE  "UnidadeCard"
#define ID_SUBSCRIBE2 "AcaoMemoria"

#define TOPIC_PUBLISH     "EstadoPorta"
#define TOPIC_SUBSCRIBE   "CARD"
#define TOPIC_SUBSCRIBE2  "EEPROM"

const char* ssid = "LIEC_119";
const char* pass = "********";
const char* broker = "public.mqtthq.com"; //Test
int port = 1883;

//TaskHandle_t ConnectWiFi;

//Global constants
String idCard = "";

bool cartaoEncontrado = false;

int liberado = 0;
int act = 2;
int card = 10; //Por padrão o endereço usado é o último, para evitar que dados sejam perdidos

uint8_t uid_tag_nova[4];

//Conexões 
WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);
//PubSubClient MQTT2(wifiClient);
PubSubClient MQTT3(wifiClient);
MFRC522 mfrc522(PINO_SS, PINO_RST);

// Matriz para armazenar os cartões
uint8_t cartoes[NUM_CARTOES][4];

//void receivePacket2(char* topic, byte* payload, unsigned int length);
void receivePacket3(char* topic, byte* payload, unsigned int length);

void setup() {
  EEPROM.begin(EEPROM_SIZE);

  pinMode(pinBuzz, OUTPUT);
  digitalWrite(pinGND, LOW);
  digitalWrite(pinBuzz, LOW);

  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Serial.println(". . .Booting System   ");

  SPI.begin();

  mfrc522.PCD_Init();
  
  connectWiFi();
  MQTT.setServer(broker, port);
  //MQTT2.setServer(broker, port);
  MQTT3.setServer(broker, port);
  
  //MQTT2.setCallback(receivePacket2); //Pacote com o endereço do cartão
  MQTT3.setCallback(receivePacket3); //Pactote com a ação para a memória

  keepConnections();

  Serial.println("\nAguardando tag correta para abrir a porta...");
}/*
void receivePacket2(char* topic, byte* payload, unsigned int length){
  String msg;

  for(int i = 0; i < length; i++){
    char c = (char)payload[i];
    msg += c;
  }
  card = msg.toInt(); //Índice de escrita ou para apagar da memória
}
*/
void receivePacket3(char* topic, byte* payload, unsigned int length){
  String msg;

  for(int i = 0; i < length; i++){
    char c = (char)payload[i];
    msg += c;
  }
  if (msg == "0"){ //Comando para ESCREVER a Tag lida na memória
    Serial.print("\nMessage Received '0'");
    eeprom(1, card, 0); //Escreve no endereço 'card' da matriz, pode SOBRESCREVER dados, cuidado!
  }
  if (msg == "1"){ //Comando para APAGAR a Tag selecionada da memória
    Serial.print("\nMessage Received '1'");
    eeprom(0, card, 0);
  }
  if(msg == "2"){ //Comando para LER toda a memória
    Serial.print("\nMessage Received '2");
    eeprom(2, 0, 1); //Act = Nada, Card = Indiferente, READ!!
  }
}
void eeprom(int act, int card, bool read) { //Recebe uma Ação e um índice de Cartão
  if(act == 1){ //Ação para escrever na memória
    for (int i = 0; i < 4; i++) {
    EEPROM.write(card * 4 + i, uid_tag_nova[i]);
    Serial.println("Writing the new tag on the system...");
    } 
    act = 2; //Não faz nada
  } else if(act == 0){ //Ação para apagar da memória
    for (int i = 0; i < 4; i++) {
    EEPROM.write(card * 4 + i, 0);
    Serial.println("Erasing card ");
    Serial.print(card+1); //0 é o endereço do cartão 1..
    Serial.print(" from the system...");
    }
    act = 2; //NOP
  }
  EEPROM.commit();
  Serial.println("Process complete");
  //Leitura de dados da EEPROM
  if(read){
    Serial.println("Conteúdo da EEPROM:");

    for (int c = 0; c < NUM_CARTOES; c++) {
      Serial.print("Cartão ");
      Serial.print(c+1);
      Serial.print(": ");

      for (int i = 0; i < 4; i++) {
        cartoes[c][i] = EEPROM.read(c * 4 + i);
        Serial.print(cartoes[c][i], HEX);
        Serial.print(" ");
      }
    }
    Serial.println("\nFim da leitura da EEPROM");
  }
}

void buzz(){
  delay(50);
  if(liberado == 0){
    MQTT.publish(TOPIC_PUBLISH, "0");
    Serial.print("\nAccess Denied");
    for(int b = 0; b < 6; b++){
      digitalWrite(pinBuzz, LOW);
      delay(timeNegado);
      digitalWrite(pinBuzz, HIGH);
      delay(timeNegado);
    }
    digitalWrite(pinBuzz, LOW);
  } else if(liberado == 1){
      delay(50);
      MQTT.publish(TOPIC_PUBLISH, "1");
      Serial.print("\nAccess Granted");
      digitalWrite(pinBuzz,HIGH);
      delay(timeAcesso);
      digitalWrite(pinBuzz,LOW);
      delay(50);  
    }
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
  if (MQTT.connected()){
    return;
  }
  while (!MQTT.connected()){
    Serial.print("\nConnecting to ");
    Serial.print(broker);
    if (MQTT.connect(ID_PUBLISH)){
      Serial.print("\nConnected to Broker.");
    }
    break;
  }
  if(!MQTT.connected()) {
    Serial.print("\nCould not publish. New attempt in 3 seconds.");
    delay(3000);
  }
}
/*
void connectMQTT2(){
  if (MQTT2.connected()){
    return;
  }
  while (!MQTT2.connected()){
    Serial.print("\nConnecting to ");
    Serial.print(broker);
    if (MQTT2.connect(ID_SUBSCRIBE)){
      Serial.print("\nConnected to Broker.");
      MQTT2.subscribe(TOPIC_SUBSCRIBE);

      if (MQTT2.connected()){
        break;
      }
    } else if(!MQTT2.connected()) {
      Serial.print("\nCould not subscribe. New attempt in 3 seconds.");
      delay(3000);
    }
  }
}*/

void connectMQTT3(){
  if (MQTT3.connected()){
    return;
  }
  while (!MQTT3.connected()){
    Serial.print("\nConnecting to ");
    Serial.print(broker);
    if (MQTT3.connect(ID_SUBSCRIBE2)){
      Serial.print("\nConnected to Broker.");
      MQTT3.subscribe(TOPIC_SUBSCRIBE2);

      if (MQTT3.connected()){
        break;
      }
    }
    else if(!MQTT3.connected()){
      Serial.print("\nCould not subscribe. New attempt in 3 seconds.");
      delay(3000);
    }
  }
}

void keepConnections(){
  if(!MQTT.connected()){
      connectMQTT();
  }/*
  if(!MQTT2.connected()){
    connectMQTT2();
  }*/
  if(!MQTT3.connected()){
    connectMQTT3();
  }
  if(WiFi.status() != WL_CONNECTED){
    connectWiFi();
  }
}

void loop() {
  // Verifica se existe um cartão presente para leitura, e faz a leitura do cartão
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.println("\nTag identificada: "); 
      idCard = ""; // Limpa a string antes de cada leitura
      memset(uid_tag_nova, 0, sizeof(uid_tag_nova));// Limpa a uid_tag_nova antes de armazenar novos valores
      for (int i = 0; i < 4; i++) {
        idCard += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : " ");
        idCard += String(mfrc522.uid.uidByte[i], HEX);
        uid_tag_nova[i] = mfrc522.uid.uidByte[i];
      }
      keepConnections();
        }
      for (int c = 0; c < NUM_CARTOES; c++) { //Endereço do cartão na matrix de cartões
        for (int i = 0; i < 4; i++) {
          if (EEPROM.read(c * 4 + i) != uid_tag_nova[i]) { //Compara a tag lida com os valores da memória
            liberado = 0;
          } else {
            liberado = 1;  

          }
        }
      }
      Serial.print("UID Nova: ");
      for (int i = 0; i < 4; i++) {
        Serial.print(uid_tag_nova[i], HEX);
        Serial.print(" ");
      }

      // Verifica se a UID está na lista de cartões
      if (!cartaoEncontrado) {
        for (int c = 0; c < NUM_CARTOES; c++) {
          bool cartaoCorrespondente = true;

          for (int i = 0; i < 4; i++) {
            if (uid_tag_nova[i] != cartoes[c][i]) {
              cartaoCorrespondente = false;
              break;
            }
          }

          if (cartaoCorrespondente) {
            liberado = 1;
            cartaoEncontrado = true; //O cartão já foi encontrado na memória
            break;
          } else {
            liberado = 0;
          }
        }
      }

      buzz();
    }

    // Delay para não ficar lendo rapidamente
    delay(1000);
    Serial.println("\nAguardando tag correta para abrir a porta..."); 
    MQTT.loop();
    //MQTT2.loop();
    MQTT3.loop();
}
