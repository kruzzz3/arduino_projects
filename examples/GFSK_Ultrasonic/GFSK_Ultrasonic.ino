#include <GFSK_Ultrasonic.h>

GFSK_Ultrasonic ultrasonic(2, 4);

void setup()
{
  Serial.begin(9600);
  ultrasonic.init();
}

void loop()
{
  Serial.println(ultrasonic.getDistanceInCM());
  //delay(100);
}

