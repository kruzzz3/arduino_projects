#include <GFSK_DC.h>

GFSK_DC dc1(8, 9, 10);
GFSK_DC dc2(4, 5, 6);

void setup() {
  Serial.begin(9600);
  dc1.init();
  dc2.init();
}

void loop() {
  dc1.setDirection(0);
  dc2.setDirection(0);
  
  dc1.setSpeed(0);
  dc2.setSpeed(0);
  delay(2000);
  dc1.setSpeed(20);
  dc2.setSpeed(20);
  delay(2000);
  dc1.setSpeed(40);
  dc2.setSpeed(40);
  delay(2000);
  dc1.setSpeed(60);
  dc2.setSpeed(60);
  delay(2000);
  dc1.setSpeed(80);
  dc2.setSpeed(80);
  delay(2000);
  dc1.setSpeed(100);
  dc2.setSpeed(100);
  delay(2000);
  dc1.setSpeed(120);
  dc2.setSpeed(120);
  delay(2000);
  dc1.setSpeed(140);
  dc2.setSpeed(140);
  delay(2000);
  dc1.setSpeed(160);
  dc2.setSpeed(160);
  delay(2000);
  dc1.setSpeed(180);
  dc2.setSpeed(180);
  delay(2000);
  dc1.setSpeed(200);
  dc2.setSpeed(200);
  delay(2000);
  dc1.setSpeed(220);
  dc2.setSpeed(220);
  delay(2000);
  dc1.setSpeed(240);
  dc2.setSpeed(240);
  delay(2000);
  dc1.setSpeed(255);
  dc2.setSpeed(255);
  delay(2000);
}

