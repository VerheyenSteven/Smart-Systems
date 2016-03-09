/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
int pos = 0;    // variable to store the servo position 

#define echoPin A0 // Echo Pin
#define trigPin A1 // Trigger Pin
#define timeOutPin A5


int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

unsigned long previousMillisServo = 0;  // will store last time LED was updated
const long intervalServo = 1000; // interval at which to blink (milliseconds)
 
void setup() 
{ 
  Serial.begin(9600);
  myservo.attach(3);  // attaches the servo on pin 9 to the servo object 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(timeOutPin, OUTPUT);
} 
 
void loop() 
{   
 unsigned long currentMillisServo = millis();
 
   if(currentMillisServo - previousMillisServo >= intervalServo) {
     
     previousMillisServo = currentMillisServo;     
     myservo.write(pos);  
     Distance();
     
     if (pos == 180) pos = 0;
     else pos = pos + 90;
   }
} 

void Distance() {
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
 Serial.println("-1");
 digitalWrite(timeOutPin, LOW);
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
   Serial.println(distance);

 }
 digitalWrite(timeOutPin, HIGH);
 }

