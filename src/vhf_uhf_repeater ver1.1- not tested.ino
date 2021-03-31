///////////////////////////////////////////////////////////////////////////////
// Serial port Menus Demo2
//
// Usage:
// - Compile and load this sketch onto your Arduino board.
// - Keep the USB cable connected while the board is running.
// - In the Arduino programming IDE, go in the "Tools" menu and click on the
//   "Serial Monitor" menu entry.
// A window should appear, and in it a menu will be displayed. Try to type
// the first character of one of the menu entries, followed by enter, in the
// window's text input field.
//
// If there is no clearly legible text shown in the window, set the speed to
// 9600 baud, and set the autoscroll checkbox. Reset the board or reload the
// program.
//
// The goal of this demo is to create 2 menus that call each other. Try to
// bounce around the two menus using the '<' and '>' keys.
// Other teachings include using PROGMEM to store the menu text in FLASH to
// save SRAM memory, and demonstrate menu keys are case insensitive.
///////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "DRA818.h" // uncomment the following line in DRA818.h (#define DRA818_DEBUG)

const float ver = 1.1;

SoftwareSerial *Vdra_serial; // Serial connection to DRA818V
SoftwareSerial *Udra_serial; // Serial connection to DRA818U
DRA818 *dra; 
// the DRA object once instanciated
//  REPETER PARAMETTERs

int   v_TX_pl = 9;         //91.5
int   v_RX_pl = 9;         //91.5
int   u_TX_pl = 9;         //
int   u_RX_pl = 9;         //

const int is_dual_side = 1;
int    gui_menu ;

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

int uhf_state ;
int vhf_state ;


#define vRX      5   // arduino serial RX pin to the DRA818 TX pin
#define vTX      4   // arduino serial TX pin to the DRA818 RX pin

#define uRX      3   // arduino serial RX pin to the DRA818 TX pin
#define uTX      2   // arduino serial TX pin to the DRA818 RX pin



#define SERIALMENU_DISABLE_HEARTBEAT_ON_IDLE true

#include <SerialMenu.hpp>
const SerialMenu& menu = SerialMenu::get();


float vhf_frq  ;
float uhf_frq  ;
float vhf_pl   ;
float uhf_pl   ;







///////////////////////////////////////////////////////////////////////////////
// Main menu
///////////////////////////////////////////////////////////////////////////////

// Forward declarations for the sub-menu referenced before it is defined.
extern const SerialMenuEntry subMenu[];
extern const uint8_t subMenuSize;
void print_setting()
{
  Serial.begin(9600);
  Serial.println(vhf_frq,3);
  Serial.println(uhf_frq,3);
  Serial.println(vhf_pl,3);
  Serial.println(uhf_pl,3);
 // menu.load(mainMenu, mainMenuSize);
//  menu.show();
}
// Define the main menu
const SerialMenuEntry mainMenu[] = {
  {"M - SHOW RADIO SETTING               ", false, 'm', [](){ print_setting(); } },
  {"R - RADIO SETTING",       false, 'r', [](){ menu.load(subMenu, subMenuSize); menu.show();} }
};
constexpr uint8_t mainMenuSize = GET_MENU_SIZE(mainMenu);


///////////////////////////////////////////////////////////////////////////////
// Sub-menu
///////////////////////////////////////////////////////////////////////////////



void config_transceiver() 
{
  Serial.println("initializing trasceivers");  
  Vdra_serial = new SoftwareSerial(vRX, vTX); // Instantiate the Software Serial Object.
  Udra_serial = new SoftwareSerial(uRX, uTX); // Instantiate the Software Serial Object.

  Serial.println("DRA_V PROGGRAMING");
  dra = DRA818::configure(Vdra_serial, DRA818_VHF, vhf_frq, vhf_frq, 4, 8, vhf_pl, vhf_pl, DRA818_12K5, true, true, true, &Serial);
  
   if (!dra)
   {
   Serial.println("\nError configuring DRA818V");
   }else{   Serial.println("VHF SETTING - OK");}


  Serial.println("DRA_U PROGGRAMING");
  dra = DRA818::configure(Udra_serial, DRA818_UHF, uhf_frq, uhf_frq, 4, 8, uhf_pl, uhf_pl, DRA818_12K5, true, true, true, &Serial);
   if (!dra) {
  Serial.println("\nError configuring DRA818U");
   }else{   Serial.println("UHF SETTING - OK");}


   EEPROM.put(0, vhf_frq);
   EEPROM.put(10, uhf_frq);
   EEPROM.put(20, vhf_pl);
   EEPROM.put(30, vhf_pl);
   menu.load(mainMenu, mainMenuSize);
   menu.show();
   return;
}




// Example callback function
void foo() {
    }
  


// For this menu most entries are in PROGMEM Flash, and the last two in SRAM
// which is the normal default
const char subMenuStr0[] PROGMEM = "1- SET VHF FREQ";
const char subMenuStr1[] PROGMEM = "2- SET UHF FREQ";
const char subMenuStr2[] PROGMEM = "3- SET VHP PL";
const char subMenuStr3[] PROGMEM = "4- SET UHF PL";
const char subMenuStr4[] PROGMEM = "5- PROGRAM the NEW SETTING to EPROM";

// Define the sub-menu
// The last two menu entries declare their string directly
const SerialMenuEntry subMenu[] = {
  {subMenuStr0, true, '1', [](){ vhf_frq = menu.getNumber<float>("Input VHF freq: "); }},
  {subMenuStr1, true, '2', [](){ uhf_frq = menu.getNumber<float>("Input UHF freq: "); }},
  {subMenuStr2, true, '3', [](){ vhf_pl  = menu.getNumber<float>("Input VHF PL: "); }},
  {subMenuStr3, true, '4', [](){ uhf_pl  = menu.getNumber<float>("Input UHF PL: "); }},
  {subMenuStr4, true, '5', config_transceiver},
  {"6 - BACK",  false, '6',[](){ menu.load(mainMenu, mainMenuSize);
          menu.show(); } }
};
constexpr uint8_t subMenuSize = GET_MENU_SIZE(subMenu);

///////////////////////////////////////////////////////////////////////////////
// Main program
///////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  Serial.println("VHF/UHF REPEATER");  // prints hit cnt
  Serial.println("REPEATER OWNER = EITAN BROGARDI -4X4EB");
  EEPROM.get(0,  vhf_frq);
  EEPROM.get(10, uhf_frq); 
  EEPROM.get(20, vhf_pl);
  EEPROM.get(30, uhf_pl);


  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(vhf_ptt,OUTPUT);
  pinMode(uhf_ptt,OUTPUT);
  pinMode(vhf_pd, OUTPUT);
  pinMode(uhf_pd, OUTPUT);
  pinMode(high_low, INPUT);

  
  pinMode(Vhf_sq, INPUT_PULLUP);
  pinMode(uhf_sq, INPUT_PULLUP);

  pinMode(dtmf_le, INPUT);
//  pinMode(dtmf0, INPUT);
  pinMode(dtmf1, INPUT);
  pinMode(dtmf2, INPUT);
  pinMode(dtmf3, INPUT);


  
  while (!Serial){};
  //Serial.println(DEMOCOPYRIGHT);

  // Set the main menu as the current active menu
  menu.load(mainMenu, mainMenuSize);

  // Display the current menu
  menu.show();
}

constexpr uint8_t loopDelayMs = 100;

void loop() {

  // Run the current menu monitoring and callbacks
  menu.run(loopDelayMs);

  // put your main code here
   // READ REPEATER STATE //
    int uhf_state = digitalRead(uhf_sq);   
    int vhf_state = digitalRead(Vhf_sq);

    // VHF IS RECEIVING // 
    if ((vhf_state == 0 )&& (is_dual_side ==1)){
        digitalWrite(led1, HIGH);          // turn the LED1 on (HIGH is the voltage level)
        digitalWrite(uhf_ptt, LOW);       // KEY THE UHF PTT
        Serial.println("\VHF PTT KEYED");// PRINT TO THE SCREEN
    }
    else
    {
        digitalWrite(led1, LOW);        // turn the LED OFF (HIGH is the voltage level)
        digitalWrite(uhf_ptt, HIGH);   // RELEASE UHF PTT
    }
    // UHF IS RECEIVING // 
    if (uhf_state == 0){
        digitalWrite(led2, HIGH);         // turn the LED2 on (HIGH is the voltage level)
        digitalWrite(vhf_ptt, LOW);      // KEY THE VHF PTT
        Serial.println("UHF PTT KEYED");// PRINT TO THE SCREEN
    }
    else
    {
        digitalWrite(led2, LOW);   // turn the LED OFF (HIGH is the voltage level)
        digitalWrite(vhf_ptt, HIGH);   // RELEASE VHF PTT
    }
  delay(loopDelayMs);
}
