#include <GFSK_Ultrasonic.h>

GFSK_Ultrasonic ultrasonicLeft(11, 12, 0);
GFSK_Ultrasonic ultrasonicMiddle(8, 9, 0);
GFSK_Ultrasonic ultrasonicRight(5, 6, 0);

int ledRight = 4;
int ledMiddle = 7;
int ledLeft = 10;

int distanceRight = 350;
int distanceMiddle = 350;
int distanceLeft = 350;

int criticalDistanceRight = 30;
int criticaldistanceMiddle = 20;
int criticaldistanceLeft = 30;

byte msg = B00000000;

void setup() {
  Serial.begin(115200);
  ultrasonicLeft.init();
  ultrasonicMiddle.init();
  ultrasonicRight.init();

  pinMode(ledRight, OUTPUT);
  pinMode(ledMiddle, OUTPUT);
  pinMode(ledLeft, OUTPUT);

  digitalWrite(ledRight, LOW);
  digitalWrite(ledMiddle, LOW);
  digitalWrite(ledLeft, LOW);

}

void loop()
{ 
  msg = B00000000;
  digitalWrite(ledRight, LOW);
  digitalWrite(ledMiddle, LOW);
  digitalWrite(ledLeft, LOW);
  distanceRight = ultrasonicRight.getDistanceInCM();
  distanceMiddle = ultrasonicMiddle.getDistanceInCM();
  distanceLeft = ultrasonicLeft.getDistanceInCM();

  if (distanceRight <= criticalDistanceRight) {
      digitalWrite(ledRight, HIGH);
      msg = msg | B00000001;
  }
  if (distanceMiddle <= criticaldistanceMiddle) {
      digitalWrite(ledMiddle, HIGH);
      msg = msg | B00000010;
  }
  if (distanceLeft <= criticaldistanceLeft) {
      digitalWrite(ledLeft, HIGH);
      msg = msg | B00000100;
  }
  
  //Serial.print("RIGHT:");
  //Serial.println(distanceRight);
  //Serial.print("LEFT:");
  //Serial.println(distanceMiddle);
  //Serial.print("MIDDLE:");
  //Serial.println(distanceLeft);

  Serial.write(msg);
}


