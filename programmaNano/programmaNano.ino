#include <Servo.h>
int reader;
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long interval = 500;           // interval at which to blink (milliseconds)

#define echoPin A0          // Echo Pin
#define trigPin A1          // Trigger Pin
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
  if (reader == 1) { myservo.write(60); delay(250); reader = -1;}
  if (reader == 2) { myservo.write(100); delay(250); reader = -1; }
  if (reader == 3) { myservo.write(0); delay(250); reader = -1; }
  if (reader == 4) { myservo.write(180); delay(250); reader = -1; }
  
  Distance();
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

