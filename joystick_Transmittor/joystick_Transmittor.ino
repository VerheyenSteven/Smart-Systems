
 /*-----( Import needed libraries )-----*/
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN 7
#define CSN_PIN 8


const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
int input[3];
int tijdelijk;

int SW_pin = 2; // digital pin connected to switch output  

void setup()   /****** SETUP: RUNS ONCE ******/
{
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
 
  Serial.begin(9600);

  pinMode(0, INPUT);
  pinMode(1, INPUT);
  radio.begin();
  radio.openWritingPipe(pipe);
}
//--(end setup )---

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{ 
  input[0] = digitalRead(SW_pin);
  input[1] = analogRead(0);
  input[2] = analogRead(1);
  
  Serial.println(input[1]);
  Serial.println(input[2]);

  
  //Serial.println(input[0]);
  radio.write(input, sizeof(input));

}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

//NONE
//*********( THE END )***********

