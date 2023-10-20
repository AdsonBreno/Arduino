#define Red 13
#define Green 12
#define BLUE 11

int red = 255;
int green = 255; 
int blue = 255;
int red2 = 55;
int green2 = 55; 
int blue2 = 55;
void setup() {
  // put your setup code here, to run once:
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(BLUE, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(BLUE, blue2);
  delay(500);
  analogWrite(BLUE, 0);
  delay(500);
  analogWrite(Red, red2);
  delay(500);
  analogWrite(Red, 0);
  delay(500);
  analogWrite(Green, green2);
  delay(500);
  analogWrite(Green, 0);
  delay(500);
  analogWrite(Red, red);
  delay(500);
  analogWrite(Green, green);
  delay(500);
  analogWrite(BLUE, blue);
}
