
#include "IOpins.h"
#include "Constants.h"
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

void setup() {

  Serial.begin(9600);              // begin met de serieele communicatie
   
  pinMode(LmotorA,OUTPUT);        // zet de pinnen voor de H brug op output
  pinMode(RmotorA,OUTPUT);
  pinMode(LmotorB,OUTPUT);
  pinMode(RmotorB,OUTPUT);

  pinMode(Achteruit, OUTPUT);     //pinnen voor de mp3 trigger
  pinMode(Roses, OUTPUT);
  pinMode(Vooruit, OUTPUT);


  radio.begin();                  // voor de RF module te openen
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}

void loop() {
   Snelheid = 1500;             // zet snelheid in de center positie
   Stuur = 1500;                // zet stuur in de center positie
   currentMillis = millis();    // wordt gebruikt voor verschillende time-outs

   


 if (radio.available() )       // kijkt of er een RF signaal is
  {
    RadioZekerUit = 0;         //Zorgt ervoor dat je nog in de radio if blijft
    if(!myserialuit){          //zet de serial uit, voor extra performantie
      Serial.end();
      myserialuit = true;
    }
   
    // Fetch the data payload
      
      done = radio.read(input, sizeof(input));        
      
      Snelheid= (input[1])+1000;     // geeft de geontvangde data door aan snelheid.

      Stuur  = (input[2])+1000;     // geeft de geontvangde data door aan stuur.
      Rij();                        //roept de rij methode aan om de motors aan te sturen
     
      
    }
  else if(RadioZekerUit>3000) {   //als er geen radio communicatie is ga erin
    
    
      if(myserialuit){           //als de seriele communicatie uit staat zet deze aan
        Serial.begin(9600);       
        myserialuit = false;
      }
  
      if (Serial.available() > 1) {  //leest de seriele communicatie uit
        reader = Serial.parseInt();    // zet de seriele data om in een int
        delay(2); 
      }
  
       Automatichrijden();         //al de methodes die kunnen worden aangeroepen door de nano

  }else if(RadioZekerUit<3002){    // zorgt ervoor dat men checkt of er geen radio singalen meer aankomen
       RadioZekerUit ++;
    }
}



//*-------------------------------------Methodes automatisch rijden----------------------------------------*/
void Automatichrijden(){
    if(reader == 11){             //als er een 11 wordt doorgestuurd stop de wagen
      Stoop(); 
    }else if(reader == 2){        //als er een 2 wordt doorgestuurd laat de wagen traag rijden
      Snelheid = 1650;
      Stuur = 1500;
      Rij();  
    }else if(reader == 5){        //als er een 5 wordt doorgestuurd laat de wagen snel rijden
      Snelheid = 1800;
      Stuur = 1500;
      Rij();  
    }else if(reader == 3){        //als er een 3 wordt doorgestuurd laat de wagen 90 graden naar rechts draaien
      RechtsDraaien90();
    }
    else if(reader == 4){         //als er een 4 wordt doorgestuurd laat de wagen naar links draaien
      LinksDraaien90();
    }else if(reader == 6){        //als er een 6 wordt doorgestuurd draai de wagen volledig rond zijn as
      Draaien180();
    }else{
      Stoop();                    // als er een onbekende waarde wordt doorgestuurd stop de wagen
    }
}

void RechtsDraaien90(){        //methode om rechts te draaien
    Snelheid = 1500;
    Stuur = 2023;
    Rij();
    delay(2000);
    Stoop(); 
    reader=0; 
    delay(1500);
}

void Draaien180(){            //methode om rond zijn as te draaien
    Snelheid = 1500;
    Stuur = 2023;
    Rij();
    delay(3500);
    Stoop(); 
    reader=0; 
    delay(1500);
}

void LinksDraaien90(){       //methode om links te draaien

    Snelheid = 1500;
    Stuur = 1023;
    Rij();
    delay(2000);
    Stoop(); 
    reader=0;  
    delay(1500);
}



void Stoop(){              //methode om de wagen te laten stoppen
  Snelheid = 1500;
  Stuur = 1500;
  Rij();
  Stuur = 1500;

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
