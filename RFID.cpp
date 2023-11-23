//https://mundoprojetado.com.br/modulo-rfid-rc522/

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Pino para colocar no modo "power-down"
// Não estamos utilizando no circuito proposto
#define PINO_RST  15
// Pino SS (no módulo está escrito SDA neste pino)
#define PINO_SS   5

#define timenegado 300
#define timeacesso 1000

#define EEPROM_SIZE 512 //Define o tamanho da EEPROM (1 - 512)

#define Door "DOOR" //ID para envio das informações sobre o estado da porta
#define RFID "RFID"
#define TOPIC_PUBLISH "HALLWAY"
#define TOPIC_SUBSCRIBE "RFID"

WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);
PubSubClient MQTT2(wifiClient);

//Consts
const char* ssid = "INTELBRAS";
const char* pass = "marcia12345";
const char* broker = "public.mqtthq.com"; //Teste

// UID da tag responsável por liberar o acesso
uint8_t uid_tag_desejada[4] = {0x0F, 0xD8, 0xC8, 0x43};

int cont = 0;
int liberado = 0;
int reactRFID = 5;
// Instância do módulo
MFRC522 mfrc522(PINO_SS, PINO_RST);

void receivePacket(char* topic, byte* payload, unsigned int length);

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
          EEPROM.begin(EEPROM_SIZE); //inicia a memória EEPROM
          EEPROM.write(cont, mfrc522.uid.uidByte[i]);
          cont++;

        }
    }
  }
  if(reactRFID == 1){
    for (int i = 0, i < 4, cont--){
      mfrc522.uid.uidByte[cont]
    
    }
}
void eeprom(){
  

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

  pinMode(27,OUTPUT);
  pinMode(32,OUTPUT); // PINO BUZZER (+ NO 5V E GND NO PINO 13. LOGICA INVERTIDA.)
  digitalWrite(13,LOW);
  
  Serial.begin(9600);
  Serial.println("...Iniciando Sistema");

  MQTT.setServer(broker, port);
  MQTT2.setServer(broker, port);
  MQTT2.setCallback(receivePacket);  

  // Inicia a comunicação SPI
  SPI.begin();

  // Inicia o módulo
  mfrc522.PCD_Init();

  Serial.print(EEPROM.read(0), HEX);
  Serial.print(EEPROM.read(1), HEX);
  Serial.print(EEPROM.read(2), HEX);
  Serial.print(EEPROM.read(3), HEX);
  Serial.println();
  Serial.println("Aguardando tag certa para abrir a porta...");
}

void connectWiFi(){
  Serial.println("Connecting to");
  Serial.print(ssid);
  Serial.println("...");

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
  delay(500);
  Serial.print("...");
  }
  if (WiFi.status() == WL_CONNECTED){
    Serial.print("\nConnected at ");
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
    if (MQTT.connect(pubMag)){
      Serial.print("\nConnected to Broker for Door.");
    }
    else if (MQTT2.connect(RFID)){
      Serial.print("\nConnected to Broker for RFID.");
      MQTT2.subscribe(TOPIC_SUBSCRIBE);
      if (MQTT.connected() and MQTT2.connected()){
        return;
      }
    }
    else if (!MQTT.connected() and !MQTT2.connected()){
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
    reactRFID = 0;
    delay(500);
  }
  if (msg == "1"){
    reactRFID = 1;
    Serial.print("Erasing Selected Card");
    delay(500);
  }
}

void loop()
{
  // Verifica se existe um cartão presente para leitura
  if (mfrc522.PICC_IsNewCardPresent()){
    // Se sim, começa a ler o cartão
    if (mfrc522.PICC_ReadCardSerial()){
      uint8_t liberado = 1;
     
      // Verifica os 4 bytes da UID
      Serial.print("Tag identificada: ");
      for (byte j = 0; j < 4; j=j+4){

                  for (byte i = 0; i < 4; i++)
                  {
                    // Printa o byte atual
                    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                    Serial.print(mfrc522.uid.uidByte[i], HEX);
      
                    // Se o byte for diferente da tag esperada, não libera o acesso
                    if(EEPROM.read(i) != mfrc522.uid.uidByte[i])
                    {
                      liberado = 0;
                    }
                  }
      Serial.println("");
      }
      if(liberado)
      {
        delay(50);
        digitalWrite(13,HIGH);
        Serial.println("Acesso liberado!");
        delay(timeacesso);
        digitalWrite(13,LOW);
        delay(50);
        // Executa outras ações, como abrir uma porta
      }
      else
      {
        delay(50);
        digitalWrite(13,LOW);
        delay(timenegado);
        digitalWrite(13,HIGH);
        delay(timenegado);
        digitalWrite(13,LOW);
        delay(timenegado);
        digitalWrite(13,HIGH);
        delay(timenegado);
        digitalWrite(13,LOW);
        delay(timenegado);
        digitalWrite(13,HIGH);
        delay(timenegado);
        digitalWrite(13,LOW);
        delay(timenegado);
        digitalWrite(13,HIGH);
        delay(timenegado);
        digitalWrite(13,LOW);
        delay(timenegado);
        digitalWrite(13,HIGH);
        delay(timenegado);
        digitalWrite(13,LOW);
        Serial.println("Acesso negado!");
        delay(50);
      }
    }
   
    // Delay para não ficar lendo rapidamente
    delay(1000);
    Serial.println("Aguardando tag certa para abrir a porta...");
  }
}
