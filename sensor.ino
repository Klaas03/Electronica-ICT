const int InfraredSensorPin = 4;//Connect the signal pin to the digital pin 4
const int LedDisp = 13;
int timer;

void setup()
{
  Serial.begin(57600);
  Serial.println("Start!");
  pinMode(InfraredSensorPin, INPUT);
  pinMode(LedDisp, OUTPUT);
  digitalWrite(LedDisp, LOW);
}

void loop()
{
  if (digitalRead(InfraredSensorPin) == 0 or timer != 0)  {
    digitalWrite(LedDisp, HIGH);
    timer += 50;
  }
  else  digitalWrite(LedDisp, LOW);
  if (timer == 5000) {
    timer = 0;
  }
  if (digitalRead(InfraredSensorPin) == 0){
    timer = 50;
  }
  Serial.print("Infrared Switch Status:");
  Serial.println(digitalRead(InfraredSensorPin), BIN);
  delay(50);
}
