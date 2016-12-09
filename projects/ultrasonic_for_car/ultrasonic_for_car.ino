#include <GFSK_Ultrasonic.h>

GFSK_Ultrasonic ultrasonicLeft(11, 12, 10);
GFSK_Ultrasonic ultrasonicMiddle(8, 9, 10);
GFSK_Ultrasonic ultrasonicRight(5, 6, 10);

int ledRight = 4;
int ledMiddle = 7;
int ledLeft = 10;

int distanceRight = 350;
int distanceMiddle = 350;
int distanceLeft = 350;

int criticalDistanceRight = 30;
int criticaldistanceMiddle = 25;
int criticaldistanceLeft = 30;

void setup() {
  Serial.begin(9600);
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
  digitalWrite(ledRight, LOW);
  digitalWrite(ledMiddle, LOW);
  digitalWrite(ledLeft, LOW);
  distanceRight = ultrasonicRight.getDistanceInCM();
  distanceMiddle = ultrasonicMiddle.getDistanceInCM();
  distanceLeft = ultrasonicLeft.getDistanceInCM();

  if (distanceRight <= criticalDistanceRight) {
      digitalWrite(ledRight, HIGH);
  }
  if (distanceMiddle <= criticaldistanceMiddle) {
      digitalWrite(ledMiddle, HIGH);
  }
  if (distanceLeft <= criticaldistanceLeft) {
      digitalWrite(ledLeft, HIGH);
  }
  
  //Serial.print("RIGHT:");
  //Serial.println(distanceRight);
  //Serial.print("LEFT:");
  //Serial.println(distanceMiddle);
  //Serial.print("MIDDLE:");
  //Serial.println(distanceLeft);
  
  
  //delay(500);
}


