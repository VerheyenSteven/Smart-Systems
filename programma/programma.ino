//#include <Servo.h> 
#include "IOpins.h"
#include "Constants.h"
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

void setup() {
 
  Serial.begin(9600);
  pinMode(LmotorA,OUTPUT);        // zet de pinnen voor de H brug op output
  pinMode(RmotorA,OUTPUT);
  pinMode(LmotorB,OUTPUT);
  pinMode(RmotorB,OUTPUT);

  radio.begin();                  // voor de RF module te openen
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  
  //myservo.attach(A2);              // attaches the servo on pin 9 to the servo object 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(timeOutPin, OUTPUT);
 
}

void loop() {
   Snelheid = 1500;             // zet snelheid in de center positie
   Stuur = 1500;                // zet stuur in de center positie
   unsigned long currentMillis = millis(); // wordt gebruikt voor verschillende time-outs
   //myservo.write(10); 
  
  
  if (radio.available() )       // kijkt of er een RF signaal is
  {
    Serial.println("Het werkt");
      
    // Fetch the data payload
      
      done = radio.read(input, sizeof(input));
      
      Serial.print("Switch:  ");
      Serial.print(input[0]);
      
      Snelheid= (input[1])+1000;     // geeft de geontvangde data door aan snelheid.
      
      Serial.print(input[1]);
      Serial.print("\n");
      Serial.print("Y-axis: ");
      Serial.println(input[2]);
      Stuur  = (input[2])+1000;     // geeft de geontvangde data door aan stuur.
      Serial.print("\n\n"); 
      rij(Snelheid, Stuur);
    }

    // ------------------------------------------automatisch rijden -------------------------------------------------------

  else {
    delay(500);
    long waarde = Distance();
    if (waarde < 20 && waarde > 0){
      if (waarde < 10 && waarde > 0){
         rij(1500, 1500);
      } else{
          rij(1600, 1500);
      }  
    }
    else {
      rij(1700, 1500);
    }
      if (beginLeft == true) {
      Links30();    
  }  
  else {
    long waarde = Distance();
    if (waarde < 20 && waarde > 0){
      if(waarde < 10 && waarde > 0){
        Draaien90();
      }
      else {
        Snelheid=1600;
        Stuur= 1500;
        rij (Snelheid, Stuur);
      }
    } 
    else {
        Stuur = 1500;
        Snelheid = 1800;
        rij(Snelheid, Stuur);
    }
  }
  }


//******************************************************ZIGZAG********************************************



  

    Serial.println(Stuur);
    Serial.println(Snelheid);
  
  
    Pinkers(Stuur, currentMillis);
}

void Draaien90() {
  if (rechtsDraaien == true) { rij(1500, 1023); rechtsDraaien = false; }
  else { rij(1500, 2023); rechtsDraaien = true; }
  delay(1000);
  Stop();
}

void Stop(){
  rij(1500, 1500);
  
}

void Links30() {   
  rij(1500, 2023);
  delay(500);
  StopLinks();  
}

void StopLinks() {
  rij(1500,1500);
  beginLeft = false;
}

    //------------------------------------------------------------ Code voor RC inputs. ---------------------------------------------------------
void rij(int Snelheid, int Stuur) {

  if (abs(Snelheid-1500)<RCdeadband) Snelheid=1500;           // Als de snelheid input in de speling is dan zet men het op 1500 (standaard waarde voor meeste servo's). 
  if (abs(Stuur-1500)<RCdeadband) Stuur=1500;                 // Als de sturing input in de speling is dan zet men het op 1500 (standaard waarde voor meeste servo's).

  // Mengt de snelheid en de stuur signalen.

  Stuur=Stuur-1500;
  LinkseSnelheid=Snelheid-Stuur;
  RechtseSnelheid=Snelheid+Stuur;


  linksemodus=1;
  rechtsemodus=1;
  if (LinkseSnelheid>(Leftcenter+RCdeadband)) linksemodus=2;          // Als de linkse input voorwaarts is dan zet de linkde modus naar voorwaarts. 
  if (RechtseSnelheid>(Rightcenter+RCdeadband)) rechtsemodus=2;       // Als de rechtse input voorwaarts is dan zet de rechtse modus naar voorwaarts. 
  if (LinkseSnelheid<(Leftcenter-RCdeadband)) linksemodus=0;          
  if (RechtseSnelheid<(Rightcenter-RCdeadband)) rechtsemodus=0;

  linksePWM=abs(LinkseSnelheid-Leftcenter)*10/scale;                 // Verschaal 1000-2000uS naar 0-255.
  linksePWM=min(linksePWM,255);                                      // Zet maximum limiet 255.

  rechtsePWM=abs(RechtseSnelheid-Rightcenter)*10/scale;              // Verschaal 1000-2000uS naar 0-255.
  rechtsePWM=min(rechtsePWM,255);                                    // Zet maximum limiet 255.
   

    // --------------------------------------------------------- Code voor het aandrijven van dd dubbele "H" bruggen. --------------------------------------

  Serial.print("linksPWM ");
  Serial.print(linksemodus);
  Serial.print(" PWM:  ");
  Serial.println(linksePWM);
   Serial.print("rechtsePWM ");
   Serial.print(rechtsemodus);
  Serial.print(" PWM:  ");
   Serial.println( rechtsePWM);
  
         
  switch (linksemodus)                                    // Als de linkse motor niet recent overbelast is geweest.
  {
  case 2:                                                 // Linkse motor voorwaarts. 
    analogWrite(LmotorA,0);
    analogWrite(LmotorB,linksePWM);
    break;

  case 1:                                                 // Linkse motor stopt.
    analogWrite(LmotorA,linksePWM);
    analogWrite(LmotorB,linksePWM);
    break;

  case 0:                                                 // Linkse motor achterwaarts.
    analogWrite(LmotorA,linksePWM);
    analogWrite(LmotorB,0);
    break;
  }

  switch (rechtsemodus)                                   // Als de rechtse motor niet recent overbelast is geweest.
  {
  case 2:                                                 // Rechtse motor voorwaarts.
    analogWrite(RmotorA,0);
    analogWrite(RmotorB,rechtsePWM);
    break;

  case 1:                                                 // Rechtse motor stopt. 
    analogWrite(RmotorA,rechtsePWM);
    analogWrite(RmotorB,rechtsePWM);
    break;

  case 0:                                                 // Rechtse motor achterwaarts.
    analogWrite(RmotorA,rechtsePWM);
    analogWrite(RmotorB,0);
    break;
  }
}


//----------------------------------------------------------------DISTANCE-------------------------------------------------------------------------------------------------
long Distance() { // meet de afstand van de sensor
 
 long duration, distance;                  // Duration used to calculate distance
  
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
  // Serial.println("-1");
   digitalWrite(timeOutPin, LOW);
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
   //Serial.println(distance);
 }
  digitalWrite(timeOutPin, HIGH);
  return distance;
}


 //--------------------------------------------code voor de pinkers ----------------------------------------------------
 void Pinkers(int Stuur, unsigned long currentMillis){ // laat de pinkers pinken
   
   if (Stuur > 1550){ // als de wagen naar rechts gaat, rechter pinkers pinken
    if(currentMillis - previousMillis >= interval) {
          // save the last time you blinked the LED 

      previousMillis = currentMillis;   


       // if the LED is off turn it on and vice-versa:
       if (RledState == LOW)
           RledState = HIGH;
       else
           RledState = LOW;
          
         // set the LED with the ledState of the variable:
           digitalWrite(Rpinker, RledState);
   }
  }
   else {
       RledState = LOW;
       digitalWrite(Rpinker, RledState);
   }
         
   if (Stuur < 1450) { // als de wagen naar links gaat, linker pinkers pinken
     if(currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED 
        previousMillis = currentMillis;   

          // if the LED is off turn it on and vice-versa:
          if (LledState == LOW)
            LledState = HIGH;
          else
            LledState = LOW;
      
          // set the LED with the ledState of the variable:
          digitalWrite(Lpinker, LledState);
        }
      }
      else {
        LledState = LOW;              
        digitalWrite(Lpinker, LledState);             
      } 
}
//----------------------------------------------------------------------------------------------------------------------------
