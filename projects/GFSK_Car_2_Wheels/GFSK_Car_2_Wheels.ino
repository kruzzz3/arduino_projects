#include <GFSK_DC.h>
GFSK_DC motorRight(6, 7, 3);
GFSK_DC motorLeft(12, 8, 5);

byte msg;
byte criticalDistanceLeft = B00000000;
byte criticalDistanceMiddle = B00000000;
byte criticalDistanceRight = B00000000;
int lastState = 0;
int currentState = 0;

int speedLeft = 73;
int speedRight = 55;
long _lastTimer;

void setup() {
  Serial.begin(115200);
  
  motorRight.init();
  motorLeft.init();

  motorRight.setSpeed(speedRight);
  motorLeft.setSpeed(speedLeft);
  motorRight.stop();
  motorLeft.stop();

  _lastTimer = 0;
  
  delay(1000);
}

void loop()
{   
  setNextState();

  Serial.println(currentState);

  if (currentState == 0) { // drive forward     
    motorRight.setDirection(1);
    motorLeft.setDirection(1);
    motorRight.setSpeed(speedRight);
    motorLeft.setSpeed(speedLeft);
  } 
  else if (currentState == 1) // stop
  { 
    motorRight.setSpeed(speedRight);
    motorLeft.setSpeed(speedLeft);
    if (_lastTimer == 0 && motorLeft.getDirection() == motorRight.getDirection())
    {
       _lastTimer = millis();
       motorRight.setSpeed(speedRight/2);
       motorLeft.setSpeed(speedLeft/2);
    }

    if (millis() > _lastTimer + 500)
    {
       motorRight.setDirection(0);
       motorLeft.setDirection(0);
    }

    if (millis() > _lastTimer + 600)
    {
      _lastTimer = 0;
      if(criticalDistanceRight > 0)
      {
        motorRight.setDirection(1);
        motorLeft.setDirection(0);
      }
      else if(criticalDistanceLeft > 0)
      {
        motorRight.setDirection(0);
        motorLeft.setDirection(1);
      }
      else 
      {
        motorRight.setSpeed(speedRight/2);
        motorLeft.setSpeed(speedLeft/2);
        motorRight.setDirection(1);
        motorLeft.setDirection(0);
      }
      motorRight.setSpeed(speedRight);
      motorLeft.setSpeed(speedLeft);
    }
  } else if (currentState == 2) // turn slightly right
  {
    motorRight.setSpeed(speedRight - 10);
    motorLeft.setSpeed(speedLeft + 10);
  } else if (currentState == 3) // turn slightly left
  {
    motorRight.setSpeed(speedRight + 10);
    motorLeft.setSpeed(speedLeft -10);
  }

}

int setNextState()
{ 
  currentState = 0;
      
  if(Serial.available() > 0)
  {
     msg = Serial.read();
     Serial.flush();
     criticalDistanceLeft = msg & B00000100;
     criticalDistanceMiddle = msg & B00000010;
     criticalDistanceRight = msg & B00000001;
  }
  
  if(criticalDistanceMiddle > 0)
  {
    Serial.println("MIDDLE critical");
    currentState = 1; // stop and move backwards, then turn 
  }
  else
  {   
    if(criticalDistanceRight > 0)
    {
      Serial.println("RIGHT critical");
      currentState = 3; // turn slightly left
    }
    if(criticalDistanceLeft > 0)
    {
      Serial.println("LEFT critical");
      currentState = 2; // turn slightly right
    }
  }
}

void turnRightAtCurrentPlace()
{
  motorRight.setDirection(0);
  motorLeft.setDirection(1);
}

void turnLeftAtCurrentPlace()
{
  motorRight.setDirection(1);
  motorLeft.setDirection(0);
}

