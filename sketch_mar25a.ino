#define trigPin 34
#define echoPin 35
#define ledPin 32

long distance;
long duration; 

void setup() { 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  } 
  
void loop() { // every 10 µs
  distance=readUltrasonicSensor(); 
  if (distance >= 4 || distance <= 0){ 
    Serial.println("Unknown value"); } 
  else { 
    digitalWrite(ledPin, HIGH);   
    delay(1000);              
    digitalWrite(ledPin, LOW);   
    delay(1000);  } 
  delay(500);// delay in milliseconds 
  } 

long readUltrasonicSensor(){ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10) ;
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH); 
  Serial.println(duration); //Convert and return value 
  return duration*0.034/2; 
}
