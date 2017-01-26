#include <CurieIMU.h>
#include <MadgwickAHRS.h>
#include <GFSK_Stepper.h>

Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;

GFSK_Stepper motorRight(6, 7, 8, 9);
GFSK_Stepper motorLeft(10, 11, 12, 13);

int motorDirection = 1;

float targetAngle = 0;

// get orientation -->
int aix, aiy, aiz;
int gix, giy, giz;
float ax, ay, az;
float gx, gy, gz;
float roll, pitch, heading;
unsigned long microsNow;

void setup() {
  Serial.begin(115200);
  // start the IMU and filter
  CurieIMU.begin();
  CurieIMU.setGyroRate(25);
  CurieIMU.setAccelerometerRate(25);
  filter.begin(25);

  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
  // Set the gyroscope range to 250 degrees/second
  CurieIMU.setGyroRange(250);
  
  // initialize variables to pace updates to correct rate
  microsPerReading = 1000000 / 25;
  //microsPerReading = 1000;
  microsPrevious = micros();

  // Motors -->
  motorRight.init();
  motorRight.setHalfStepping(0);
  motorRight.setStepsPerRevolution(200);  
  motorRight.setSpeed(2);
  
  motorLeft.init();
  motorLeft.setHalfStepping(0);
  motorLeft.setStepsPerRevolution(200);  
  motorLeft.setSpeed(1);
}

void loop() {
  // check if it's time to read data and update the filter
  //microsNow = micros();
  if ((micros() - microsPerReading) >= microsPrevious) {
  //if (microsNow - microsPrevious >= microsPerReading) {
    microsPrevious = micros();
    
    // read raw data from CurieIMU
    CurieIMU.readMotionSensor(aix, aiy, aiz, gix, giy, giz);
  
    // convert from raw data to gravity and degrees/second units
    ax = convertRawAcceleration(aix);
    ay = convertRawAcceleration(aiy);
    az = convertRawAcceleration(aiz);
    gx = convertRawGyro(gix);
    gy = convertRawGyro(giy);
    gz = convertRawGyro(giz);
  
    // update the filter, which computes orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);  
    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch() - 9.9;
    heading = filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);
  }

  //pitch = pitch - targetAngle;
  
  if(pitch < targetAngle && motorDirection == -1)
  {
    motorRight.stop();
    motorLeft.stop();
    motorDirection = 1;
  } else if(pitch > targetAngle && motorDirection == 1)
  {
    motorRight.stop();
    motorLeft.stop();
    motorDirection = -1;
  }
  
  float abs_pitch = abs((pitch - targetAngle)*100); 
  if (abs_pitch < 7000)
  {
    //if(abs_pitch > 1000) abs_pitch = 1000;
    //int motor_speed = map(abs_pitch, 0, 1000, 80, 2);
    if(abs_pitch > 1600) abs_pitch = 1600;
    int motor_speed = map(abs_pitch, 0, 1600, 100, 2);
  /*
    int motor_speed;
    if(abs_pitch > 1000) abs_pitch = 1000;
    if(abs_pitch <= 100)
    {
      motor_speed = 40;
      //motor_speed = map(abs_pitch, 0, 100, 25, 15);
    } else if(abs_pitch <= 200) {
      motor_speed = 25;
     // motor_speed = map(abs_pitch, 300, 800, 10, 5);
    } else if(abs_pitch <= 400) {
      motor_speed = 9;
      //motor_speed = map(abs_pitch, 300, 800, 10, 5);
    } else if(abs_pitch <= 600) {
      motor_speed = 3;
      //motor_speed = map(abs_pitch, 300, 800, 10, 5);
    }else if(abs_pitch < 1000) {
      motor_speed = 2;
      //motor_speed = map(abs_pitch, 300, 800, 10, 5);
    } else {
      motor_speed = 1;
    }
    // int motor_speed = map(abs_pitch, 0, 500, 20, 2);
    */
    //Serial.print("motor_speed: ");
    //Serial.println(motor_speed);
    
    // set Motors --> 
    motorRight.setSpeed(motor_speed); 
    motorLeft.setSpeed(motor_speed);
    //motorRight.setSpeed(1); 
    //motorLeft.setSpeed(1);
    int motor_degrees = 360 * motorDirection;
    motorRight.stepAsync(motor_degrees);
    motorLeft.stepAsync(motor_degrees * -1);
  } else {
    motorRight.stop();
    motorLeft.stop();
  }
}

float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}
