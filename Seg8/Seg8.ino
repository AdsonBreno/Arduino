//Controle de LED 8 segmentos com 10 pinos

int n;
#define seg0 2
#define seg1 3
#define seg2 4
#define seg3 5
#define seg4 6
#define seg5 7
#define seg6 8
#define seg7 9

void setup() {
    //Tensão x manda sinal apenas para x, dado resistor x?
    Serial.begin(9600);
    Serial.println("Iniciando");

    pinMode(seg0, OUTPUT);
    pinMode(seg1, OUTPUT);
    pinMode(seg2, OUTPUT);
    pinMode(seg3, OUTPUT);
    pinMode(seg4, OUTPUT);
    pinMode(seg5, OUTPUT);
    pinMode(seg6, OUTPUT);
    pinMode(seg7, OUTPUT);

    digitalWrite(seg0, LOW);
    digitalWrite(seg1, LOW);
    digitalWrite(seg2, LOW);
    digitalWrite(seg3, LOW);
    digitalWrite(seg4, LOW);
    digitalWrite(seg5, LOW);
    digitalWrite(seg6, LOW);
    digitalWrite(seg7, LOW);
}
void number(int n){
  if (n == 0){
    digitalWrite(seg3, LOW);
    digitalWrite(seg0, HIGH); //   ----
    digitalWrite(seg1, HIGH); //  |    |
    digitalWrite(seg2, HIGH); //  |    | 
    digitalWrite(seg4, HIGH); //  |    |
    digitalWrite(seg5, HIGH); //   ----
    digitalWrite(seg6, HIGH);
    Serial.println("Zero");
  } else if (n == 1){
    digitalWrite(seg1, LOW);
    digitalWrite(seg2, LOW);
    digitalWrite(seg3, LOW);
    digitalWrite(seg4, LOW);
    digitalWrite(seg5, LOW);
    digitalWrite(seg0, HIGH); //      |
    digitalWrite(seg6, HIGH); //      |
    Serial.println("Um");
  } else if (n == 2){
    digitalWrite(seg2, LOW);
    digitalWrite(seg4, LOW);
    digitalWrite(seg6, LOW);
    digitalWrite(seg0, HIGH); //   ----
    digitalWrite(seg1, HIGH); //       |
    digitalWrite(seg3, HIGH); //   ----
    digitalWrite(seg4, HIGH); //  |
    digitalWrite(seg5, HIGH); //   ----
    Serial.println("Dois");
  } else if (n == 3){
    digitalWrite(seg2, LOW);
    digitalWrite(seg4, LOW);
    digitalWrite(seg0, HIGH); //  ----
    digitalWrite(seg1, HIGH); //      |
    digitalWrite(seg3, HIGH); //  ----
    digitalWrite(seg5, HIGH); //      |
    digitalWrite(seg6, HIGH); //  ----
    Serial.println("Três");
  } else if (n == 4){
    digitalWrite(seg1, LOW);
    digitalWrite(seg4, LOW);
    digitalWrite(seg5, LOW);
    digitalWrite(seg7, LOW);
    digitalWrite(seg0, HIGH); //  |   |
    digitalWrite(seg2, HIGH); //   ---   
    digitalWrite(seg3, HIGH); //      |
    digitalWrite(seg6, HIGH);
    Serial.println("Quatro");
  } else if (n == 5){
    digitalWrite(seg0, LOW);
    digitalWrite(seg4, LOW);
    digitalWrite(seg1, HIGH); //   ----
    digitalWrite(seg2, HIGH); //  |
    digitalWrite(seg3, HIGH); //   ----
    digitalWrite(seg5, HIGH); //       | 
    digitalWrite(seg6, HIGH); //   ----
    Serial.println("Cinco");
  } else if (n == 6){
    digitalWrite(seg0, LOW);
    digitalWrite(seg1, HIGH); //   ----
    digitalWrite(seg2, HIGH); //  |
    digitalWrite(seg3, HIGH); //   ----
    digitalWrite(seg4, HIGH); //  |    | 
    digitalWrite(seg5, HIGH); //   ----
    digitalWrite(seg6, HIGH); 
    Serial.println("Seis");
  } else if (n == 7){
    digitalWrite(seg2, LOW);
    digitalWrite(seg3, LOW);
    digitalWrite(seg4, LOW);
    digitalWrite(seg5, LOW);
    digitalWrite(seg0, HIGH); //   ---
    digitalWrite(seg1, HIGH); //      |
    digitalWrite(seg6, HIGH); //      |
    Serial.println("Sete");
   } else if (n == 8){
    digitalWrite(seg0, HIGH); //   ----
    digitalWrite(seg1, HIGH); //  |    |
    digitalWrite(seg2, HIGH); //  |----| 
    digitalWrite(seg3, HIGH); //  |    |
    digitalWrite(seg4, HIGH); //   ----
    digitalWrite(seg5, HIGH);
    digitalWrite(seg6, HIGH);
    Serial.println("Oito");
   } else if (n == 9){
    digitalWrite(seg4, LOW);
    digitalWrite(seg0, HIGH); //   ----
    digitalWrite(seg1, HIGH); //  |    |
    digitalWrite(seg2, HIGH); //  |----| 
    digitalWrite(seg3, HIGH); //       |
    digitalWrite(seg5, HIGH); //   ----
    digitalWrite(seg6, HIGH);
    Serial.println("Nove");
   } else if (n == 10){
    digitalWrite(seg0, HIGH); //   ----
    digitalWrite(seg1, HIGH); //  |    |
    digitalWrite(seg2, HIGH); //  |----| 
    digitalWrite(seg3, HIGH); //  |    |
    digitalWrite(seg4, HIGH);
    digitalWrite(seg5, LOW);
    digitalWrite(seg6, HIGH);
    Serial.println("Dez");
   } else if (n == 11){
    digitalWrite(seg0, LOW);
    digitalWrite(seg1, LOW);  //  |    
    digitalWrite(seg2, HIGH); //  |---- 
    digitalWrite(seg3, HIGH); //  |    |
    digitalWrite(seg4, HIGH); //   ----
    digitalWrite(seg5, HIGH);
    digitalWrite(seg6, HIGH);
    Serial.println("Onze");
   } else if (n == 12){
    digitalWrite(seg0, LOW);  //   ----
    digitalWrite(seg1, HIGH); //  |
    digitalWrite(seg2, HIGH); //  | 
    digitalWrite(seg3, LOW);  //  |
    digitalWrite(seg5, HIGH); //   ----
    digitalWrite(seg6, LOW);
    Serial.println("Doze");
   } else if (n == 13){
    digitalWrite(seg0, HIGH); //      
    digitalWrite(seg1, LOW);  //       |
    digitalWrite(seg2, LOW);  //   ----| 
    digitalWrite(seg3, HIGH); //  |    |
    digitalWrite(seg4, HIGH); //   ----
    digitalWrite(seg5, HIGH);
    digitalWrite(seg6, HIGH);
    Serial.println("Treze");
   } else if (n == 14){
    digitalWrite(seg0, LOW);  //   ----
    digitalWrite(seg1, HIGH); //  |
    digitalWrite(seg2, HIGH); //  |---- 
    digitalWrite(seg3, HIGH); //  |
    digitalWrite(seg5, HIGH); //   ----
    digitalWrite(seg6, LOW);
    Serial.println("Quatorze");
   } else if (n == 15){
    digitalWrite(seg0, LOW);  //   ----
    digitalWrite(seg1, HIGH); //  |    
    digitalWrite(seg2, HIGH); //  |---- 
    digitalWrite(seg3, HIGH); //  |
    digitalWrite(seg4, HIGH);
    digitalWrite(seg5, LOW);
    digitalWrite(seg6, LOW);
    Serial.println("Quize");
   } else {
    digitalWrite(seg0, HIGH);
    digitalWrite(seg1, HIGH);
    digitalWrite(seg2, HIGH);
    digitalWrite(seg3, HIGH);
    digitalWrite(seg4, HIGH);
    digitalWrite(seg5, HIGH);
    digitalWrite(seg6, HIGH);
    digitalWrite(seg7, HIGH);
    delay(2000);

    digitalWrite(seg0, LOW);
    digitalWrite(seg1, LOW);
    digitalWrite(seg2, LOW);
    digitalWrite(seg3, LOW);
    digitalWrite(seg4, LOW);
    digitalWrite(seg5, LOW);
    digitalWrite(seg6, LOW);
    digitalWrite(seg7, LOW);
    delay(2000);
   }
}
void loop() {
  for (int i = 0; i < 16; i++){
    n = i;
    number(n);
    delay(1000);
  }
}
