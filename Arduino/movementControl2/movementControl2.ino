/*
This sketch is meant to allow a laptop to interface with the movement controls of the car.
it takes one float value sent over serial

1. Steering 
if the float is 10, it wont move the steering column
if the float is 11, it'll steer to the right
if the float is 12, it'll steer to the left
if the float is 13, it will return the cars current steering position

2. Moving
if the float is from 14 to 15, the car will go forward at a speed float-14%
if the float is greater than 15 and up to 16, the car will go backwards at a speed float-15%
if the float is 17, it will return the cars' current speed 
*/

int steerPin1 = 7; //gpio pin on arduino uno
int steerPin2 = 8; //gpio pin on arduino uno
int dirPin = 6; //gpio pin on arduino uno
int speedPin = 5; //NEEDS to be a PWM pin.

String data = "";
float formattedData = 0;
float velocity = 0;
int steerDir = 0;
boolean dir;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(steerPin1, OUTPUT);
  pinMode(steerPin2, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(speedPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    getData();
    if(data != ""){
      formattedData = data.toFloat();
      if(formattedData >= 10 && formattedData <= 13){
        int steer = (int)formattedData - 10;
        if(steer <= 1){
          steerDir = steer;
          setDirection(steer);
        }
        else if(steer == 3){
          getDirection();
        }
      }
      if(formattedData >= 14 && formattedData <= 17){
        float vel = formattedData-14;
        if(vel <= 2){
          setVelocity(vel);
        }
        else if(vel == 3){
          getVelocity();
        }
      }
    }
  }
}

void getData(){
  //Serial.println("data recieved");
  data = "";
  while(Serial.available() > 0){
    char c = Serial.read();
    if(c != '\n'){
      data += c;
    }
    delay(2);
  }
  //Serial.println(data);
}

void setDirection(int dir){
  if(dir == 2){
    digitalWrite(steerPin1, HIGH);
    digitalWrite(steerPin2, LOW);
  }
  else if(dir == 1){
    digitalWrite(steerPin1, LOW);
    digitalWrite(steerPin2, HIGH);
  }
  else{
    digitalWrite(steerPin1, LOW);
    digitalWrite(steerPin2, LOW);
  }
}

//added this function in case i'm able to tell which angle the steering wheel is at.
void getDirection(){
  Serial.println(steerDir);
}

void setVelocity(float vel){
  dir = false;
  if(vel > 1){
    vel = vel - 1;
    dir = true;
  }
  velocity = vel;
  digitalWrite(dirPin, dir);
  analogWrite(speedPin, velocity*255);
}

void getVelocity(){
  if(dir == true){
    Serial.println(-velocity);
  }
  else{
    Serial.println(velocity);
  }
}
