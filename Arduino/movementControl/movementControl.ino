/*
This sketch is meant to allow a laptop to interface with the movement controls of the car.
it takes two different function calls written over serial
1. "wheel int" 
if the int is -1, it'll steer to the left
if the int is 0, it wont move the steering column
if the int is 1, it'll steer to the right

2. "move float"
float can be any number between -1 and 1
1 is full speed, and 0 is stopped
if the float is negative, the car will go backwards
*/
#include <AsciiMassageParser.h>
#include <AsciiMassagePacker.h>
AsciiMassageParser inbound;
AsciiMassagePacker outbound;

int steerPin1 = 7; //gpio pin on arduino uno
int steerPin2 = 8; //gpio pin on arduino uno
int dirPin = 6; //gpio pin on arduino uno
int speedPin = 5; //NEEDS to be a PWM pin.

String incoming;
float velocity;
int steerDir;

void setup() {
  pinMode(steerPin1, OUTPUT);
  pinMode(steerPin2, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(speedPin, OUTPUT);
}

void loop() {
  if(inbound.parseStream(&Serial)){
    if(inbound.fullMatch("steer")){
      int steer = inbound.nextInt();
      if(steer <= 1){
        steerDir = steer;
        setDirection(steer);
      }
      else if(steer == 2){
        getDirection();
      }
    }
    else if(inbound.fullMatch("move")){
      float mes = inbound.nextFloat();
      if(mes <= 1){
        velocity = mes;
        setVelocity(velocity);
      }
      else{
        getVelocity();
      }
    }
  }
}

void setDirection(int dir){
  if(dir == 1){
    digitalWrite(steerPin1, HIGH);
    digitalWrite(steerPin2, LOW);
  }
  else if(dir == -1){
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
  //double dir = 0;
  outbound.beginPacket("steer");
  //do something here to find the angle of the wheel
  outbound.addInt(steerDir);
  outbound.streamPacket(&Serial);
}

void setVelocity(float velocity){
  boolean dir = false;
  if(velocity < 0){
    velocity = abs(velocity);
    dir = true;
  }
  digitalWrite(dirPin, dir);
  analogWrite(speedPin, velocity/255);
}

void getVelocity(){
  outbound.beginPacket("move");
  outbound.addFloat(velocity);
  outbound.streamPacket(&Serial);
}
