byte msg;
byte criticalDistanceLeft = B00000000;
byte criticalDistanceMiddle = B00000000;
byte criticalDistanceRight = B00000000;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
     msg = Serial.read();
     criticalDistanceLeft = msg & B00000100;
     criticalDistanceMiddle = msg & B00000010;
     criticalDistanceRight = msg & B00000001;
  }
  if(criticalDistanceLeft > 0)
  {
    Serial.println("LEFT critical");
  }
  if(criticalDistanceMiddle > 0)
  {
    Serial.println("MIDDLE critical");
  }
  if(criticalDistanceRight > 0)
  {
    Serial.println("RIGHT critical");
  }
  
}
