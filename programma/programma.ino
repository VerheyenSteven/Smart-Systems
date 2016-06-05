
#include "IOpins.h"
#include "Constants.h"
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(3, 2); // RX, TX , maakt een serieele poort aan op pin 3 en 2
int RadioZekerUit=0;


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
bool schuinKijken = false;
void setup() {

  Serial.begin(9600);
  //mySerial.begin(9600); 
  pinMode(LmotorA,OUTPUT);        // zet de pinnen voor de H brug op output
  pinMode(RmotorA,OUTPUT);
  pinMode(LmotorB,OUTPUT);
  pinMode(RmotorB,OUTPUT);

  pinMode(Achteruit, OUTPUT);
  pinMode(Roses, OUTPUT);
  pinMode(Vooruit, OUTPUT);


  radio.begin();                  // voor de RF module te openen
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}

void loop() {
   Snelheid = 1500;             // zet snelheid in de center positie
   Stuur = 1500;                // zet stuur in de center positie
   currentMillis = millis(); // wordt gebruikt voor verschillende time-outs

   


 if (radio.available() )       // kijkt of er een RF signaal is
  {
    RadioZekerUit = 0;
    if(!myserialuit){
      Serial.end();
      myserialuit = true;
    }
   
    // Fetch the data payload
      
      done = radio.read(input, sizeof(input));
      
      Snelheid= (input[1])+1000;     // geeft de geontvangde data door aan snelheid.
      /*if (Snelheid < 1300) digitalWrite(Achteruit, LOW);
      else digitalWrite(Achteruit, HIGH);*/

      /*if (Snelheid > 1700) digitalWrite(Vooruit, LOW);
      else digitalWrite(Vooruit, HIGH);*/
      Stuur  = (input[2])+1000;     // geeft de geontvangde data door aan stuur.
      Rij();
     
      
    }
  else if(RadioZekerUit>3000) {
    
    
      if(myserialuit){
        Serial.begin(9600);
        myserialuit = false;
      }
  
      if (Serial.available() > 1) {
        reader = Serial.parseInt();
        delay(2);
      }
  
  
  
       Automatichrijden();


  }else if(RadioZekerUit<3002){
       RadioZekerUit ++;
    }
}



//*-------------------------------------Methodes automatisch rijden----------------------------------------*/
void Automatichrijden(){
    if(reader == 11){
      Stoop(); 
    }else if(reader == 2){
      Snelheid = 1650;
      Stuur = 1500;
      Rij();  
    }else if(reader == 5){
      Snelheid = 1800;
      Stuur = 1500;
      Rij();  
    }else if(reader == 3){
      RechtsDraaien90();
    }
    else if(reader == 4){
      LinksDraaien90();
    }else{
      Stoop();
    }
}

void RechtsDraaien90(){ 
    Snelheid = 1500;
    Stuur = 2023;
    Rij();
    delay(1500);
    Stoop(); 
    reader=0; 
    delay(1500);
}


void LinksDraaien90(){

    Snelheid = 1500;
    Stuur = 1023;
    Rij();
    delay(1500);
    Stoop(); 
    reader=0;  
    delay(1500);
}

void AchteruitRijden(){
  Snelheid = 1500;
  Stuur = 2023;
  Rij();
  delay(2000);
  Stoop();
}

void Stoop(){
  Snelheid = 1500;
  Stuur = 1500;
  Rij();
  Stuur = 1500;

}

//----------------------------------------------Methodes voor zigzag---------------------------------------

/*void Draaien90() {
    Snelheid = 1500;
    if (rechtsDraaien == true) {Stuur = 2023; rechtsDraaien = false;}
    else { Stuur = 1023; rechtsDraaien = true; }
    Rij();
    delay(2000);
    Stoop();
    delay(200);
}*/

void Links30() { 
  Snelheid = 1500;
  Stuur = 2023;  
  Rij();
  delay(500);
  Stoop();  
}


    //------------------------------------------------------------ Code voor RC inputs. ---------------------------------------------------------
void Rij() {

  Pinkers(Stuur, currentMillis);


  if (abs(Snelheid-1500)<RCdeadband) Snelheid=1500;           // Als de snelheid input in de speling is dan zet men het op 1500 (standaard reader voor meeste servo's). 
  if (abs(Stuur-1500)<RCdeadband) Stuur=1500;                 // Als de sturing input in de speling is dan zet men het op 1500 (standaard reader voor meeste servo's).

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

    switch (linksemodus)                                    // Als de linkse motor niet recent overbelast is geweest.
    {
    case 2:                                                 // Linkse motor voorwaarts. 
      analogWrite(LmotorA,0);
      analogWrite(LmotorB,linksePWM);
      break;
  
    case 1:                                                 // Linkse motor Stoopt.
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
  
    case 1:                                                 // Rechtse motor Stoopt. 
      analogWrite(RmotorA,rechtsePWM);
      analogWrite(RmotorB,rechtsePWM);
      break;
  
    case 0:                                                 // Rechtse motor achterwaarts.
      analogWrite(RmotorA,rechtsePWM);
      analogWrite(RmotorB,0);
      break;
    }
}

 //--------------------------------------------code voor de pinkers ----------------------------------------------------*/
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
