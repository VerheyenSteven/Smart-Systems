Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
int pos = 90;                               // variable to store the servo position 
int count = 0; 

unsigned long previousMillis = 0;         // will store last time LED was updated
const long interval = 250;                // interval at which to blink (milliseconds)
int RledState = LOW;                      //de status van de rechter led
int LledState = LOW;                      //de status van de linker led

int hoekServo = 35;                       // de hoek die de servo steeds opschuift
int hoekServoRondkijken = 90;             //de maximale hoek van de servo

int maximumRange = 200;                   // Maximum range needed
int minimumRange = 0;                     // Minimum range needed
long duration, distance;                  // Duration used to calculate distance

unsigned long previousMillisServo = 0;    // slaagt de laatste tijd op waar servo geupdate was
const long intervalServo = 1000;          // interval van de servo
unsigned long previousMillisSensor =0;    // slaagt de laatste tijd op waar sensor geupdate was
const long intervalSensor = 300;          //interval sensor

bool vooruitRijden = LOW;                 // de bools die worden aangepast van de RF, hierdoor zal de auto rijden of andere manuevers maken
bool achteruitRijden= LOW;
bool linksRijden = LOW;
bool rechtsRijden = LOW;
bool rechtsKijken = LOW;
bool vooruitKijken = HIGH;
bool rechtsDraaien = LOW;

int linksemodus=1;                         // 0=achterwaards, 1=rem, 2=voorwaards.
int rechtsemodus=1;                        // 0=achterwaards, 1=rem, 2=voorwaards.

int Leftcenter  = 1500;                    // when RC inputs are centered then input should be 1.5mS
int Rightcenter = 1500;                    // when RC inputs are centered then input should be 1.5mS
int RCdeadband = 35;                       // inputs do not have to be perfectly centered to stop motors (speling)

int linksePWM;                             // PWM waarde voor de linkse motor snelheid/rem.
int rechtsePWM;                            // PWM waarde voor de rechtse motor snelheid/rem.

int LinkseSnelheid=0;                      // de snelheid van de linkse motor
int RechtseSnelheid=0;                     // de snelheid van de rechtse motor
int Snelheid = 1500;                       // de snelheid van het wagentje, Snelheid > 1500 is vooruit, Snelheid < 1500 is achteruit, snelheid = 1500 -> stil
int Stuur = 1500;                          // de richting van het wagenteje Stuur = 1500 -> stil, Stuur > 1500 -> naar rechts, Stuur < 1500 -> naar links

int scale=12;                              // wordt gebruikt om de 1000-2000uS naar 0-255 te verschalen.

                                           // NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL;      // Define the transmit pipe

/*-----( Declaratie Variables voor de RF module)-----*/
int input[3];
int done;


