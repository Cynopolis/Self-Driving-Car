int pwm_a = 3;  //PWM control for motor outputs 1 and 2 
int pwm_b = 9;  //PWM control for motor outputs 3 and 4 
int dir_a = 2;  //direction control for motor outputs 1 and 2 
int dir_b = 8;  //direction control for motor outputs 3 and 4 
void setup() {
  // put your setup code here, to run once:
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  Serial.begin(9600);
  analogWrite(pwm_a, 255);  //set both motors to run at (100/255 = 39)% duty cycle (slow)
  //analogWrite(pwm_b, 100);
}

String rawData = "";
float data = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    getData();
    if(rawData != ""){
      data = rawData.toFloat();
      if(data <= 1){
        setVelocity(data);
      }
    }
  }
  Serial.println(data);
}

void getData(){
  //Serial.println("data recieved");
  rawData = "";
  while(Serial.available() > 0){
    char c = Serial.read();
    if(c != '\n'){
      rawData += c;
    }
    delay(1);
  }
  Serial.println(data);
}

void setVelocity(float vel){
  if(vel < 0){
    vel = abs(vel);
    digitalWrite(dir_a, HIGH);
  }
  else{
    digitalWrite(dir_a, LOW);
  }
  analogWrite(pwm_a, vel*255);
}
