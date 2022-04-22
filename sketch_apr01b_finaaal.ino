#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define WIFI_SSID "IoTdevices"
#define WIFI_PASSWORD "dFscvc=Itrs"

#define API_KEY "AIzaSyBfEA9-tvOhCddffdjDDEHs7hsvqCsgL18"
#define DATABASE_URL "https://ea2022-502d9-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config; 

const int InfraredSensorPin1 = 34;//Connect the signal pin to the digital pin 4
const int InfraredSensorPin2 = 35;
const int InfraredSensorPin3 = 32;
const int InfraredSensorPin4 = 33;
const int LedPin1 = 14;
const int LedPin2 = 27;
const int LedPin3 = 26;
const int LedPin4 = 25;
int timer1 = 0;
int timer2 = 0;
int timer3 = 0;
int timer4 = 0;

void setup()
{
  Serial.begin(57600);
  Serial.println("Start!");
  pinMode(InfraredSensorPin1, INPUT);
  pinMode(InfraredSensorPin2, INPUT);
  pinMode(InfraredSensorPin3, INPUT);
  pinMode(InfraredSensorPin4, INPUT);
  pinMode(LedPin1, OUTPUT);
  pinMode(LedPin2, OUTPUT);
  pinMode(LedPin3, OUTPUT);
  pinMode(LedPin4, OUTPUT);
  digitalWrite(LedPin1, LOW);
  digitalWrite(LedPin2, LOW);
  digitalWrite(LedPin3, LOW);
  digitalWrite(LedPin4, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
//    signupOK = true;
  }
  else{
    Serial.printf("v%s\n\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  if (digitalRead(InfraredSensorPin1) == 0 or timer1 != 0)  {
    digitalWrite(LedPin1, HIGH);
    timer1 += 50;
  }
  else  digitalWrite(LedPin1, LOW);
  if (timer1 == 5000) {
    timer1 = 0;
  }
  if (digitalRead(InfraredSensorPin1) == 0){
    timer1 = 50;
  }

  if (digitalRead(InfraredSensorPin2) == 0 or timer2 != 0)  {
    digitalWrite(LedPin2, HIGH);
    timer2 += 50;
  }
  else  digitalWrite(LedPin2, LOW);
  if (timer2 == 5000) {
    timer2 = 0;
  }
  if (digitalRead(InfraredSensorPin2) == 0){
    timer2 = 50;
  }

  if (digitalRead(InfraredSensorPin3) == 0 or timer3 != 0)  {
    digitalWrite(LedPin3, HIGH);
    timer3 += 50;
  }
  else  digitalWrite(LedPin3, LOW);
  if (timer3 == 5000) {
    timer3 = 0;
  }
  if (digitalRead(InfraredSensorPin3) == 0){
    timer3 = 50;
  }

  if (digitalRead(InfraredSensorPin4) == 0 or timer4 != 0)  {
    digitalWrite(LedPin4, HIGH);
    timer4 += 50;
  }
  else  digitalWrite(LedPin4, LOW);
  if (timer4 == 5000) {
    timer4 = 0;
  }
  if (digitalRead(InfraredSensorPin4) == 0){
    timer4 = 50;
  }
  Serial.print("Infrared Switch Status:");
  Serial.print(digitalRead(InfraredSensorPin1), BIN);
  Serial.print(digitalRead(InfraredSensorPin2), BIN);
  Serial.print(digitalRead(InfraredSensorPin3), BIN);
  Serial.println(digitalRead(InfraredSensorPin4), BIN);
  delay(50);
}
