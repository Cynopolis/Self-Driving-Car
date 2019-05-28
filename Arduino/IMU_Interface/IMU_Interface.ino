#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "IMU.h"

#define DT  0.02          // Loop time
#define AA  0.97         // complementary filter constant
#define G_GAIN 0.070    // [deg/s/LSB]

TinyGPSPlus gps;
SoftwareSerial gpsSerial(3, 4); //rx pin then tx pin

String gpsData; //NMEA Sentence from GPS
float roll = 0.0; //angle of the x-axis gro
float pitch = 0.0; //angle of the y-axis gro
float yaw = 0.0; //angle of the z-axis gro
float AccYangle = 0.0; //y acceleration
float AccXangle = 0.0; //x acceleration
float heading = 0; //compass heading
float CFangleX = 0.0; //filtered x-axis data
float CFangleY = 0.0; //filtered y-axis data

unsigned long startTime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gpsSerial.begin(9600);
  enableIMU();
}

void loop() {
  startTime = millis();
  getIMUData();
  updateGPS();
  //send and recieve messages here
  Serial.println("\nStream Start");
  Serial.println(gpsData);
  Serial.println(yaw);

  while(millis() - startTime < (DT*1000)){
    delay(1);
  }
  delay(1000);  
}

void updateGPS(){
  gpsData = "";
  while(gpsSerial.available() > 0){
    char rawGpsData = gpsSerial.read();
    gps.encode(rawGpsData);
    gpsData += rawGpsData;
    delay(1);
  }
}

byte buff[6];
void getIMUData(){
  //Read the measurements from  sensors
  getAccelData();
  getCompassData();
  getGyroData();
}

void correctAngle(float val){
  while(val < 0){
    val += 360;
  }
  while(val > 360){
    val -= 360;
  }
}

void getAccelData(){
  int accRaw[3];
  
  readACC(buff);
  accRaw[0] = (int)(buff[0] | (buff[1] << 8));   
  accRaw[1] = (int)(buff[2] | (buff[3] << 8));
  accRaw[2] = (int)(buff[4] | (buff[5] << 8));

  //Convert Accelerometer values to degrees
  AccXangle = (float) (atan2(accRaw[1],accRaw[2])+M_PI)*RAD_TO_DEG;
  AccYangle = (float) (atan2(accRaw[2],accRaw[0])+M_PI)*RAD_TO_DEG;

  //If IMU is up the correct way, use these lines
  AccXangle -= (float)180.0;
  if (AccYangle > 90){
    AccYangle -= (float)270;
  }
  else{
    AccYangle += (float)90;
  }
}

void getCompassData(){
  int magRaw[3];

  readMAG(buff);
  magRaw[0] = (int)(buff[0] | (buff[1] << 8));   
  magRaw[1] = (int)(buff[2] | (buff[3] << 8));
  magRaw[2] = (int)(buff[4] | (buff[5] << 8));
  
  //Compute heading  
  heading = 180 * atan2(magRaw[1], magRaw[0])/M_PI;
  
  //Convert heading to 0 - 360
  while(heading < 0){
    heading += 360;
  }
}

void getGyroData(){
  int gyrRaw[3];
  float rate_gyr_y = 0.0;   // [deg/s]
  float rate_gyr_x = 0.0;    // [deg/s]
  float rate_gyr_z = 0.0;     // [deg/s]

  readGYR(buff);
  gyrRaw[0] = (int)(buff[0] | (buff[1] << 8));   
  gyrRaw[1] = (int)(buff[2] | (buff[3] << 8));
  gyrRaw[2] = (int)(buff[4] | (buff[5] << 8));

  //Convert Gyro raw to degrees per second
  rate_gyr_x = (float) gyrRaw[0] * G_GAIN;
  rate_gyr_y = (float) gyrRaw[1]  * G_GAIN;
  rate_gyr_z = (float) gyrRaw[2]  * G_GAIN;

  //Calculate the angles from the gyro
  roll+=rate_gyr_x*DT;
  pitch+=rate_gyr_y*DT;
  yaw+=rate_gyr_z*DT;

  //correctAngle(heading);
  correctAngle(roll);
  correctAngle(pitch);
  correctAngle(yaw);

  //Complementary filter used to combine the accelerometer and gyro values.
  CFangleX=AA*(CFangleX + rate_gyr_x*DT) + (1 - AA) * AccXangle;
  CFangleY=AA*(CFangleY + rate_gyr_y*DT) + (1 - AA) * AccYangle;
}
