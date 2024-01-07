int linha[8] = {6, 3, 8, 2, 13, 9, A1, 10};
int coluna[8] = {A0, 4, 5, 11, 7, 12, A2, A3};

byte p[] = {B00000000, B00100000, B00100000, B00111000, B00100100, B00100100, B00011000, B00000000};
byte e[] = {B00000000, B00001110, B00010000, B00011110, B00011110, B00010000, B00001110, B00000000};
byte s[] = {B00000000, B00011000, B00100100, B00001000, B00010000, B00100100, B00011000, B00000000};

void printLetter(byte letter[], int shift) {
    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            digitalWrite(coluna[k], (~letter[i] >> (7 - k - shift)) & 0x01);
        }
        digitalWrite(linha[i], HIGH);
        delay(2);
        digitalWrite(linha[i], LOW);
    }
}

void setup() {
    for (int i = 2; i < 14; i++) {
        pinMode(i, OUTPUT);
    }
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);

    Serial.begin(9600);
}

void loop() {
    // Aparece e some completamente a letra "P"
    for (int shift = 0; shift < 8; shift++) {
        printLetter(p, shift);
        delay(50);
    }

    // Aparecem as letras "E" e somem antes da letra "S"
    for (int shift = 0; shift < 8; shift++) {
      printLetter(e, shift);
      delay(50);
    }

    // Aparece a letra "S" após todas as aparições da letra "E"
    for (int shift = 0; shift < 8; shift++) {
        printLetter(s, shift);
        delay(50);
    }
}
