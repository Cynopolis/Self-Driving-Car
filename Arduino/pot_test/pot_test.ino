int anglePin = A0;
float measuredAngle = 0;
float minAngle = 1000;
float maxAngle = 0;
//max = 668 min = 141
//max angle 30 to -30

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  minAngle = analogRead(anglePin);
  maxAngle = analogRead(anglePin);
}

void loop() {
  // put your main code here, to run repeatedly:
  measuredAngle = analogRead(anglePin);
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
  Serial.print("Current: ");
  Serial.print(measuredAngle);
  Serial.print(" Max: ");
  Serial.print(maxAngle);
  Serial.print(" Min: ");
  Serial.print(minAngle);
  Serial.print(" Actual Angle: ");
  Serial.println(((measuredAngle-minAngle-(maxAngle-minAngle)/2)/(maxAngle-minAngle))*60);
  delay(1);
}
