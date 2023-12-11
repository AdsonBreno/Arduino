#include <Arduino.h>
#include <HTTPClient.h>
#include <MFRC522.h>  //https://mundoprojetado.com.br/modulo-rfid-rc522/
//#include <PubSubClient.h>
#include <SPI.h>
#include <WiFi.h>

#define PINO_RST  15 //SDA no módulo RFID
#define PINO_SS   5  

#define pinRel    27
#define pinBuzz   32
#define pinGND    13

#define timeNegado 300
#define timeAcesso 750

const char* serverAddress = "http://your-server-url";

MFRC522 mfrc522(PINO_SS, PINO_RST);

void buzz(int liberado){
  delay(50);
  if(liberado == 1){
      delay(50);
      Serial.print("\nAccess Granted");
      digitalWrite(pinBuzz,HIGH);
      digitalWrite(pinRel,HIGH);
      delay(timeAcesso);
      digitalWrite(pinBuzz,LOW);
      delay(timeAcesso*2);
      digitalWrite(pinRel,LOW);  
  } else if(liberado == 0){
    Serial.print("\nAccess Denied");
    for(int b = 0; b < 6; b++){
      digitalWrite(pinBuzz, LOW);
      delay(timeNegado);
      digitalWrite(pinBuzz, HIGH);
      delay(timeNegado);
    }
    digitalWrite(pinBuzz, LOW);
  } 
}

void sendUIDToServer(String uid) {
  // Replace the following with your server details

  HTTPClient http;
  http.begin(serverAddress);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String payload = "uid=" + uid;
   int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Read the response from the server
    String response = http.getString();
    Serial.print("Server Response: ");
    Serial.println(response);

    // Parse the response (assuming '1' for success and '0' for failure)
    if (response == "1") {
      Serial.println("UID is in the database");
      buzz(1);
    } else {
      Serial.println("UID is NOT in the database");
      buzz(0);
    }
  } else {
    Serial.print("HTTP Request failed. Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void setup(){

  pinMode(pinRel,OUTPUT);
  pinMode(pinGND,OUTPUT);
  pinMode(pinBuzz,OUTPUT); // PINO BUZZER (+ NO 5V E GND NO PINO 13. LOGICA INVERTIDA.)

  digitalWrite(pinGND,LOW);
  
  // Inicia a comunicação serial (monitor serial)
  Serial.begin(9600);
  Serial.println(". . . Booting System");

  // Inicia a comunicação SPIE
  SPI.begin();

  // Inicia o módulo
  mfrc522.PCD_Init();

  Serial.println("Aguardando tag correta para abrir a porta...");
}

void loop(){
  String uid = "";
  // Verifica se existe um cartão presente para leitura
  if (mfrc522.PICC_IsNewCardPresent()){
    // Se sim, começa a ler o cartão
    if (mfrc522.PICC_ReadCardSerial()){     
      // Verifica os 4 bytes da UID
      Serial.print("Tag identificada: ");
      for (byte j = 0; j < 4; j=j+4){
        for (byte i = 0; i < 4; i++){
          // Printa o byte atual
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
          uid += String(mfrc522.uid.uidByte[i], HEX);

        }
      }
    
    Serial.println("The ID Card: ");
    Serial.println(uid);
    Serial.println(" will be send to ");
    Serial.println(serverAddress);
    sendUIDToServer(uid); //Envia UID para o Servidor

    delay(3000); // Delay para não ficar lendo rapidamente
    Serial.println("Aguardando tag certa para abrir a porta...");
    }
  }
}