int anglePin = A0;
float minAngle;
float maxAngle;
float range = 10;
float angleValues[10];
//max = 668 min = 141
//max angle range 30 to -30

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  minAngle = analogRead(anglePin);
  maxAngle = analogRead(anglePin);
  for(int i = 0; i < range; i++){
    angleValues[i] = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  float avg = average(analogRead(anglePin));
  if(avg > maxAngle && avg < 1024){
      maxAngle = avg;
  }
  if(avg < minAngle && avg > 0){
      minAngle = avg;
  }
  Serial.print("Current: ");
  Serial.println(avg);
  Serial.print(" Max: ");
  Serial.print(maxAngle);
  Serial.print(" Min: ");
  Serial.print(minAngle);
  Serial.print(" Actual Angle: ");
  float angle = (avg-0.5*(minAngle+maxAngle))/(maxAngle-minAngle)*60;
  Serial.println(angle);
  delay(1);
}

float average(float measuredAngle){
  for(int i = 0; i < range-1; i++){
    angleValues[i+1] = angleValues[i];
  }
  angleValues[0] = measuredAngle;
  float sum = 0;
  for(int i = 0; i < range; i++){
    //Serial.print(angleValues[i]);
    //Serial.print(", ");
    sum = sum + angleValues[i];
  }
  //Serial.println();
  return sum/range;
}
