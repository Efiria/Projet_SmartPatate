//****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND



#define SET(x,y) (x |=(1<<y))				//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       		// |
#define CHK(x,y) (x & (1<<y))           		// |
#define TOG(x,y) (x^=(1<<y))            		//-+



#define N 160  //How many frequencies

float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;

 
   
   

void setup()
{
  
  
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin
  pinMode(2,OUTPUT);        // Pin DEL rouge
  pinMode(3,OUTPUT);        // Pin DEL jaune
  pinMode(4,OUTPUT);        // Pin DEL verte

  Serial.begin(115200);
}

void loop()
{
  unsigned int d;

  int counter = 0;
  for(unsigned int d=0;d<N;d++)
  {
    int v=analogRead(0);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=1;                 // |
    OCR1A=1/2;              //-+
    SET(TCCR1B,0);          //-Restart generator

    Serial.println(v);
    delay(500);

    if (v < 460)    // Pleine main
    {
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
      digitalWrite(4,HIGH);
    }
    else if (v < 500)   // 2 doigts
    {
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
    }
    else if (v < 575)   // 1 doigt
    {
      digitalWrite(2,HIGH);
    }
    else    // Sans contact
    {
      digitalWrite(2,LOW);
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
    }
  }


PlottArray(1,freq,results); 
 

  TOG(PORTB,0);            //-Toggle pin 8 after each sweep (good for scope)
}
   
