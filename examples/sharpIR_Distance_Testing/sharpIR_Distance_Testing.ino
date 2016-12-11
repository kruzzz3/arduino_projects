#include <SharpIR.h>

#define ir A0
#define model 1080


SharpIR sharp(ir, 25, 10, model);

void setup() {
  Serial.begin(9600);
  pinMode (ir, INPUT);
}

void loop() {

  
  int dis=sharp.distance();  // this returns the distance to the object you're measuring


  Serial.print("Mean distance: ");  // returns it to the serial monitor
  Serial.println(dis);

  //delay(1000);
}
