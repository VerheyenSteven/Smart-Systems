#include <Servo.h>
char reader;
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long interval = 500;           // interval at which to blink (milliseconds)


Servo myservo;

void setup(){
  myservo.attach(3);  // attaches the servo on pin 11 to the servo object
  Serial.begin(9600);
  reader = '0';
  pinMode(6, HIGH);
  pinMode(7, HIGH);
  pinMode(8, HIGH);
}

void loop(){

  unsigned long currentMillis = millis();
  if(Serial.available()>1){
      reader = Serial.read();
      delay(2);
  }
  
  if (reader == '0') { myservo.write(80); }
  if (reader == '1') { myservo.write(60); }
  if (reader == '2') { myservo.write(100); }
  if (reader == '3') { myservo.write(0); }
  if (reader == '4') { myservo.write(180); }
  if (reader == '5') { digitalWrite(6, LOW); }
  if (reader == '6') { digitalWrite(7, LOW); }
  if (reader == '7') { digitalWrite(8, LOW); }
}
