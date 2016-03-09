/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
int input[3];
int done;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  Serial.println("Nrf24L01 Receiver Starting");
  
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{  
  if ( radio.available() )
  {
    Serial.println("Het werkt");
      
    // Fetch the data payload
      
      done = radio.read(input, sizeof(input));
      
      Serial.print("Switch:  ");
      Serial.print(input[0]);
      Serial.print("\n");
      Serial.print("X-axis: ");
      Serial.print(input[1]);
      Serial.print("\n");
      Serial.print("Y-axis: ");
      Serial.println(input[2]);
      Serial.print("\n\n");      
  } //end if radio available
  delay(500);
}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

//NONE
//*********( THE END )***********

