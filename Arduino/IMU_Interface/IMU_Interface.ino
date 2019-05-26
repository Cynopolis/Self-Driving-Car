#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
//rx pin then tx pin
SoftwareSerial gpsSerial(3, 4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gpsSerial.begin(9600);
}
String data;
void loop() {
  // put your main code here, to run repeatedly:
  while(gpsSerial.available() > 0){
    char rawData = gpsSerial.read();
    gps.encode(rawData);
    data += rawData;
    delay(1);
  }
  Serial.println("\nBegin data stream");
  delay(100);
  Serial.println(data);
  delay(1000);
  data = "";
}
