int RadioZekerUit=0;

char reader = 0;

unsigned long previousMillis = 0;         // will store last time LED was updated
const long interval = 250;                // interval at which to blink (milliseconds)
int RledState = LOW;                      //de status van de rechter led
int LledState = LOW;                      //de status van de linker led

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

unsigned long currentMillis;

bool myserialuit = false;

