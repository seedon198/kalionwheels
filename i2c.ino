#include <Wire.h>
#include <Servo.h>

#define ADDRESS 0x08

byte array1[4] = {127,127,127,127};
int panPos = 0;
int tiltPos = 0;

#define PANI 7
#define TILTI 6
#define ABCK  13
#define AFWD  9
#define BBCK  10
#define BFWD  11
#define LED 8

Servo pan;
Servo tilt;


void setup(void) {
  Wire.begin(ADDRESS);
  Wire.onReceive(msg_received);
  Serial.begin(112500);

  pinMode(LED, OUTPUT);
  pinMode(ABCK, OUTPUT);
  pinMode(AFWD, OUTPUT);
  pinMode(BBCK, OUTPUT);
  pinMode(BFWD, OUTPUT);
  pan.attach(PANI);
  tilt.attach(TILTI);

}

void loop() {
  Serial.println(array1[3]);
  pan.write(map(array1[1],0x00,0XFF,0,180));
  tilt.write(map(array1[0],0xFF,0x00,0,180));
  delay(1000);
  if(array1[3] == 0xFF) {
  digitalWrite(AFWD, HIGH);
  digitalWrite(ABCK, LOW);
  }
  else if(array1[3] == 0x00) {
  digitalWrite(AFWD, LOW);
  digitalWrite(ABCK, HIGH);
  }
  else {
    digitalWrite(AFWD, LOW);
    digitalWrite(ABCK, LOW);
  }
}

void msg_received(int numBytes)
{
  byte byteCount = 0;
  byteCount = Wire.available();
  Serial.print("------ Slave address > ");
  Serial.println(ADDRESS);
  Serial.print("Byte Count ");
  Serial.println(byteCount);
  digitalWrite(LED, HIGH);
  while(byteCount > 0)
  {
    byteCount -= 1;
    if (byteCount == 4) {
      byte x = Wire.read();
      Serial.println("Command Byte ");
    }
    else {
      array1[byteCount] = Wire.read();
      Serial.println(array1[byteCount]);
    }
  } 
}
