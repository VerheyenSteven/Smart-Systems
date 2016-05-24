
#include "IOpins.h"
#include "Constants.h"
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
bool schuinKijken = false;
void setup() {
 
  Serial.begin(9600);
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
    //Serial.println("Het werkt");
      
    // Fetch the data payload
      
      done = radio.read(input, sizeof(input));
      
      //Serial.print("Switch:  ");
      //Serial.print(input[0]);
      
      Snelheid= (input[1])+1000;     // geeft de geontvangde data door aan snelheid.
      if (Snelheid < 1300) digitalWrite(Achteruit, LOW);
      else digitalWrite(Achteruit, HIGH);

      if (Snelheid > 1700) digitalWrite(Vooruit, LOW);
      else digitalWrite(Vooruit, HIGH);
      
      //Serial.print(input[1]);
      //Serial.print("\n");
      //Serial.print("Y-axis: ");
      //Serial.println(input[2]);
      Stuur  = (input[2])+1000;     // geeft de geontvangde data door aan stuur.
      //Serial.print("\n\n"); 
      Rij();
      
    }

    // ------------------------------------------automatisch rijden -------------------------------------------------------

  else {

  /*if (Serial.available() > 1) {
      reader = Serial.parseInt();
      delay(2);
  }

    if (reader < 25 && reader > 0){

      if (reader < 10 && reader > 0){
        schuinKijken = false;
        Stoop();
        rechtseAfstand = RechtsKijken();
        
        if (rechtseAfstand> 20){
          RechtsDraaien();
        } 
        else {
         linkseAfstand = LinksKijken();
         if (linkseAfstand>20){
          LinksDraaien();
          } 
          else {
            AchteruitRijden;
          }
        }
        Serial.println(0);

      } 
      else {
            Stoop();

          if (schuinKijken == false){ 
            schuinKijken = true; 
            rechtseAfstand = RechtsKijken20();            
            linkseAfstand = LinksKijken20();
          }
         
            if( rechtseAfstand < 25 || linkseAfstand < 25){
              if(rechtseAfstand < linkseAfstand) Serial.println(1);
              else Serial.println(2);
            }
            else Serial.println(0);
        

        Snelheid = 1600;
        Stuur = 1500;
        Rij();
      }  
    }
    else {
      //Serial.println(6);
      schuinKijken = false;
      Snelheid = 1800;
      Stuur = 1500;
      Rij();
    }
 }*/


//******************************************************ZIGZAG********************************************
   if (Serial.available() > 1) {
        reader = Serial.parseInt();
        delay(2);
    }
    
    if (beginLeft == true) {
        Links30();
        digitalWrite(Roses, LOW);
        Serial.println(6);
           
    }  
    else {
      long reader = getDistance();
      if (reader < 30 && reader > 0){
        if(reader < 20 && reader > 0){
          Draaien90();
        }
        else {
          Snelheid = 1600;
          Stuur = 1500;
          Rij();
        }
      } 
      else {      
        Snelheid = 1700;
        Stuur = 1500;
        Rij();
      }
    }
  }
}

//-------------------------------------Methodes automatisch rijden----------------------------------------

int RechtsKijken20(){
  reader = 0; 
  Serial.println(1); 
  delay(500);   
  return getDistance();
}  

int LinksKijken20() {
  reader = 0;
  Serial.println(2);
  delay(500);
  return getDistance();
}

int RechtsKijken() {
  reader = 0;
  Serial.println(3);
  delay(500);
  return getDistance();  
}

int RechtsDraaien(){ 
    Snelheid = 1500;
    Stuur = 2023;
    Rij();
    delay(1000);
    Stoop();  
}

int LinksKijken() {
  reader = 0;
  Serial.println(4);
  delay(500);
  return getDistance(); 
}

void LinksDraaien(){

    Snelheid = 1500;
    Stuur = 1023;
    Rij();
    delay(1000);
    Stoop();  
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
}

//----------------------------------------------Methodes voor zigzag---------------------------------------

void Draaien90() {
  Snelheid = 1500;
  if (rechtsDraaien == true) {Stuur = 2023; rechtsDraaien = false; Serial.println(6);}
  else { Stuur = 1023; rechtsDraaien = true; Serial.println(7); }
  Rij();
  delay(1000);
  Stoop();
}

void Links30() { 
  Snelheid = 1500;
  Stuur = 2023;  
  Rij();
  delay(500);
  StoopLinks();  
}

void StoopLinks() {
  Snelheid = 1500;
  Stuur = 1500;
  Rij();
  beginLeft = false;
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


//----------------------------------------------------------------DISTANCE-------------------------------------------------------------------------------------------------
int getDistance() {

    
  int count = 0;
  while (count <= 5 && reader == 201 || reader == 0) {
    count++;
    if(Serial.available()>1){
      reader = Serial.parseInt();
      delay(2);
    }
  } 
   
  return  reader;
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
