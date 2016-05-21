
#define echoPin A0 // Echo Pin
#define trigPin A1 // Trigger Pin
#define LEDPin 13 // Onboard LED
#define testPin 5

#define trans A5 //analog

int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

unsigned long previousMillis = 0;
int interval1 = 2;
int invterval2 = 12;

void setup() {
 Serial.begin (9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
 pinMode(trans, OUTPUT);
 pinMode(testPin, OUTPUT);
 
}

void loop() {
  
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 

 unsigned long currentMillis = micros();
 if (currentMillis - previousMillis == 0) {
 digitalWrite(trans, HIGH);
 digitalWrite(trigPin, LOW);
 }

 if (currentMillis - previousMillis == 2) {
 digitalWrite(trigPin, HIGH); 
 }
 if (currentMillis - previousMillis >= 10) {

    previousMillis = currentMillis;
     
     
     digitalWrite(trigPin, LOW);
     duration = pulseIn(echoPin, HIGH);
     
     //Calculate the distance (in cm) based on the speed of sound.
     distance = duration/58.2;
     
     if (distance >= maximumRange || distance <= minimumRange){
     /* Send a negative number to computer and Turn LED ON 
     to indicate "out of range" */
     Serial.println("-1");
     digitalWrite(LEDPin, HIGH);
     digitalWrite(trans, LOW);
     }
     else {
     /* Send the distance to the computer using Serial protocol, and
     turn LED OFF to indicate successful reading. */
     Serial.println(distance);
     digitalWrite(LEDPin, LOW); 
     }
  }

}
