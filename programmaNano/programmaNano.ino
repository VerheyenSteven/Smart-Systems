#include <Servo.h>
int reader;
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        
unsigned long previousMillisStappenmotor = 0;        

const long interval = 50;           
const long intervalStappenmotor = 10;   

#define echoPin 5          // Echo Pin
#define trigPin 4          // Trigger Pin
#define timeOutPin A5       //De pin om de sensor aan en uit te doen

int maximumRange = 200;                   // Maximum range needed
int minimumRange = 0;                     // Minimum range needed


Servo myservo;

void setup(){
  myservo.attach(3);  // attaches the servo on pin 11 to the servo object
  Serial.begin(9600);
  reader = 0;

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(timeOutPin, OUTPUT);
}

void loop(){

  unsigned long currentMillis = millis();
  if (Serial.available() > 1) {
      reader = Serial.parseInt();
      delay(2);
  }
  
  if (reader == 0) { myservo.write(80);  delay(250); reader = -1; }
  else if (reader == 1) { myservo.write(60); delay(250); reader = -1;}
  else if (reader == 2) { myservo.write(100); delay(250); reader = -1; }
  else if (reader == 3) { myservo.write(0); delay(550); reader = -1; }
  else if (reader == 4) { myservo.write(180); delay(550); reader = -1; }
  else{
    
     if (currentMillis - previousMillis >= interval) {
        Distance();
        previousMillis = currentMillis;
     }
    }  
}

void Distance() { // meet de afstand van de sensor
 
  long duration, distance;                  // Duration used to calculate distance
  digitalWrite(timeOutPin, HIGH);
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
 
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
 
  if (distance >= maximumRange || distance <= minimumRange){
    /* Send a negative number to computer and Turn LED ON 
    to indicate "out of range" */
    distance = 201;
    digitalWrite(timeOutPin, LOW);
  }
  else { 
    /* Send the distance to the computer using Serial protocol, and
    turn LED OFF to indicate successful reading. */
    //Serial.println(distance);
  }
  Serial.println(distance);
}

void 1draaiNaarBeneden(){
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor && currentMillis - previousMillisStappenmotor < intervalStappenmotor+10) {
    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);
  }

  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor+10 && currentMillis - previousMillisStappenmotor < intervalStappenmotor+20) {
  
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor+20 && currentMillis - previousMillisStappenmotor < intervalStappenmotor+30) {

    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);

  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor+30 && currentMillis - previousMillisStappenmotor < intervalStappenmotor+40) {

    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, LOW);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor+40&& currentMillis - previousMillisStappenmotor < intervalStappenmotor+50) {
  
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor+50 && currentMillis - previousMillisStappenmotor < intervalStappenmotor+60) {

    digitalWrite(coila1, LOW);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor+60 && currentMillis - previousMillisStappenmotor < intervalStappenmotor+70) {

    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor+70) {
  previousMillisStappenmotor = currentMillis;
    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, LOW);
  }
}

void 1draaiNaarBoven(){
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor && currentMillis - previousMillisStappenmotor < intervalStappenmotor+10) {

    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, LOW);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor && currentMillis - previousMillisStappenmotor < intervalStappenmotor+20) {

    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor && currentMillis - previousMillisStappenmotor < intervalStappenmotor+30) {

    digitalWrite(coila1, LOW);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor && currentMillis - previousMillisStappenmotor < intervalStappenmotor+40) {

    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor && currentMillis - previousMillisStappenmotor < intervalStappenmotor+50) {

    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, LOW);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor && currentMillis - previousMillisStappenmotor < intervalStappenmotor+60) {

    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor && currentMillis - previousMillisStappenmotor < intervalStappenmotor+70) {

    digitalWrite(coila1, LOW);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);
  }
  if (currentMillis - previousMillisStappenmotor >= intervalStappenmotor) {
    previousMillisStappenmotor = currentMillis;
    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);
  }
}

