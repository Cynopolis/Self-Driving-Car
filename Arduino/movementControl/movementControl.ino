/*
This sketch is meant to allow a laptop to interface with the movement controls of the car.
it takes one float value sent over serial

1. Steering 
Give it a float between 10 and 12 to set the steering position
The conversion to an angle is done by the equation
angle = (float-11)*30
if the float is 13, it will return the cars current steering position

2. Moving
The speed is a float between 14 and 16.
speed = float-15
if the float is 17, it will return the cars' current speed 
*/

int steerPin1 = 9; //gpio pin on arduino uno
int steerPin2 = 8; //gpio pin on arduino uno
int dirPin = 6; //gpio pin on arduino uno
int speedPin = 5; //NEEDS to be a PWM pin.
int anglePin = A0; //Needs to be an analog pin

String rawData = ""; //unformatted serial data
float data = 0; //formatted serial data
float velocity = 0; //wheel speed between 0 and 1
boolean dir; //Tells the wheel motor whether to go forward or backwards
int minAngle; //the minimum value the potentiometer can return
int maxAngle; //the maximum value the potentiometer can return
float currentAngle = 0; //the angle in degrees the car's steering wheel is at.
float targetAngle = 0; //a float between 1 and -1 to represent what angle the wheels should be at.

//max angle 30 to -30 degrees

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(steerPin1, OUTPUT);
  pinMode(steerPin2, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(speedPin, OUTPUT);
  minAngle = analogRead(anglePin);
  maxAngle = analogRead(anglePin);
  setupSteering();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    getData();
    if(rawData != ""){
      data = rawData.toFloat();

      //is the data for steering?
      if(data >= 10 && data <= 13){
        //notify computer that the data has been recieved
        Serial.println(data);
        
        data -= 11;
        if(data >= -1 && data <= 1){
          targetAngle = data*30;
        }
        if(data >= 2){
          getAngle();
        }
      }
      
      //is the data for wheel speed?
      if(data >= 14 && data <= 17){
        //notify the computer that the data has been recieved
        Serial.println(data);
        if(data <= 16){
          setVelocity(data-15);
        }
        else if(data > 16){
          getVelocity();
        }
      }
    }
  }
  setAngle(targetAngle);
}

//Parses data from the serial bus
void getData(){
  Serial.println("data recieved");
  rawData = "";
  data = 0;
  while(Serial.available() > 0){
    char c = Serial.read();
    if(c != '\n'){
      rawData += c;
    }
    delay(2);
  }
  Serial.println(rawData);
}

//moves the steering wheel to the desired angle +/- 1 degree
void setAngle(float angle){
  measureAngle();
  if(currentAngle > angle+1){
    //to the left
    digitalWrite(steerPin1, LOW);
    digitalWrite(steerPin2, HIGH);
    measureAngle();
  }
  if(currentAngle < angle-1){
    //to the right
    digitalWrite(steerPin1, HIGH);
    digitalWrite(steerPin2, LOW);
    measureAngle();
  }
  //stop turning
  if(angle+1 > currentAngle && angle-1 < currentAngle){
    digitalWrite(steerPin1, LOW);
    digitalWrite(steerPin2, LOW);
  }
}

//finds the angle the wheels are at
float measureAngle(){
  int measuredAngle = analogRead(anglePin);
  if(measuredAngle > maxAngle){
    if((measuredAngle-maxAngle) < 8){
      maxAngle = measuredAngle;
    }
  }
  if(measuredAngle < minAngle){
    if((minAngle-measuredAngle) < 8){
      minAngle = measuredAngle;
    }
  }
  //currentAngle = ((measuredAngle-minAngle-(maxAngle-minAngle)/2)/(maxAngle-minAngle))*60;
  currentAngle = 60*((measuredAngle-0.5*(minAngle + maxAngle))/(maxAngle-minAngle));
  /*Serial.print("Pot Value: ");
  Serial.print(measuredAngle);
  Serial.print(" Max Pot Value: ");
  Serial.print(maxAngle);
  Serial.print(" Min Pot Value: ");
  Serial.print(minAngle);
  Serial.print(" Angle: ");
  Serial.println(60*((measuredAngle-0.5*(minAngle + maxAngle))/(maxAngle-minAngle)));
  */
  return currentAngle;
}

//prints the angle the wheels are at to the serial bus
void getAngle(){
  measureAngle();
  Serial.println(currentAngle);
}

//Sets the speed and direction of the wheel motor
void setVelocity(float vel){
  dir = false;
  if(vel < 0){
    vel = -1*vel;
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
/*
 * moves the steering wheel all the
 * way to the left and then all the way to the right
 */
void setupSteering(){ 
  long timePassed = millis();
  while(millis()-timePassed < 10000){
    //to the left
    digitalWrite(steerPin1, HIGH);
    digitalWrite(steerPin2, LOW);
    //measureAngle();
    Serial.println(measureAngle());
  }
  timePassed = millis();
  while(millis()-timePassed < 10000){
    //to the right
    digitalWrite(steerPin1, LOW);
    digitalWrite(steerPin2, HIGH);
    //measureAngle();
    Serial.println(measureAngle());
  }
  digitalWrite(steerPin1, LOW);
  digitalWrite(steerPin2, LOW);
  Serial.println("0");
}
