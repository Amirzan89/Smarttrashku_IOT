#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial espSerial(2, 3); // RX, TX
Servo servo1, servo2;
LiquidCrystal_I2C lcd(0x27),16,2);
const int echo1 = 4;
const int echo2 = 5;
const int echo3 = 6;
const int trig1 = 7;
const int trig2 = 8;
const int trig3 = 9;
const int servoPin1 = A1;
const int servoPin2 = A2;
const int maxPos = 150;
const float maxDistance = 100.0;
void setup() {
  lcd.begin();
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  Serial.begin(9600);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(echo3, INPUT);
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(trig3, OUTPUT);
  pinMode(servoPin1, OUTPUT);
  pinMode(servo1, OUTPUT);
  espSerial.begin(9600);
  lcd.setCursor(0,0);
  lcd.print("Sistem aktif mohon tunggu sebentar");
  lcd.clear();
  delay(500);
}
void loop(){
  if (espSerial.available()) {
    char data = espSerial.read();
    Serial.print("Received from esp32 : ");
    Serial.println(data);
  }
  int volAnorganik = scanAnorganik();
  int volOrganik = scanOrganik();
  if(volAnorganik <= maxDistance){
    //hitung persen
    float percentage = (distance / maxDistance) * 100;
     Serial.println("Organik +String(percentage));
  }else{
    Serial.println("tempat sampah anorganik penuh");
    lcd.print("penuh");
  }
  if(volOrganik <= maxDistance){
    //hitung persen 
    float percentage = (distance / maxDistance) * 100;
    Serial.println("Organik +String(percentage));
    lcd.print(percentage);
    //send data to esp32
    espSerial.print(data);
  }else{
    Serial.println("tempat sampah organik penuh");
    lcd.print("penuh");
    //send data to esp32
    espSerial.print(data);
  }
}
float scanAnorganik() {
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  long duration = pulseIn(echo1,HIGH);
  //hitung jarak di cm
  int jarak = duration*0.034/2;
  return jarak;
}
float scanOrganik() {
  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  long duration = pulseIn(echo2,HIGH);
  //hitung jarak di cm
  int jarak = duration*0.034/2;
  return jarak;
}
void conServoLeft(char* condition){
  if(condition == 'open'){
    for (pos = 0; pos <= maxPos; pos += 1) {
      myservo.write(pos);
      //Jeda selama 15 milidetik
      delay(15);
    }
  }else if(condition == 'close'){
    //Servo berputar dari posisi 180 derajat ke 0 derajat
    for (pos = maxPos ; pos >= 0; pos -= 1) {
      myservo.write(pos);
      //Jeda selama 15 milidetik
      delay(15);
    }
  }
 }
void conServoRight(char* condition){
  if(condition == 'open'){
    for (pos = 0; pos <= maxPos; pos += 1) {
      myservo1.write(pos);
      //Jeda selama 15 milidetik
      delay(15);
    }
    //Servo berputar dari posisi 180 derajat ke 0 derajatd
      for (pos = maxPos; pos >= 0; pos -= 1) {
      myservo1.write(pos);
      //Jeda selama 15 milidetik
      delay(15);
    }
  }else{
    
  }
}
bool checkConn(){
  return true;
}
