#include <SPI.h>
#include <TFT_eSPI.h>

int pot;
float x, y, bin, b;

TFT_eSPI tft = TFT_eSPI();

void setup() {

  Serial.begin(115200);
  Serial.println("\n\n TFT_eSP Starting");

  tft.init();
  tft.setRotation(0); //0 = 0°; 1 = 90°; 2 = 180° clockwise

  x = 0.0;
  y = 12.0; //Starts at (0,12), avoiding top border
  bin = 1.0;
  b = 0.5;

}

void loop() { //Comments for debugging
  
  tft.fillScreen(0x000000);
  tft.fillCircle(x, y, 10, 0xFFFFFF);

  if (x == 228){
    bin = -1;
    //Serial.println("\nSubtraindo x");
  }else if (x == 12){
    bin = 1;
    //Serial.print("\n Somando x");
  }
  x += bin;

  if (y == 268){
    b = -0.5;
    //Serial.print("\nSubtraindo y");
  }else if (y == 12){
    b = 0.5;
    //Serial.println("\n Somando y");
  }
  y += b;

  tft.drawString(String(x), 30, 90, 5);
  tft.drawString(String(y), 70, 90, 5);
  /*Serial.print("x = ");
  Serial.println(x);
  Serial.print("\n");
  Serial.print("y = ");
  Serial.print(y);*/
  delay(100);
}
