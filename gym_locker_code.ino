#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define SERVO 7
#define GREEN_LED 5
#define RED_LED 6
#define SPEAKER 3


MFRC522 rfid(SS_PIN, RST_PIN);
Servo lockerServo;

enum LockerState {
  OPEN,
  LOCKED,
  WAIT_REMOVE
};

LockerState state = OPEN;
byte ownerUID[10];
byte ownerLen = 0;

bool sameUID(byte *a, byte aLen, byte *b, byte bLen) {
  if (aLen != bLen) return false;
  for (byte i = 0; i < aLen; i++) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

void copyUID() {
  ownerLen = rfid.uid.size;
  for (byte i = 0; i < ownerLen; i++) {
    ownerUID[i] = rfid.uid.uidByte[i];
  }
}

void successSound() {
  tone(SPEAKER, 1200);
  delay(120);
  tone(SPEAKER, 1600);
  delay(120);
  tone(SPEAKER, 2000);
  delay(150);
  noTone(SPEAKER);
}

void beab(){
  tone(SPEAKER, 1000);
  delay(500);

  noTone(SPEAKER);
  delay(500);
}
void deniedSound() {
  tone(SPEAKER, 400);
  delay(250);
  noTone(SPEAKER);
  delay(35);
  tone(SPEAKER, 250);
  delay(340);
  noTone(SPEAKER);
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  //leds
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  lockerServo.attach(SERVO);
  Serial.println("Gym locker ready (OPEN)");
  lockerServo.write(0);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);

  //speaker
  pinMode(SPEAKER, OUTPUT);
}

void loop() {

  if (state == WAIT_REMOVE) {
    if (!rfid.PICC_IsNewCardPresent()) {
      state = (ownerLen == 0) ? OPEN : LOCKED;
      Serial.println("Card removed");
    }
    return;
  }

  //No card
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  //Card detected
  if (state == OPEN) {
    Serial.println("Locker LOCKED");
    lockerServo.write(90);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    beab();
    copyUID();
    state = WAIT_REMOVE;
  }

  else if (state == LOCKED) {
    if (sameUID(rfid.uid.uidByte, rfid.uid.size, ownerUID, ownerLen)) {
      //Correct card
      Serial.println("Locker OPENED");
      lockerServo.write(0);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      successSound();


      ownerLen = 0;   //embty
      state = WAIT_REMOVE;
    } else {
      Serial.println("Access DENIED (Wrong card)");
      deniedSound();
      state = WAIT_REMOVE;
    }
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
