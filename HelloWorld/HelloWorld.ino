#include <LiquidCrystal.h>

LiquidCrystal plate(2,3,4,6,7,8,9,10,11,12,13);

void setup() {
  // put your setup code here, to run once:
  plate.begin(16,2);
  plate.print("Hello World!");
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
