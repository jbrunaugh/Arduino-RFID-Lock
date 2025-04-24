#include <Servo.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
String UID = "66 A1 C1 01";
byte lock = 0;

Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);


void setup() {
  Serial.begin(9600);
  servo.write(70);
  servo.attach(4);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {


  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print("NUID tag is :");
  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(100);
  }
  ID.toUpperCase();

  if (ID.substring(1) == UID && lock == 0 ) {
    servo.write(100);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    //delay(1500);
    lock = 1;
    delay(3000);
    lock = 0;
    servo.write(0);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
  }
}