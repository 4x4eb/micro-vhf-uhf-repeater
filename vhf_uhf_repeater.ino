
// by Eitan Brogardi
// DATE 7/3/21
// ver num      what changed                            DATE
// ver = 1.0    firstversion               7/3/21 

#include <stdio.h>
#include <SoftwareSerial.h>
#include "DRA818.h" // uncomment the following line in DRA818.h (#define DRA818_DEBUG)


SoftwareSerial *Vdra_serial; // Serial connection to DRA818V
SoftwareSerial *Udra_serial; // Serial connection to DRA818U
DRA818 *dra;                // the DRA object once instanciated

const float ver = 1.0;


//------freq + pl variable-------------//
float v_freq = 145.200;
float u_freq = 439.200;
int   vpl=9;
int   upl=9;
const int is_dual_side = 1;


//------pin numbers-------------//
const int  vhf_ptt = 6;
const int  uhf_ptt = 7;
const int  vhf_pd  = 8;
const int  uhf_pd  = 9;
const int  Vhf_sq  = A0;
const int  uhf_sq  = A1;
const int  high_low  = 10;
const int  dtmf_le  = 13;
const int  dtmf0  = A7;
const int  dtmf1  = A6;
const int  dtmf2  = A5;
const int  dtmf3  = A4;
const int  led1  = 11;
const int  led2  = 12;
#define vRX      5   // arduino serial RX pin to the DRA818 TX pin
#define vTX      4   // arduino serial TX pin to the DRA818 RX pin

#define uRX      3   // arduino serial RX pin to the DRA818 TX pin
#define uTX      2   // arduino serial TX pin to the DRA818 RX pin

//------other variables-------------//
int uhf_state ;
int vhf_state ;




//--------------------------------------//
//                 SETUP
//--------------------------------------//

void setup() {
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(vhf_ptt,OUTPUT);
  pinMode(uhf_ptt,OUTPUT);
  pinMode(vhf_pd, OUTPUT);
  pinMode(uhf_pd, OUTPUT);
  pinMode(high_low, OUTPUT);

  
  pinMode(Vhf_sq, INPUT_PULLUP);
  pinMode(uhf_sq, INPUT_PULLUP);

  pinMode(dtmf_le, INPUT);
  //pinMode(dtmf0, INPUT);
  pinMode(dtmf1, INPUT);
  pinMode(dtmf2, INPUT);
  pinMode(dtmf3, INPUT);
//***********************************************************************
  Serial.begin(9600); // open the serial port at 9600 bps:
  Serial.println("VHF/UHF REPEATER");  // prints hit cnt
  Serial.println("4X4EB");  // prints hit cnt
  Serial.print("VER = ");  // 
  Serial.println(ver);  // 

  digitalWrite(vhf_pd, HIGH);   // 
  digitalWrite(uhf_pd, HIGH);   //
  digitalWrite(uhf_ptt, HIGH);   // 
  digitalWrite(vhf_ptt, HIGH);   //
  digitalWrite(high_low, LOW);   //
 
  digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay (500);    
  digitalWrite(led1, LOW);   // turn the LED on (HIGH is the voltage level)
    delay (500);    
  digitalWrite(led2, LOW);   // turn the LED on (HIGH is the voltage level)

config_radio();

  
}
//------------------------------------------------------------------------------------
//                                         LOOP
//------------------------------------------------------------------------------------

void loop() {

    // READ REPEATER STATE //
    int uhf_state = digitalRead(uhf_sq);   
    int vhf_state = digitalRead(Vhf_sq);

    // VHF IS RECEIVING // 
    if ((vhf_state == 0 )&& (is_dual_side ==1)){
        digitalWrite(led1, HIGH);          // turn the LED1 on (HIGH is the voltage level)
        digitalWrite(uhf_ptt, LOW);       // KEY THE UHF PTT
       // Serial.println("\VHF PTT KEYED");// PRINT TO THE SCREEN
    }
    else
    {
       // delay(300);
        digitalWrite(led1, LOW);        // turn the LED OFF (HIGH is the voltage level)
        digitalWrite(uhf_ptt, HIGH);   // RELEASE UHF PTT
    }
    // UHF IS RECEIVING // 
    if (uhf_state == 0){
        digitalWrite(led2, HIGH);         // turn the LED2 on (HIGH is the voltage level)
        digitalWrite(vhf_ptt, LOW);      // KEY THE VHF PTT
     //   Serial.println("UHF PTT KEYED");// PRINT TO THE SCREEN

    }
    else
    {
      //  delay(300);
        digitalWrite(led2, LOW);   // turn the LED OFF (HIGH is the voltage level)
        digitalWrite(vhf_ptt, HIGH);   // RELEASE VHF PTT
    }
}
//------------------------------------------------------------------------------------
//                                         CONFIG RADIOs
//------------------------------------------------------------------------------------
void config_radio()
{
Serial.println("initializing tx rx radios... ");  
  Vdra_serial = new SoftwareSerial(vRX, vTX); // Instantiate the Software Serial Object.
  Udra_serial = new SoftwareSerial(uRX, uTX); // Instantiate the Software Serial Object.

  Serial.println("DRA_V PROGGRAMING");
  dra = DRA818::configure(Vdra_serial, DRA818_VHF, v_freq, v_freq, 1, 8, vpl, vpl, DRA818_12K5, true, true, true, &Serial);  
   if (!dra) 
   {
   Serial.println("\nError while configuring DRA818V");
   }
   else
   {
   Serial.println("vhf_OK");
   }
   

  Serial.println("DRA_U PROGGRAMING");
  dra = DRA818::configure(Udra_serial, DRA818_UHF, u_freq, u_freq, 1, 8, upl, upl, DRA818_12K5, true, true, true, &Serial);
   if (!dra)
   {
   Serial.println("\nError while configuring DRA818U");
   }
   else
   {
   Serial.println("Uhf_OK");
   }
}
//------------------------------------------------------------------------------------
