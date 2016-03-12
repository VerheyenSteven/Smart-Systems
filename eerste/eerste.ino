#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
int pos = 90;    // variable to store the servo position 
int count = 0;

//pinkers
int Lpinker = 4; // Linker pinker
int Rpinker = 2; // Rechter pinker

unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 250; // interval at which to blink (milliseconds)
int RledState = LOW;
int LledState = LOW;

int hoekServo = 35;
int hoekServoRondkijken = 90;

#define echoPin A0 // Echo Pin
#define trigPin A1 // Trigger Pin
#define timeOutPin A5

int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

unsigned long previousMillisServo = 0;  // will store last time LED was updated
const long intervalServo = 1000; // interval at which to blink (milliseconds)
unsigned long previousMillisSensor =0;
const long intervalSensor = 300;
 
bool vooruitRijden = LOW;
bool achteruitRijden= LOW;
bool linksRijden = LOW;
bool rechtsRijden = LOW;
bool rechtsKijken = LOW;
bool vooruitKijken = HIGH;
bool rechtsDraaien = LOW;

int LmotorA= 5;  // Left  motor H bridge, input A
int LmotorB = 6;  // Left  motor H bridge, input B
int RmotorA =9;  // Right motor H bridge, input A
int RmotorB =10;  // Right motor H bridge, input B

//int RCleft = 12;  // Digital input 10
//int RCright = 10;  // Digital input 12

int linksemodus=1;                                             // 0=achterwaards, 1=rem, 2=voorwaards.
int rechtsemodus=1;                                            // 0=achterwaards, 1=rem, 2=voorwaards.

int Mix = 1;     // Set to 1 if L/R and F/R signals from RC need to be mixed
int Leftcenter  = 1500;     // when RC inputs are centered then input should be 1.5mS
int Rightcenter = 1500; // when RC inputs are centered then input should be 1.5mS
int RCdeadband = 35;     // inputs do not have to be perfectly centered to stop motors

int linksePWM;                                                 // PWM waarde voor de linkse motor snelheid/rem.
int rechtsePWM;                                                // PWM waarde voor de rechtse motor snelheid/rem.

int LinkseSnelheid=0;
int RechtseSnelheid=0;
int Snelheid = 1500;
int Stuur = 1500;

int scale=12;  

unsigned long linkseoverbelasting;
unsigned long rechtseoverbelasting;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   7
#define CSN_PIN 8

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
int input[3];
int done;


void setup() {
 
  Serial.begin(9600);


  pinMode(LmotorA,OUTPUT);
  pinMode(RmotorA,OUTPUT);
  pinMode(LmotorB,OUTPUT);
  pinMode(RmotorB,OUTPUT);

  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  
  myservo.attach(3);  // attaches the servo on pin 9 to the servo object 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(timeOutPin, OUTPUT);
  

}

void loop() {
   Snelheid = 1500;
   Stuur = 1500;
   unsigned long currentMillis = millis();
   unsigned long currentMillisServo = millis();
   unsigned long currentMillisSensor = millis();
  
  if (radio.available() )
  {
    vooruitRijden = LOW;
    Serial.println("Het werkt");
      
    // Fetch the data payload
      
      done = radio.read(input, sizeof(input));
      
      Serial.print("Switch:  ");
      Serial.print(input[0]);
      
      Snelheid= (input[1])+1000;
      
      Serial.print(input[1]);
      Serial.print("\n");
      Serial.print("Y-axis: ");
      Serial.println(input[2]);
      Stuur  = (input[2])+1000;
      Serial.print("\n\n"); 
    }
    else {
      if(currentMillisServo - previousMillisServo >= intervalServo) {
       
       previousMillisServo = currentMillisServo;     
       myservo.write(pos);  
       
       if(vooruitKijken)
       {         
         pos += hoekServo;
         if(pos >= 125 || pos <= 55) hoekServo = hoekServo*-1;
       }       
     }
     
   if (currentMillisSensor - previousMillisSensor >= intervalSensor) {
     previousMillisSensor = currentMillisSensor; 
     Distance(); 
     
     if (pos >= 55 && pos <= 125 && distance <= 10 && distance > 0) {
       count += 1;       
       vooruitRijden = LOW;       
       if (count >= 5) rechtsKijken = HIGH;
       
       Serial.println("STOP");
       
      }
       
     else if (pos >= 55 && pos <= 125 && distance > 10 || distance == -1) {
        count = 0;
       vooruitRijden = HIGH;
       //rondKijken = LOW;
       Serial.println("DRIVE");
     }
     }
     }
    
  
  if(vooruitRijden){
         Snelheid = 2000;
         Stuur = 1500;
  }
  
  if(rechtsKijken) {
    vooruitKijken = LOW;
    Serial.println("rechtskijken");
    Serial.println(distance);
    Serial.println(pos);
    pos = 0;
     if ( pos == 0 && distance >= 10 || distance == -1)
     {
       Snelheid = 1500;
       Stuur = 2000; 
       rechtsDraaien = HIGH;
       
       Serial.println("rechtsdraaien"); 
      //  vooruitKijken = HIGH;     
     }   
    /* else 
     {
       pos = 180;
       Serial.println("Linkskijken");
       
       if (pos == 180 && distance >= 20 ||distance == -1)
       {
         Snelheid = 1500;
         Stuur = 1000;
         Serial.println("Linksdraaien");
         vooruitKijken = HIGH;
       }       
       else vooruitKijken = HIGH;
     }*/
  }
  
  if (rechtsDraaien) 
  {
    pos = 90;
    Serial.println("Rechtsdraaien");
    if (pos == 90 && distance >= 30 || distance == -1)
    {
      rechtsKijken = LOW;
      vooruitKijken = HIGH;
    }
  }
    
   
    Pinkers(Stuur, currentMillis);
    //------------------------------------------------------------ Code voor RC inputs. ---------------------------------------------------------


  if (abs(Snelheid-1500)<RCdeadband) Snelheid=1500;           // Als de snelheid input in de speling is dan zet men het op 1500 (standaard waarde voor meeste servo's). 
  if (abs(Stuur-1500)<RCdeadband) Stuur=1500;                 // Als de sturing input in de speling is dan zet men het op 1500 (standaard waarde voor meeste servo's).

  if (Mix==1)                                                 // Mengt de snelheid en de stuur signalen.
  {
    Stuur=Stuur-1500;
    LinkseSnelheid=Snelheid-Stuur;
    RechtseSnelheid=Snelheid+Stuur;
  }
  else                                                        // Individuele stick controller. 
  {
    LinkseSnelheid=Snelheid;
    RechtseSnelheid=Stuur;
  }

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
 
 void Pinkers(int Stuur, unsigned long currentMillis){ 
   
   if (Stuur > 1550){
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
         
   if (Stuur < 1450) {
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


    
