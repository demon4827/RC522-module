#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

byte correctUID[4] = {0x, 0x, 0x, 0x};

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  clearLED();
  
  Serial.println(" Поднесите NFC метку...");
  Serial.print("Версия считывателя:");
  Serial.println(mfrc522.PCD_ReadRegister(mfrc522.VersionReg), HEX);
}

void loop() {

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID метки:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (compareUID(mfrc522.uid.uidByte, correctUID)) {
    setColor(0, 255, 0); // green
    Serial.println("Метка правильная!");
  } else {
    setColor(255, 0, 0); // red
    Serial.println("Метка неправильная!");
  }
  
  delay(2000); 
  clearLED();
}

bool compareUID(byte *uid1, byte *uid2) {
  for (byte i = 0; i < 4; i++) {
    if (uid1[i] != uid2[i]) {
      return false;
    }
  }
  return true;
}

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void clearLED() {
  setColor(0, 0, 0);
}
