/*
This sketch is meant to allow a laptop to interface with the movement controls of the car.
it takes one float value sent over serial

1. Steering 
Give it a float between 10 and 12 to set the steering position
The conversion to an angle is done by the equation
angle = (float-11)*30
if the float is 13, it will return the cars current steering position

2. Moving
Give a float between 14 and 16 to set the wheel velocity
The conversion to a velocity is done by the equation (negative values mean reverse)
velocity = (float-15)
if the float is 17, it will return the cars' current wheel velocity 
*/

int steerPin1 = 9; //gpio pin on arduino uno. Controls the direction the steering wheel turns
int steerPin2 = 8; //gpio pin on arduino uno. Controls the direction the steering wheel turns
int dirPin = 6; //gpio pin on arduino uno. Controls the direction the car moves
int speedPin = 5; //NEEDS to be a PWM pin. Controls the speed the car moves
int anglePin = A0; //Needs to be an analog pin. Recieves angle data from the steering column.

String rawData = ""; //unformatted serial data
float data = 0; //formatted serial data
float velocity = 0; //wheel speed between 0 and 1
boolean dir; //Tells the wheel motor whether to go forward or backwards
int minAngle; //the minimum value the potentiometer can return
int maxAngle; //the maximum value the potentiometer can return
float realAngle = 0; //the angle in degrees the car's steering wheel is at.
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
        data -= 11;
        if(data >= -1 && data <= 1){
          setAngle(data*30);
        }
        if(data >= 2){
          getAngle();
        }
      }
      
      //is the data for wheel velocity?
      if(data >= 14 && data <= 17){
        float vel = data-15;
        if(vel <= 1){
          setVelocity(vel);
        }
        else if(vel == 2){
          getVelocity();
        }
      }
    }
  }
}

//Parses data from the serial bus
void getData(){
  //Serial.println("data recieved");
  rawData = "";
  data = 0;
  while(Serial.available() > 0){
    char c = Serial.read();
    if(c != '\n'){
      rawData += c;
    }
    delay(2);
  }
  //Serial.println(rawData);
}

//moves the steering wheel to the desired angle +/- 1 degree
void setAngle(float angle){
  measureAngle();
  long timePassed = millis();
  while(realAngle > angle+1 && millis()-timePassed < 3000){
    //to the left
    digitalWrite(steerPin1, HIGH);
    digitalWrite(steerPin2, LOW);
    measureAngle();
  }
  while(realAngle < angle-1 && millis()-timePassed < 3000){
    //to the right
    digitalWrite(steerPin1, LOW);
    digitalWrite(steerPin2, HIGH);
    measureAngle();
  }
  //to the right
  digitalWrite(steerPin1, LOW);
  digitalWrite(steerPin2, LOW);
}

//finds the angle the wheels are at
float measureAngle(){
  int measuredAngle = analogRead(anglePin);
  if(measuredAngle > maxAngle){
    if((measuredAngle-maxAngle) < 5){
      maxAngle = measuredAngle;
    }
  }
  if(measuredAngle < minAngle){
    if((minAngle-measuredAngle) < 5){
      minAngle = measuredAngle;
    }
  }
  //realAngle = ((measuredAngle-minAngle-(maxAngle-minAngle)/2)/(maxAngle-minAngle))*60;
  realAngle = 60*((measuredAngle-0.5*(minAngle + maxAngle))/(maxAngle-minAngle));
  Serial.print("Pot Value: ");
  Serial.print(measuredAngle);
  Serial.print(" Max Pot Value: ");
  Serial.print(maxAngle);
  Serial.print(" Min Pot Value: ");
  Serial.print(minAngle);
  Serial.print(" Angle: ");
  Serial.println(60*((measuredAngle-0.5*(minAngle + maxAngle))/(maxAngle-minAngle)));
  return realAngle;
}

//prints the steering wheel angle to serial
void getAngle(){
  measureAngle();
  Serial.println(realAngle);
}

//Sets the speed and direction of the wheel motor
void setVelocity(float vel){
  dir = false;
  velocity = vel;
  if(vel < 0){
    dir = true;
    vel = vel *(-1);
  }
  digitalWrite(dirPin, dir);
  analogWrite(speedPin, velocity*255);
}

void getVelocity(){
  Serial.println(velocity);
}

/*
 * moves the steering wheel all the
 * way to the left and then all the way to the right
 */
void setupSteering(){ 
  long timePassed = millis();
  while(millis()-timePassed < 0){
    //to the left
    digitalWrite(steerPin1, HIGH);
    digitalWrite(steerPin2, LOW);
    //measureAngle();
    Serial.println(measureAngle());
  }
  timePassed = millis();
  while(millis()-timePassed < 0){
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
