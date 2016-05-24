#include <Servo.h>
int reader;
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;              
bool stippenlijn = false;
bool boven= false;
bool beneden = true;
const long interval = 10;           

#define echoPin 5          // Echo Pin
#define trigPin 4          // Trigger Pin
#define timeOutPin A5       //De pin om de sensor aan en uit te doen

int coila1 = 6;
int coila2 = 7;
int coilb1 = 8;
int coilb2 = 9;
int enable = 10;

int draainaarbeneden = 0;
int middenpuntpositie = 0;
int draainaarboven =0;
int positie = 0;

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
  pinMode(coila1, OUTPUT);
  pinMode(coila2, OUTPUT);
  pinMode(coilb1, OUTPUT);
  pinMode(coilb2, OUTPUT);
  pinMode(enable, OUTPUT);
}

void loop(){

  unsigned long currentMillis = millis();
  if (Serial.available() > 1) {
      reader = Serial.parseInt();
      delay(2);
  }
  
  if (reader == 0) { myservo.write(80);  delay(500); reader = -1; }
  else if (reader == 1) { myservo.write(60); delay(500); reader = -1;}
  else if (reader == 2) { myservo.write(100); delay(500); reader = -1; }
  else if (reader == 3) { myservo.write(0); delay(1000); reader = -1; }
  else if (reader == 4) { myservo.write(180); delay(1000); reader = -1; }
  else if (reader == 6) {  beneden = true; stippenlijn = false; reader = -1; }
  else if (reader == 7) {stippenlijn = true ; beneden = false; reader = -1; }
  else{

  if(beneden){

    draainaarbeneden = 10 - middenpuntpositie;
    beneden=false;

 }

  if(stippenlijn && !beneden){
    if(middenpuntpositie<=0){
       draainaarbeneden = 10 - middenpuntpositie;
    }
    if(middenpuntpositie>=10){
      draainaarboven = middenpuntpositie;
    }
  }
    
     if (currentMillis - previousMillis >= interval) {

        Distance();
            if(draainaarboven>0){
              digitalWrite(enable, HIGH);
        
              switch(positie){
                case 0:
                  a1Enb2();
                  positie++;
                  break;
                case 1:
                  b2();
                  positie++;
                  break;
                case 2:
                  b2Ena2();
                  positie++;
                  break;
                case 3:
                  a2();
                  positie++;
                  break;
                case 4:
                  a2Enb1();
                  positie++;
                  break;
                case 5:
                  b1();
                  positie++;
                  break;
                case 6:
                  b1Ena1();
                  positie++;
                  break;
                case 7:
                  a1();
                  positie = 0;
                  draainaarboven--;
                  middenpuntpositie--;
                  break;
              }
              
            }else if(draainaarbeneden>0){
              digitalWrite(enable, HIGH);
        
              switch(positie){
                case 0:
                  a1();
                  positie ++;
                  break;
                case 1:
                  b1Ena1();
                  positie++;
                  break;
                case 2:
                  b1();
                  positie++;
                  break;
                case 3:
                  a2Enb1();
                  positie++;
                  break;
                case 4:
                  a2();
                  positie++;
                  break;
                case 5:
                  b2Ena2();
                  positie++;
                  break;
                case 6:
                  b2();
                  positie++;
                  break;
                case 7:
                  a1Enb2();
                  positie = 0;
                  draainaarbeneden--;
                  middenpuntpositie++;
                  break;
              }
              
            }
            else{
              digitalWrite(enable, LOW);
            }
        previousMillis = currentMillis;
     
    }
  }
   
}

void Distance(){ // meet de afstand van de sensor
 
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



void a1Enb2(){
    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);
}

void b2(){
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);
}
void b2Ena2(){
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);
}
void a2(){
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, LOW);
}
void a2Enb1(){
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
}
void b1(){
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
}
void b1Ena1(){
    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
}
void a1(){
    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, LOW);
}



