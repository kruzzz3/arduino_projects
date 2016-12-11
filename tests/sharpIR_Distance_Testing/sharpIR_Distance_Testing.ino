#include <SharpIR.h>

#define ir A0
#define model 1080

SharpIR SharpIR(ir, model);

void setup() {
  Serial.begin(9600);
  pinMode (ir, INPUT);
}

void loop() {

  //delay(1000);   

  //unsigned long pepe1=millis();  // takes the time before the loop on the library begins

  int dis=SharpIR.distance();  // this returns the distance to the object you're measuring


  Serial.print("Mean distance: ");  // returns it to the serial monitor
  Serial.println(dis);
  
  //unsigned long pepe2=millis()-pepe1;  // the following gives you the time taken to get the measurement
  //Serial.print("Time taken (ms): ");
  //Serial.println(pepe2);  
}
