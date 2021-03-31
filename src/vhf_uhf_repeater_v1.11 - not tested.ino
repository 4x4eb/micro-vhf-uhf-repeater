
// by Eitan Brogardi
// DATE 7/3/21
// ver num      what changed                      DATE
// ver = 1.0    first version                     7/3/21 
// ver = 1.1    add start up menu on terminal    9/3/21 


#include <stdio.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "DRA818.h" // uncomment the following line in DRA818.h (#define DRA818_DEBUG)

const float ver = 1.1;

SoftwareSerial *Vdra_serial; // Serial connection to DRA818V
SoftwareSerial *Udra_serial; // Serial connection to DRA818U
DRA818 *dra;                // the DRA object once instanciated
//--------------------------------------------------------------------------------

//  REPETER PARAMETTERs
float v_rx_freq = 145.275;
float v_offset = 0;
float u_rx_freq = 439.500;
float u_offset = 0;

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
int cnt = 0;


#define vRX      5   // arduino serial RX pin to the DRA818 TX pin
#define vTX      4   // arduino serial TX pin to the DRA818 RX pin

#define uRX      3   // arduino serial RX pin to the DRA818 TX pin
#define uTX      2   // arduino serial TX pin to the DRA818 RX pin


void setup() {
   
  v_TX_pl = EEPROM.read(0);
  v_RX_pl = EEPROM.read(1);
  u_TX_pl = EEPROM.read(2);
  u_RX_pl = EEPROM.read(3);


 
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
//***********************************************************************
  Serial.begin(9600); // open the serial port at 9600 bps:
  Serial.println("VHF/UHF REPEATER");  // prints hit cnt
  Serial.println("REPEATER OWNER = EITAN BROGARDI -4X4EB");  // prints hit cnt
  Serial.print("VER = ");  // 
  Serial.println(ver);  // 
  Serial.print("VHF rx FREQ = ");  // 
  Serial.print(v_rx_freq,3);  // 
  Serial.println(" MHz");  // 

  Serial.print("VHF tx FREQ = ");  // 
  Serial.print(v_rx_freq+v_offset,3);  // 
  Serial.println("MHz");  // 

  Serial.print("UHF tx FREQ = ");  // 
  Serial.print(u_rx_freq,3);  // 
  Serial.println(" MHz");  // 
  Serial.print("UHF rx FREQ = ");  // 
  Serial.print(u_rx_freq+u_offset,3);  // 
  Serial.println(" MHz");  // 

  digitalWrite(vhf_pd, HIGH);   // 
  digitalWrite(uhf_pd, HIGH);   //
  digitalWrite(uhf_ptt, HIGH);   // 
  digitalWrite(vhf_ptt, HIGH);   //
  //digitalWrite(high_low, HIGH);   //
 
  digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)
//----------- check if char 0 was pressed this code is to set target address via MONITOR on powerup------------- 
  delay(1500);
  Serial.println("0+ENTER- TO SET REPEATER PARAMETERS GUI ");
  while (cnt < 11)
 {
    delay (500);//wait 5 sec before continue
    cnt++;
     if (Serial.available())
     {
        char a = Serial.read();
        if (a == '0')
        {
            Serial.println("0 pressed");
            set_gui_param();
        }
     } 
 }
  Serial.println("EXIT GUI RUTINE ");
 
  
Serial.println("set trasceivers ...");
delay (200);
config_transceiver();
//delay (500);    
  digitalWrite(led1, LOW);   // turn the LED on (HIGH is the voltage level)
  delay (500);    
  digitalWrite(led2, LOW);   // turn the LED on (HIGH is the voltage level)

Serial.println("REPEATER IS ON AND READY!!!");

}
//------------------------------------------------------------------------------------
void loop() {

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
}

void config_transceiver() 
{
  Serial.println("initializing trasceivers");  
  Vdra_serial = new SoftwareSerial(vRX, vTX); // Instantiate the Software Serial Object.
  Udra_serial = new SoftwareSerial(uRX, uTX); // Instantiate the Software Serial Object.

  Serial.println("DRA_V PROGGRAMING");
  dra = DRA818::configure(Vdra_serial, DRA818_VHF, v_rx_freq, v_rx_freq+v_offset, 4, 8, v_TX_pl, v_RX_pl, DRA818_12K5, true, true, true, &Serial);
  
   if (!dra)
   {
   Serial.println("\nError configuring DRA818V");
   }else{   Serial.println("VHF SETTING - OK");}


  Serial.println("DRA_U PROGGRAMING");
  dra = DRA818::configure(Udra_serial, DRA818_UHF, u_rx_freq, u_rx_freq+u_offset, 4, 8, u_TX_pl, u_RX_pl, DRA818_12K5, true, true, true, &Serial);
   if (!dra) {
  Serial.println("\nError configuring DRA818U");
   }else{   Serial.println("UHF SETTING - OK");}
   return;
}

void set_gui_param() 
{
cnt =0;
          Serial.println("*********** set repeater parameters ********* ");
          Serial.println("choose parameter to set");
                    Serial.println("1-VHF TX FREQ");
                    Serial.println("2-VHF RX FREQ");
                    Serial.println("3-UHF TX FREQ");
                    Serial.println("4-UHF RX FREQ");
                    Serial.println("5-VHF TX PL");
                    Serial.println("6-VHF RX PL");
                    Serial.println("7-VHF TX PL");
                    Serial.println("8-VHF RX PL");
          while (cnt < 31)
          {
           cnt++;
           delay (500);//wait 5 sec before continue
           if (Serial.available())
           {
             gui_menu = Serial.read();
             gui_menu = gui_menu-48;
             if (gui_menu != 10){// dont detect space bar as a valid char
              switch (gui_menu) {
                case 1:
                  Serial.println("SET VHF TX FREQ");
                  break;
                case 2:
                  Serial.println("SET VHF RX FREQ");
                  break;
                case 3:
                  Serial.println("SET UHF TX FREQ");
                  break;

                case 4:
                  Serial.println("SET UHF RX FREQ");
                  break;
                case 5:
                  Serial.println("SET VHF TX PL");
                  print_pl_list();
                  set_v_tx_pl();
                  break;
                case 6:
                  Serial.println("SET VHF RX PL");
                  print_pl_list();
                  set_v_rx_pl();
                  break;
                case 7:
                  Serial.println("SET UHF TX PL");
                  print_pl_list();
                  set_u_tx_pl();
                  break;
                case 8:
                  Serial.println("SET UHF RX PL");
                  print_pl_list();
                  set_u_rx_pl();
                  break;
                  default:
                  break;
              }                

                        }
           } 
          }
      Serial.println("leaving GUI rutine ");
     // t_num = EEPROM.read(0);

}

void print_pl_list()
{
                  Serial.println(" ");
                  Serial.println("CHOOSE PL NUMBER");
                  Serial.println("1=67, 2=71.9, 3=74.4, 4=77, 5=79.7, 6=82.5, 7=85.4, 8=88.5, 9=91.5,10=94.8, 11=97.4");
                  Serial.println("12=100, 13=103.5, 14=107.2, 15=110.9, 16=114.8, 17=118.8, 18=123, 19=127.3, 20=131.8");
                  Serial.println("21=136.5, 22=141.3, 23=146.2, 24=151.4, 25=156.7, 26=162.2, 27=167.9, 28=173.8, 29=179.9");
                  Serial.println("30=186.2, 31=192.8, 32=203.5, 33=210.7, 34=218.1, 35=225.7, 36=233.6 , 37=241.8, 38=250.3");
}

void set_v_tx_pl()
{
 while (cnt < 10)
          {
           cnt++;
           delay (500);//wait 5 sec before continue
           if (Serial.available())
           {
             v_TX_pl = Serial.read();

            }
          } 
}         
          
void set_v_rx_pl()
{
}
void set_u_tx_pl()
{
}
void set_u_rx_pl()
{
}
//------------------------------------------------------------------------------------
