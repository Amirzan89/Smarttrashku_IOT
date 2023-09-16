#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
//#include <NewPing.h>
SoftwareSerial espSerial(2, 3); // RX, TX
Servo servoKiri, servoKanan;
LiquidCrystal_I2C lcd(0x27,16,2);
//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
const int trigSonar = 4;
const int echoSonar = 5;
const int trigKiri = 6;
const int echoKiri = 7;
const int trigKanan = 8;
const int echoKanan = 9;
const int pinServoKiri = A1;
const int pinServoKanan = A2;
const int maxPos = 150;
const float maxDistance = 100.0;
void setup() {
  Serial.begin(9600);
  Serial.println("sistem on");
  servoKiri.attach(pinServoKiri);
  servoKanan.attach(pinServoKanan);
  servoKiri.write(150);
  servoKanan.write(0);
  lcd.init();
  lcd.backlight();
  pinMode(echoSonar, INPUT);
  pinMode(echoKiri, INPUT);
  pinMode(echoKanan, INPUT);
  pinMode(trigSonar, OUTPUT);
  pinMode(trigKiri, OUTPUT);
  pinMode(trigKanan, OUTPUT);
  espSerial.begin(9600);
  lcd.setCursor(0,0);
  lcd.print("Sistem aktif mohon tunggu sebentar");
  delay(2000);
  lcd.clear();
  delay(500);
//  conServoKiri(true);
//  delay(500);
//  conServoKiri(false);
//  delay(2500);
//  conServoKanan(true);
//  delay(500);
//  conServoKanan(false);
//  delay(2500);
//  conServoKiri(true);
//  delay(500);
//  conServoKiri(false);
//  delay(2500);
//  conServoKanan(true);
//  delay(500);
//  conServoKanan(false);
//  delay(2500);
}
void loop(){
  int sonar = scanSonar();
  if (sonar <= 20 && sonar >= 0) {
    Serial.println("Object detected within 20cm. Waking up...");
    // Wake up Arduino and execute rest of the code
    wakeUp();
    lcd.print("servo kiri");
    conServoKiri(true);
    delay(1000);
    conServoKiri(false);
    delay(2000);
    lcd.clear();
    lcd.print("servo kanan");
    conServoKanan(true);
    delay(1000);
    conServoKanan(false);
    lcd.clear();
    delay(2000);
    // Rest of your code here...
  } else {
    // Put Arduino to sleep
    goToSleep();
  }
//  if (espSerial.available()) {
//    char data = espSerial.read();
//    Serial.print("Received from esp32 : ");
//    Serial.println(data);
//  }
//  int volAnorganik = scanAnorganik();
//  int volOrganik = scanOrganik();
//  if(volAnorganik <= maxDistance){
//    //hitung persen
//    float percentage = (distance / maxDistance) * 100;
//     Serial.println("Organik +String(percentage));
//  }else{
//    Serial.println("tempat sampah anorganik penuh");
//    lcd.print("penuh");
//  }
//  if(volOrganik <= maxDistance){
//    //hitung persen 
//    float percentage = (distance / maxDistance) * 100;
//    Serial.println("Organik +String(percentage));
//    lcd.print(percentage);
//    //send data to esp32
//    espSerial.print(data);
//  }else{
//    Serial.println("tempat sampah organik penuh");
//    lcd.print("penuh");
//    //send data to esp32
//    espSerial.print(data);
//  }
}
void wakeUp() {
  // Disable sleep mode
  sleep_disable();
}

void goToSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  // Enable sleep mode
  sleep_enable();

  // Enter sleep mode
  sleep_mode();
}
int scanSonar() {
  Serial.println("scan sonar");
  digitalWrite(trigSonar, LOW);
  delayMicroseconds(2);
  digitalWrite(trigSonar, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigSonar, LOW);
  long duration = pulseIn(echoSonar,HIGH);
  //hitung jarak di cm
  int jarak = duration*0.034/2;
  Serial.println("jarak "+ String(jarak));
  return jarak;
}
float scanKiri() {
  digitalWrite(trigKiri, LOW);
  delayMicroseconds(2);
  digitalWrite(trigKiri, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigKiri, LOW);
  long duration = pulseIn(echoKanan,HIGH);
  //hitung jarak di cm
  int jarak = duration*0.034/2;
  return jarak;
}
float scanKanan() {
  digitalWrite(trigKanan, LOW);
  delayMicroseconds(2);
  digitalWrite(trigKanan, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigKanan, LOW);
  long duration = pulseIn(echoKanan,HIGH);
  //hitung jarak di cm
  int jarak = duration*0.034/2;
  return jarak;
}
void conServoKiri(bool condition){
  int pos;
  Serial.println("servo kiri");
  if(condition == true){
    //buka sampah kiri
    for (pos = maxPos; pos >= 0; pos -= 3) {
      servoKiri.write(pos);
      delay(5);
    }
  }else{
    //tutup sampah kiri
    for (pos = 0; pos <= maxPos; pos += 3) {
      servoKiri.write(pos);
      delay(5);
    }
  }
}
void conServoKanan(bool condition){
  int pos;
  Serial.println("servo kanan");
  if(condition == true){
    //buka sampah kanan
    for (pos = 0; pos <= maxPos; pos += 3) {
      servoKanan.write(pos);
      delay(5);
    }
  }else{
    //tutup sampah kanan
    for (pos = maxPos; pos >= 0; pos -= 3) {
      servoKanan.write(pos);
      delay(5);
    }
  }
}
