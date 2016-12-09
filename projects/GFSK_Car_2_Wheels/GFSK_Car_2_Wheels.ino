#include <GFSK_DC.h>
#include <GFSK_Servo.h>
#include <GFSK_Ultrasonic.h>

GFSK_DC motorRight(6, 7, 3);
GFSK_DC motorLeft(12, 8, 5);
GFSK_Servo servo(10);
GFSK_Ultrasonic ultrasonic(12, 11, 50);

int currentState = 0;
int measuredDistancesLeft[50];
int measuredDistancesMiddle[20];
int measuredDistancesRight[50];

int speedLeft = 73;
int speedRight = 55;

void setup() {
  Serial.begin(9600);
  
  servo.init();
  servo.writeInstant(-60);
  
  ultrasonic.init();
  motorRight.init();
  motorLeft.init();

  motorRight.setSpeed(speedRight);
  motorLeft.setSpeed(speedLeft);
  motorRight.stop();
  motorLeft.stop();

  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(13, LOW);
  
  delay(1000);
}

void loop()
{ 
  motorRight.loop();
  motorLeft.loop();
  servo.loop();
  runServo();
  
  setNextState();

  Serial.println(currentState);

  if (currentState == 0) { // drive foreward     
    motorRight.setDirection(1);
    motorLeft.setDirection(1);
    signalMiddle();
    motorRight.setSpeed(speedRight);
    motorLeft.setSpeed(speedLeft);
  } else if (currentState == 1) // stop
  {
    motorRight.stop();
    motorLeft.stop();
    int lowestRight = lowest(0);
    int lowestLeft = lowest(2);

    if (lowestRight >= lowestLeft) {
      motorRight.setDirection(0);
      motorLeft.setDirection(1);
      motorRight.setSpeed(speedRight + 5);
      motorLeft.setSpeed(speedLeft - 5);
    } else {
      motorRight.setDirection(1);
      motorLeft.setDirection(0);
      motorRight.setSpeed(speedRight - 5);
      motorLeft.setSpeed(speedLeft + 5);
    }
  } else if (currentState == 2) // turn slightly right
  {
    signalRight();
    motorRight.setSpeed(speedRight - 5);
    motorLeft.setSpeed(speedLeft + 5);
  } else if (currentState == 3) // turn slightly left
  {
          signalLeft();
    motorRight.setSpeed(speedRight + 5);
    motorLeft.setSpeed(speedLeft - 5);
  }

}

int setNextState()
{
  int lowestRight = lowest(0);
  int lowestMiddle = lowest(1);
  int lowestLeft = lowest(2);

  if (lowestMiddle < 40)
  {
    currentState = 1; // stop and turn 
  } else if (lowestRight < 60 && lowestLeft >= 60)
  {
    currentState = 2; // turn slightly right
  } else if (lowestLeft < 60 && lowestRight >= 60)
  {
    currentState = 3; // turn slightly left
  } else
  {
    currentState = 0; // run foreward
  }

}

int lowest(int arr)
{
  int lowest = 300;
  if (arr == 0) // right
  {
    for (int i = 0; i < 50; i++)
    {
      int iDistance = measuredDistancesRight[i];
      if (lowest > iDistance) {
        //lowest = iDistance; 
      }
      lowest = lowest + iDistance;
    }
    lowest = lowest / 50;
  } else if (arr == 1)
  {
    for (int i = 0; i < 20; i++)
    {
      int iDistance = measuredDistancesMiddle[i];
      if (lowest > iDistance) {
        //lowest = iDistance; 
      }
      lowest = lowest + iDistance;
    }
    lowest = lowest / 20;
  } else if (arr == 2)
  {
    for (int i = 0; i < 50; i++)
    {
      int iDistance = measuredDistancesLeft[i];
      if (lowest > iDistance) {
        //lowest = iDistance; 
      }
      lowest = lowest + iDistance;
    }
    lowest = lowest / 50;
  }

  return lowest;
}

void runServo()
{
  if (servo.getPosition() == -60)
  {
    servo.write(60, 2);
  } else if (servo.getPosition() == 60)
  {
    servo.write(-60, 2);
  }
  
  int pos = servo.getPosition() + 60;
  if (pos < 50) // right
  {
    measuredDistancesRight[pos] = ultrasonic.getDistanceInCM();
  } else if (pos >= 50 && pos < 70)
  {
    measuredDistancesMiddle[pos - 50] = ultrasonic.getDistanceInCM();
  } else if (pos >= 70 && pos < 120) // left
  {
    measuredDistancesLeft[pos - 70] = ultrasonic.getDistanceInCM();
  }
}

void signalMiddle()
{
  digitalWrite(2, HIGH);
  digitalWrite(13, HIGH);
}

void signalLeft()
{
  digitalWrite(2, HIGH);
  digitalWrite(13, LOW);
}

void signalRight()
{
  digitalWrite(2, LOW);
  digitalWrite(13, HIGH);
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

