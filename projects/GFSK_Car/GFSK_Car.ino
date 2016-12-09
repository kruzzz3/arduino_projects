#include <GFSK_DC.h>
#include <GFSK_Servo.h>
#include <GFSK_Ultrasonic.h>
#include <GFSK_IR_Distance.h>

GFSK_Servo steering(5);
GFSK_DC motor(4, 2, 3);
GFSK_Ultrasonic ultrasonicRight(6, 7);
GFSK_Ultrasonic ultrasonicMiddle(8, 9);
GFSK_Ultrasonic ultrasonicLeft(10, 11);
GFSK_IR_Distance irDistance(A0);

long _lastTimeUltrasonic;
long _lastTimeState;
int _distance;
int _state;

void setup() {
  steering.init();
  motor.init();
  ultrasonicRight.init();
  ultrasonicMiddle.init();
  ultrasonicLeft.init();
  irDistance.init();

  pinMode(13, INPUT);
  
  Serial.begin(9600);
  steering.write(0,2);
  motor.setSpeed(255);

  _distance = 300;
  _state = 0;
  // 0 = drive forward
  // 1 = drive backward

  _lastTimeUltrasonic = millis();
  _lastTimeState = millis();
}

void loop() {
  //Serial.print("D = ");
  //Serial.println(irDistance.getDistanceInCM());
  //delay(100);
  
  if ((millis() - 100) > _lastTimeUltrasonic) {
    _lastTimeUltrasonic = millis();
    _distance = ultrasonicMiddle.getDistanceInCM();
    int dTempA = ultrasonicRight.getDistanceInCM();
    int dTempB = ultrasonicLeft.getDistanceInCM();
    if (dTempA < _distance) { _distance = dTempA; }
    if (dTempB < _distance) { _distance = dTempB; }
  }
  
  if (digitalRead(13) == HIGH) {
    steering.loop();
    motor.loop();

    if (_distance <= 10) {
      _state = 1;
      motor.stop();
    }
    
    if (_state == 0) {
      motor.setDirection(0);
      steering.write(0,2);
    }

    if (_state == 1) {
        steering.write(45,2);
        motor.setDirection(1);

        if (_distance > 30) {
          _state = 0;
        }
    }
  }
  else {
    motor.stop();
  }
}
