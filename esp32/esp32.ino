#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"

#define FlashLed 4
const String url = "https://smarttrashku.com";
const char* port = "";
//const char* url = "http://231.147.11.110";
//const char* port - "8000";
const char* path[] = {"/device/get","/device/send","/device/csrf/get"};
const char* ssid = "Bismillah";
const char* password = "sudimoro";
const char* id_device = "fn6wM3n-lSq6cn3-jSM7QF2";
const char* token = "e4HJxM6ve3VLab1qz2Bs4K80QcSS2V";
unsigned long previousMillis = 0;
//const unsigned long interval = 3600000;
const unsigned long interval = 2500;
WiFiClient client;
// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

String jsonres;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);
  pinMode(FlashLed, OUTPUT); 
  delay(10);
  //analogWrite(FlashLed, 0); 
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());
//  camera_config_t config;
//  config.ledc_channel = LEDC_CHANNEL_0;
//  config.ledc_timer = LEDC_TIMER_0;
//  config.pin_d0 = Y2_GPIO_NUM;
//  config.pin_d1 = Y3_GPIO_NUM;
//  config.pin_d2 = Y4_GPIO_NUM;
//  config.pin_d3 = Y5_GPIO_NUM;
//  config.pin_d4 = Y6_GPIO_NUM;
//  config.pin_d5 = Y7_GPIO_NUM;
//  config.pin_d6 = Y8_GPIO_NUM;
//  config.pin_d7 = Y9_GPIO_NUM;
//  config.pin_xclk = XCLK_GPIO_NUM;
//  config.pin_pclk = PCLK_GPIO_NUM;
//  config.pin_vsync = VSYNC_GPIO_NUM;
//  config.pin_href = HREF_GPIO_NUM;
//  config.pin_sscb_sda = SIOD_GPIO_NUM;
//  config.pin_sscb_scl = SIOC_GPIO_NUM;
//  config.pin_pwdn = PWDN_GPIO_NUM;
//  config.pin_reset = RESET_GPIO_NUM;
//  config.xclk_freq_hz = 20000000;
//  config.pixel_format = PIXFORMAT_JPEG;
//
//  // init with high specs to pre-allocate larger buffers
//  if(psramFound()){
//    config.frame_size = FRAMESIZE_VGA;
//    config.jpeg_quality = 12;  //0-63 lower number means higher quality
//    config.fb_count = 2;
//  } else {
//    config.frame_size = FRAMESIZE_SVGA;
//    config.jpeg_quality = 8;  //0-63 lower number means higher quality
//    config.fb_count = 1;
//  }
//  
//  // camera init
//  esp_err_t err = esp_camera_init(&config);
//  if (err != ESP_OK) {
//    Serial.printf("Camera init failed with error 0x%x", err);
//    delay(1000);
//    ESP.restart();
//  }
//
//  sensor_t * s = esp_camera_sensor_get();
//  s->set_framesize(s, FRAMESIZE_SVGA); //UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA
  postData();
}

void loop() {
  unsigned long currentMillis = millis();
  //setiap 1 jam 
  if (currentMillis - previousMillis >= interval) {
    postData();
    // Update the previousMillis variable to the current time
    previousMillis = currentMillis;
  }
}
void getData(){
  HTTPClient http;
  http.begin(url+""+port+path[0]);
  int code = http.GET();
  Serial.println("getData");
  if(code>0){
    if(code == 200){
      Serial.println("Http code get : "+String(code));
      String payload = http.getString();
      Serial.println(payload);
    }else{
      Serial.println("Error code : "+String(code));
      Serial.println(http.getString());
    }
  }else{
    Serial.println("error code : "+String(code));
  }
}
String getCsrf(){
  HTTPClient http;
  http.begin(url+""+port+path[2]);
  int code = http.GET();
  if(code>0){
    if(code == 200){
      String response = http.getString();
      Serial.println(response);
      DynamicJsonDocument jsonDocument(2048);
      DeserializationError error = deserializeJson(jsonDocument, response);
      if(error){
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
      }else{
        const char* csrfToken = jsonDocument["csrf_token"];
        Serial.print("CSRF Token: ");
        Serial.println(csrfToken);
        return csrfToken;
      }
    }else{
      Serial.println("Error code : "+String(code));
      Serial.println(http.getString());
      return "";
    }
  }else{
    Serial.println("error code : "+String(code));
    return "";
  }
}
void postData(){
//  if(csrf == ""){
//    return;
//  }
  Serial.println("send post data");
  HTTPClient http;
  DynamicJsonDocument jsonData(1024);
  http.begin(url+""+port+path[1]);
  http.addHeader("Content-Type", "application/json");
  jsonData["id_device"] = id_device;
  jsonData["token"] = token;
  jsonData["organik"] = random(101);
  jsonData["anorganik"] = random(101);
  String request;
  serializeJson(jsonData, request);
  int code = http.POST(request);
  if(code>0){
    if(code == 200){
      String response = http.getString();
      DynamicJsonDocument jsonResponse(1024);
      deserializeJson(jsonResponse, response);
      Serial.println(response);
    }else{
      Serial.println("Error code : "+String(code));
      Serial.println(http.getString());
    }
  }else{
    Serial.println("error code : "+String(code));
    String response = http.getString();
    DynamicJsonDocument jsonResponse(1024);
    deserializeJson(jsonResponse, response);
    Serial.println(response);
  }
}
//void sendPhoto() {
//  String AllData;
//  String DataBody;
//  //analogWrite(FlashLed, 10);  //aktifkan flash light
//  //pre capture for accurate timing
//  for (int i = 0; i <= 3; i++) {
//    camera_fb_t * fb = NULL;
//    fb = esp_camera_fb_get();
//     if(!fb) {
//        Serial.println("Camera capture failed");
//        delay(1000);
//        ESP.restart();
//        return;
//      } 
//    esp_camera_fb_return(fb);
//    delay(200);
//  }
//  
//  camera_fb_t * fb = NULL;
//  fb = esp_camera_fb_get();
//  
//  //analogWrite(FlashLed, 0); //nonaktifkan flash light
//  
//  Serial.println("Connecting to server: " + serverName);
//
//  if (client.connect(serverName.c_str(), serverPort)) {
//    Serial.println("Connection successful!");   
//     
////    String post_data = "--dataMarker\r\nContent-Disposition: form-data; name=\"foto\"; filename=\"_esp32Photo.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
//    String head =  post_data;
//    String boundary = "\r\n--dataMarker--\r\n";
//    //Serial.println(head);
//    uint32_t imageLen = fb->len;
//    uint32_t dataLen = head.length() + boundary.length();
//    uint32_t totalLen = imageLen + dataLen;
//    
////    client.println("POST " + serverPath + " HTTP/1.1");
////    client.println("Host: " + serverName);
//    client.println("Content-Length: " + String(totalLen));
//    client.println("Content-Type: multipart/form-data; boundary=dataMarker");
//    client.println();
//    client.print(head);
//  
//    uint8_t *fbBuf = fb->buf;
//    size_t fbLen = fb->len;
//    //Serial.println(fbLen);
//    for (size_t n=0; n<fbLen; n=n+1024) {
//      if (n+1024 < fbLen) {
//        client.write(fbBuf, 1024);
//        fbBuf += 1024;
//      }
//      else if (fbLen%1024>0) {
//        size_t remainder = fbLen%1024;
//        client.write(fbBuf, remainder);
//      }
//    }   
//    client.print(boundary);
//    
//    esp_camera_fb_return(fb);
//   
//    int timoutTimer = 10000;
//    long startTimer = millis();
//    boolean state = false;
//    Serial.println("Response:");
//    while ((startTimer + timoutTimer) > millis()) {
//      Serial.print(".");
//      delay(200);
//         
//      // Skip HTTP headers   
//      while (client.available()) {
//        char c = client.read();
//        if (c == '\n') {
//          if (AllData.length()==0) { state=true; }
//          AllData = "";
//        }
//        else if (c != '\r') { AllData += String(c); }
//        if (state==true) { DataBody += String(c); }
//        startTimer = millis();
//      }
//      if (DataBody.length()>0) { break; }
//    }
//    client.stop();
//    Serial.println(DataBody);
//    Serial.println("##############");
//    Serial.println();
//    
//  }
//  else {
//    DataBody = "Connection to " + serverName +  " failed.";
//    Serial.println(DataBody);
//  }
//}
