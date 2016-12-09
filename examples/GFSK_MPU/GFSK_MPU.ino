// I2Cdev and MPU6050 must be installed as libraries
#include "I2Cdev.h"
//#include "MPU6050.h" // not necessary if using MPU6050_6Axis_MotionApps20 include file
#include "Wire.h"
#include "MPU6050_6Axis_MotionApps20.h"

/*
* MPU -->
*/


///////////////////////////////////   CONFIGURATION   /////////////////////////////
//Change this 3 variables if you want to fine tune the sketch to your needs.
const int buffersize = 300;     //Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
const int acel_deadzone = 8;     //Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
const int giro_deadzone = 1;     //Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)

// default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
//MPU6050 accelgyro;
MPU6050 mpu(0x68); // <-- use for AD0 high

// MPU control/status vars
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

int16_t ax, ay, az, gx, gy, gz;
int mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz;
int offset_ax, offset_ay, offset_az, offset_gx, offset_gy, offset_gz;

boolean is_offset_rotation_set;
float offset_rotation;

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// VCC = 5V
// GND = GND
// SCL = A5
// SDA = A4
// AD0 = GND

/*
* <-- MPU
*/

/*
* LED RGB --> 
*/

const int redPin    = 9;
const int greenPin  = 10;
const int bluePin   = 11;

 
/*
* <-- LED RGB 
*/


void setup()
{  
/*
* DEFAULT -->
*/
  Wire.begin();
  Wire.setClock(400000);
    
  Serial.begin(115200); // initialize serial communication
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  color(255, 0, 0); // set color to red

  is_offset_rotation_set = false;
/*
* <-- DEFAULT
*/

/*
* MPU -->
*/
  // COMMENT NEXT LINE IF YOU ARE USING ARDUINO DUE
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz). Leonardo measured 250kHz.

  mpu.initialize(); // initialize device

  calibrate();

  mpu.dmpInitialize();
 
  mpu.setXAccelOffset(offset_ax);
  mpu.setYAccelOffset(offset_ay);
  mpu.setZAccelOffset(offset_az);
  mpu.setXGyroOffset(offset_gx);
  mpu.setYGyroOffset(offset_gy);
  mpu.setZGyroOffset(offset_gz);

  mpu.setDMPEnabled(true);
  packetSize = mpu.dmpGetFIFOPacketSize();
  
/*
* <-- MPU
*/
}

void loop() {
  mpu.resetFIFO();
  fifoCount = mpu.getFIFOCount();

  // wait for correct available data length, should be a VERY short wait
  while (fifoCount < packetSize)
  {
    fifoCount = mpu.getFIFOCount();
  }
  
  // read a packet from FIFO
  mpu.getFIFOBytes(fifoBuffer, packetSize);
      
  // track FIFO count here in case there is > 1 packet available
  // (this lets us immediately read more without waiting for an interrupt)
  fifoCount -= packetSize;
  
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

  if (!is_offset_rotation_set) {
    is_offset_rotation_set = true;
    offset_rotation = ypr[0] * 180/M_PI;
  }
  
  Serial.print("ypr\t");
  Serial.print((ypr[0] * 180/M_PI) - offset_rotation);
  Serial.print("\t");
  Serial.print(ypr[1] * 180/M_PI);
  Serial.print("\t");
  Serial.println(ypr[2] * 180/M_PI);  
  delay(1000); 
}

void calibrate() {
  delay(3000);
  
  if (mpu.testConnection())
  {
    color(0, 0, 255); // set color to blue
    Serial.println(F("MPU CONNECTED"));
    Serial.print("CALIBRATE");
    // reset offsets
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);

    calcMean();

    offset_ax = -mean_ax / 8;
    offset_ay = -mean_ay / 8;
    offset_az = (16384 - mean_az) / 8;
    offset_gx = -mean_gx / 4;
    offset_gy = -mean_gy / 4;
    offset_gz = -mean_gz / 4;

    mpu.setXAccelOffset(offset_ax);
    mpu.setYAccelOffset(offset_ay);
    mpu.setZAccelOffset(offset_az);
    mpu.setXGyroOffset(offset_gx);
    mpu.setYGyroOffset(offset_gy);
    mpu.setZGyroOffset(offset_gz);

    boolean finished = false;
    int j = 0;

    while (!finished)
    {
      finished = true;
      
      calcMean();
  
      if (abs(mean_ax) <= acel_deadzone) {}
      else {
        offset_ax = offset_ax - mean_ax / acel_deadzone;
        finished = false;
      }
  
      if (abs(mean_ay) <= acel_deadzone) {}
      else {
        offset_ay = offset_ay - mean_ay / acel_deadzone;
        finished = false;
      }
  
      if (abs(16384-mean_az)<=acel_deadzone) {}
      else {
        offset_az = offset_az + (16384 - mean_az) / acel_deadzone;
        finished = false;
      }
  
      if (abs(mean_gx) <= giro_deadzone) {}
      else {
        offset_gx = offset_gx - mean_gx / (giro_deadzone + 1);
        finished = false;
      }
  
      if (abs(mean_gy) <= giro_deadzone) {}
      else {
        offset_gy = offset_gy-mean_gy / (giro_deadzone + 1);
        finished = false;
      }
  
      if (abs(mean_gz) <= giro_deadzone) {}
      else {
        offset_gz = offset_gz-mean_gz / (giro_deadzone + 1);
        finished = false;
      }
      
      mpu.setXAccelOffset(offset_ax);
      mpu.setYAccelOffset(offset_ay);
      mpu.setZAccelOffset(offset_az);
      mpu.setXGyroOffset(offset_gx);
      mpu.setYGyroOffset(offset_gy);
      mpu.setZGyroOffset(offset_gz);

      Serial.print(".");

      if (j > 5) {
        break;
      }
      
      j++;
      
      delay(100);
    }
    
    /*
    calcMean();
    
    Serial.println(F("MEAN MEASURED FINISH:"));
    Serial.print(F("mean_ax="));
    Serial.println(mean_ax);
    Serial.print(F("mean_ay="));
    Serial.println(mean_ay);
    Serial.print(F("mean_az="));
    Serial.println(mean_az);
    Serial.print(F("mean_gx="));
    Serial.println(mean_gx);
    Serial.print(F("mean_gy="));
    Serial.println(mean_gy);
    Serial.print(F("mean_gz="));
    Serial.println(mean_gz);

    Serial.println(F("OFFEST MEASURED FINISH:"));
    Serial.print(F("offset_ax="));
    Serial.println(offset_ax);
    Serial.print(F("offset_ay="));
    Serial.println(offset_ay);
    Serial.print(F("offset_az="));
    Serial.println(offset_az);
    Serial.print(F("offset_gx="));
    Serial.println(offset_gx);
    Serial.print(F("offset_gy="));
    Serial.println(offset_gy);
    Serial.print(F("offset_gz="));
    Serial.println(offset_gz);
    */
    Serial.println(".");
    delay(500);
    
    color(0, 255, 0); // set color to green
  }
  else
  {
    Serial.println(F("MPU NOT CONNECTED"));
    color(255, 0, 0); // set color to red
    calibrate();
  }
}

void calcMean() {
  long i = 0, buff_ax = 0, buff_ay = 0, buff_az = 0, buff_gx = 0, buff_gy = 0, buff_gz = 0;
    
  // calibrate
  while (i < (buffersize + 101)) // first 100 measures are discarded
  { 
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // read raw accel/gyro measurements from device

    if (i > 100 && i <= (buffersize + 100)) // first 100 measures are discarded
    { 
      buff_ax = buff_ax + ax;
      buff_ay = buff_ay + ay;
      buff_az = buff_az + az;
      buff_gx = buff_gx + gx;
      buff_gy = buff_gy + gy;
      buff_gz = buff_gz + gz;
    }
    if (i == (buffersize + 100))
    {
      mean_ax = buff_ax / buffersize;
      mean_ay = buff_ay / buffersize;
      mean_az = buff_az / buffersize;
      mean_gx = buff_gx / buffersize;
      mean_gy = buff_gy / buffersize;
      mean_gz = buff_gz / buffersize;
    }

    i++;
    delay(2); // needed so we don't get repeated measures
  }
}

void color(unsigned char red, unsigned char green, unsigned char blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

