#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "IoTdevices"
#define WIFI_PASSWORD "dFscvc=Itrs"

#define API_KEY "AIzaSyBfEA9-tvOhCddffdjDDEHs7hsvqCsgL18"
#define DATABASE_URL "https://ea2022-502d9-default-rtdb.firebaseio.com/" 


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int buttonPin;
int ledCode[] = {}; 
const int InfraredSensorPin[] = {34, 35, 32, 33};
const int LedPin[] = {14, 26, 27, 25}; 

const int maxValue[] = {100, 255, 180, 25}; 
int currValue[] = {25, 25, 25, 25};
int stand[] = {3, 3, 3, 3};
int code = 0;

const int maxTimer[] = {{100, 200, 100}, 400};
int timer[] = {0, 0, 0, 0};

const int freq = 57600;
const int resolution = 8;


void setup()
{
  Serial.begin(250000);
  Serial.println("Start!");
  
  pinMode(InfraredSensorPin[0], INPUT);
  pinMode(InfraredSensorPin[1], INPUT);
  pinMode(InfraredSensorPin[2], INPUT);
  pinMode(InfraredSensorPin[3], INPUT);
  
  pinMode(ledCode[0], OUTPUT);
  pinMode(ledCode[1], OUTPUT);
  pinMode(ledCode[2], OUTPUT);
  
  digitalWrite(ledCode[0], LOW);
  digitalWrite(ledCode[1], LOW);
  digitalWrite(ledCode[2], LOW);
  
  //setup leds verschillende channels (0, 1, 2, 3)
  ledcSetup(0, freq, resolution);
  ledcSetup(1, freq, resolution);
  ledcSetup(2, freq, resolution);
  ledcSetup(3, freq, resolution);  
  
  //linken van de channels aan de LedPins
  ledcAttachPin(LedPin[0], 0);
  ledcAttachPin(LedPin[1], 1);
  ledcAttachPin(LedPin[2], 2);
  ledcAttachPin(LedPin[3], 3);
  
  pinMode(buttonPin, INPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);}  
    
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");//signupOK = true;
  }
  
  else{Serial.printf("v%s\n\n", config.signer.signupError.message.c_str());}
  
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

//checkt of het nacht is 
//de mode setter (button) checkt hij continue
//als het nacht is doorloopt hij de code 
void loop()
{  
  value=analogRead(ldr);
  readButtonMode();

  if(value<300){
      keuzeCode(code);
  }
}

//checkt of er een knop ingeduwd is 
void readButtonMode(){
  if (digitalRead(buttonPin)==HIGH) {
    reset();
    digitalWrite(ledCode[code], LOW);
    code += 1;
    code %= 2;
    digitalWrite(ledCode[code], HIGH);
  }
}

// reset achter het veranderen van modes
//de lampen worden er ook in aangepast
void reset(){
  currValue[] = {25, 25, 25, 25};
  stand[] = {3, 3, 3, 3};
  timer[] = {0, 0, 0, 0};
  for (int i = 0; i < 4; i++) {
    ledcWrite(i, currValue[i]);
  } 
}

//reset timer achter er een lamp is aangestoken
//werkt maar heeft niet steeds gewenste effect (zie geval waarbij p1 wordt aangestoken terwijl p0 ook nog aanstaat (snelle auto) -> p0 wordt gereset en er wordt langer hogere standen aangehouden)
void resetTimer(int i){
  switch (i) {
  case 0:
    timer[0] == 0;
    timer[1] == 0;
    break;
  case 3:
    timer[2] == 0;
    timer[3] == 0;
    break;
  default:
    timer[i-1] = 0;
    timer[i] = 0;
    timer[i+1] = 0;
    break;
  }
}

//gebruikte de verwerkte info van rightStandMode om de lampen naar de juiste sterkte te zetten, is voor beide modus gelijk
void writeLed(int i){
    currValue[i] = maxValue[stand[i]];
    ledcWrite(i, currValue[i]);
}

//zet juiste stand en houdt een timer bij 
void rightStandMode0(int i){
    if (stand[i] <= 2){
      timer[i] += 50;
      if (timer[i] >= maxTimer[code][stand[i]]){
        timer[i] =0;
        if (stand[i] > 0){
          stand[i] -= 1;
        }
        else{
          stand[i] = 3;
        }
      }
    }
}

//mode0: autosnelweg
void sensorMode0(){
  for (int i = 0; i < 4; i++) {
    if (digitalRead(InfraredSensorPin[i]) == 0){
      stand[i] = 1;
      resetTimer(i);
      if (i != 0 and stand[i-1] == 3){
        stand[i-1] = 0;
        }
      if (i != 3 and stand[i+1] != 1){
        stand[i+1] = 2;
    }
  }
  rightStandMode0(int i)
  writeLed(int i)
}



//zet juiste stand en houdt een timer bij 
void rightStandMode1(int i){
  if (stand[i] == 2){
    timer[i] += 50;
    if (timer[i] >= maxTimer[1]){
      timer[i] = 0;
      stand[i] = 3;
    }
  }
}

//mode1: geen autosnelweg
void sensorMode1(){
  for (int i = 0; i < 4; i++) {
    if (digitalRead(InfraredSensorPin[i]) == 0){
      stand[i] = 2;
      resetTimer(i);
      if (i != 0 and stand[i-1] == 3){
        stand[i-1] = 2;
      }
      if (i != 3 and stand[i+1] != 1){
        stand[i+1] = 2;
      }
    }
  rightStandMode1(int i)
  writeLed(int i)
}

//kijkt welke code er gevraagd wordt
void keuzeCode(int code){
  switch(code) {
    case 0:
      sensorMode0();
    case 1:
      sensorMode1();
  }
}
