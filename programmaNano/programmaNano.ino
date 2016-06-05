#include <Servo.h>

unsigned long previousMillis = 0;              
bool stippenlijn = false;
bool boven= false;
bool beneden = true;
const long interval = 20;    

bool validDistance = false;    
long afstandwaarde = 0;   
int count = 0;
int afstand = 0;

#define echoPin 5          // Echo Pin
#define trigPin 4          // Trigger Pin
#define timeOutPin 12       //De pin om de sensor aan en uit te doen

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

int rechtseAfstand  = -1;
int linkseAfstand = -1;
bool schuinKijken = false;

bool rechtsDraaien = false;


Servo myservo;

void setup(){
  myservo.attach(3);  // attaches the servo on pin 11 to the servo object
  Serial.begin(9600);


  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(timeOutPin, OUTPUT);
  pinMode(coila1, OUTPUT);
  pinMode(coila2, OUTPUT);
  pinMode(coilb1, OUTPUT);
  pinMode(coilb2, OUTPUT);
  pinMode(enable, OUTPUT);
  myservo.write(80);
}

void loop(){

  unsigned long currentMillis = millis();

 
  


  if(beneden){
    draainaarbeneden = 10 - middenpuntpositie;    // zorgt ervoor dat de pen naar beneden gaat 
    beneden=false;
  }

  if(stippenlijn && !beneden){                      // zorgt ervoor dat er een stippelijn patroon getekend wordt
    if(middenpuntpositie<=0){
       draainaarbeneden = 10 - middenpuntpositie;
    }
    if(middenpuntpositie>=10){                      
      draainaarboven = middenpuntpositie;
    }
  }

  
    
     if (currentMillis - previousMillis >= interval) {
         
         //automatischrijdenZigZag();           // functie om automatisch zig zag te rijden te rijden
         automatischrijden();                   // functie om automatisch te rijden

       
            if(draainaarboven>0){               // wordt gebruikt om de stappen motor naar boven te draaien
              digitalWrite(enable, HIGH);        // zorgt dat de H brug aan staat 
        
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
              
            }else if(draainaarbeneden>0){     // zorgt dat de stappen motor naar beneden gaat.
              digitalWrite(enable, HIGH);     // zorgt dat de H brug aan staat 
        
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
              digitalWrite(enable, LOW); // zet de H-brug terug uit
            }
        previousMillis = currentMillis; 
    }

   
}


void automatischrijdenZigZag(){     // de zig zag code 
  afstand = filterDistance();       // de afstand wordt gemeten
  if (afstand == 1){                // als er een object dicht in de buurt is 
    
    Serial.println(11);             //Verstuurd stop
     if (rechtsDraaien == true) {   // als de wagen naar rechts moet draaien
      while(rechtseAfstand <1){     // als er rechts een object is, blijf dan naar rechts kijken
        rechtseAfstand = RechtsKijken90();    //// zorgt dat de sensor naar rechts kijkt
      }    
      if(rechtseAfstand >1){            // als er rechts geen object meer is
         Serial.println(3);             // Verstuurd om rechts te draaien

          schuinKijken = false;        
          myservo.write(80);            // zet servo terug in het midden
          afstand=0;
         delay(4000);                   // wacht dat de wagen gedraaid 
         beneden = true; 
         stippenlijn = false;           // geen stippenlijn meer tekenen maar een lijn
         rechtsDraaien = false;
        }  
      }
      else if(rechtsDraaien == false){    // als de wagen naar links moet draaien
       while(linkseAfstand <1){           // als er links nog een object is blijven kijken naar links
        linkseAfstand = LinksKijken90();   // kijkt naar links
       }
        if(linkseAfstand >1){           // als er links geen object meer is
          Serial.println(4);        // Verstuurd om links te draaien

          schuinKijken = false; 
          myservo.write(80);      / zet servo terug in het midden
          afstand=0;
          delay(4000);
          stippenlijn = true ;      // zorgt dat er terug een stippellijn patroon wordt getekend
          beneden = false;
          rechtsDraaien = true;

        }  
      }
          rechtseAfstand = -1;
          linkseAfstand = -1;

         
    }  
    else if(afstand ==2){
        
        if (schuinKijken == false){ // als hij nog niet schuin kijkt 
          Serial.println(11); //Verstuurd stop
          schuinKijken = true; 
          
          rechtseAfstand = RechtsKijken20();        // kijkt naar een object schuint rechts        
          linkseAfstand = LinksKijken20();           // kijkt naar een object schuint link    
          
          if( rechtseAfstand <=2 &&  rechtseAfstand > 0|| linkseAfstand <= 2 &&  linkseAfstand > 0){
            if(rechtseAfstand < linkseAfstand){  // als het object rechts dicheter is dan links
              myservo.write(60);            // laat de servo kijken naar schuin rechts

            } 
              
          }
          else{
            myservo.write(80);            // als er links of rechts geen dichte objecten zijn zet servo terug in het midden
          }
          rechtseAfstand = -1;
          linkseAfstand = -1;
        }  
      Serial.println(2); //Verstuurd om traag te rijden
    }  
    else if(afstand ==3) {

      Serial.println(5);//Verstuurd om snel te rijden
    }else{
       Serial.println(11); //verstuurd stop
    }
    
}
void automatischrijden(){
  afstand = filterDistance();
  if (afstand == 1){   // als er een object dicht in de buurt is 
    
    Serial.println(11); //Verstuurd stop

      rechtseAfstand = RechtsKijken90(); // zorgt dat de sensor naar rechts kijkt
      delay(100);
    
      if(rechtseAfstand >1){  // als er rechts niets is draai naar rechts
         Serial.println(3); // Verstuurd om rechts te draaien
         myservo.write(80);
         delay(4000);
         schuinKijken = false;
         afstand=0;
        }else{
          linkseAfstand = LinksKijken90(); // kijkt naar links
          delay(100);  
          if(linkseAfstand >1){   // als er links niets is draai naar links
            Serial.println(4); // Verstuurd om links te draaien
            myservo.write(80); // zet servo terug in het midden
            delay(4000);
            schuinKijken = false;
            afstand=0;
            
           }else{             // als er zowel links als rechts een object is draai 180 graden
               Serial.println(6); // Verstuurd om 180 graden te draaien
               myservo.write(80); // zet servo terug in het midden
               delay(5000);
               schuinKijken = false;
               afstand=0;
               
           }
        }
      
        rechtseAfstand = -1;
        linkseAfstand = -1;
         
    }  
    else if(afstand ==2){ // code om schuin te kijken
        
        if (schuinKijken == false){ 
          Serial.println(11); //Verstuurd stop
          schuinKijken = true; 
          
          rechtseAfstand = RechtsKijken20();                
          linkseAfstand = LinksKijken20(); 
          
          if( rechtseAfstand <=2 &&  rechtseAfstand > 0|| linkseAfstand <= 2 &&  linkseAfstand > 0){
            if(rechtseAfstand < linkseAfstand){
              myservo.write(60);
            }       
          }
          else{
            myservo.write(80);
          }
          rechtseAfstand = -1;
          linkseAfstand = -1;
        }  
      Serial.println(2); //Verstuurd om traag te rijden
    }  
    else if(afstand ==3) {

      Serial.println(5);//Verstuurd om snel te rijden
    }else{
       Serial.println(11); //verstuurd stop
    } 
}

int RechtsKijken90() { // code om rechts te kijken

  myservo.write(0);
  delay(1000);
  return kijken();

}
int LinksKijken90() { // code om links te kijken
  myservo.write(180);
  delay(1000);
  return kijken();
}
int RechtsKijken20() {  // code om schuin rechts te kijken
  myservo.write(60);
  delay(500);
  return kijken();
}
int LinksKijken20() {// code om schuin links te kijken
  myservo.write(100);
  delay(500);
  return kijken();
}
int kijken(){ // code om de afstand van de sensor te weten en de waardes te filteren 
  int tempafstand = 0;
  while(tempafstand == 0){
    tempafstand = filterDistance();
    delay(10);
  }
  return tempafstand;
}



long Distance(){ // meet de afstand van de sensor
 
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

  return distance;
}

int filterDistance() { // functie die de afstand waardes filtert naar makkelijkere waardes.

  afstandwaarde = Distance();
  if(count <= 5 && afstandwaarde == 201 || afstandwaarde == 0) {
    count++;
    afstandwaarde = Distance(); //  haalt de afstand op 
    return 0;
  }else{     // als de waarde niet vals is of 5 keer zijn max range heeft
     count = 0;
    if(afstandwaarde < 15 && afstandwaarde > 0){
       return 1;
    }
     else if(afstandwaarde < 35 && afstandwaarde > 0){
      return 2;
    }
    else{
      return 3;
    }
  } 
}

 // codes om de H-brug van de stappen motor aan te sturen

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


