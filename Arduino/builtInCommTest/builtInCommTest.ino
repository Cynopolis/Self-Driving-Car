void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}
String str = "";
float data;

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() > 0){
    char c = Serial.read();
    if(c != '\n'){
      str += c;
    }
    delay(2);
  }
  if(str != ""){
    data = str.toFloat();
    Serial.println(data);
  }
  str = "";
  delay(100);

}
