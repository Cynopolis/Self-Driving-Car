/* 
This sketch is meant to allow a laptop to interface with the sonar controls of the car.
it takes one int value sent over serial.

 1. Sonar commands
 if the int is 10, it'll transmit the data, then do a sweep
 if the int is 11, it'll do a sweep and won't transmit data
 if the int is 12, it'll transmit the data without doing a sweep
 */
#include <Servo.h>
int servoPin = 3; //needs to be a PWM pin
int trigPin = 4;
int echoPin = 5;
Servo servo;
float distance[180];
int angle = 0;
String rawData = "";
int data = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(servoPin);
  Serial.begin(9600);
  for(int i = 0; i <= 180; i++){
    distance[i] = 4;
  }
}

void loop() {
  if(Serial.available()){
    getData();
    if(data != ""){
      data = rawData.toInt();
      if(data == 10){
        Serial.println(data); //tells the laptop data has been recieved.
        transmitData();
        sweep();
      }
      if(data == 11){
        Serial.println(data); //tells the laptop data has been recieved.
        sweep();
      }
      if(data == 12){
        Serial.println(data); //tells the laptop data has been recieved.
        transmitData();
      }
    }
  }
}

void transmitData(){
  for(int i = 0; i <= 179; i++){
    Serial.print(distance[i]);
    Serial.print(" ");
  }
  Serial.println("\nComplete");
}

void getData(){
  //Serial.println("data recieved");
  rawData = "";
  while(Serial.available() > 0){
    char c = Serial.read();
    if(c != '\n'){
      rawData += c;
    }
    delay(2);
  }
  //Serial.println(data);
}

void sweep(){
  if(angle == 0){
    for(angle = 0; angle <= 180; angle++){
      servo.write(angle);
      distance[angle] = getDistance();
    }
  }
  if(angle == 180){
    for(angle = 180; angle >= 0; angle--){
      servo.write(angle);
      distance[angle] = getDistance();
    }
  }
}

float getDistance(){
  digitalWrite(trigPin, LOW);
  delay(1);
  digitalWrite(trigPin, HIGH);
  delay(1);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034/2;
}
