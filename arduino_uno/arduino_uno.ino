#include <Servo.h>
#include <LiquidCrystal_I2C.h>
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
  pinMode(servo1, OUTPUT);
  pinMode(servo1, OUTPUT);
  
  lcd.setCursor(0,0);
  lcd.print("Sistem aktif mohon tunggu sebentar");
  
}
void loop(){
}
int scanAnorganik() {
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
int scanOrganik() {
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
bool checkConn(){
  return true;
}
