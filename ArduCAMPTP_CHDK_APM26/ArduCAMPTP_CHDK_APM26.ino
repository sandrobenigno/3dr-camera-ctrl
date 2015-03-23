/*
ArduCam PTP CHDK - Arduino based Remote CHDK Camera Control.
Copyright (c) 2011.  All rights reserved.
This firmware is part of the ArduCAM OSD Project: an open source Arduino based OSD and Camera Control.

Code : ArduCamPTP_CHDK for APM 2.6
Version : v0.97, May 8th 2014

Author(s): Sandro Benigno
                  Legacy PTP commands from Canon's PS-ReCSDK 1.1.0e
                  Custom extended PTP commands from CHDK's PTP implementation
                  USB host and PTP library from Oleg Mazurov - circuitsathome.com

Note: The PTP library used here was modified by the author.
      There are special characteristics needed to cover the scripting approach.
      The CHDK PTP Transaction has a wider RX buffer (reflected as well on the dataSize flag).

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2+
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <FastSerial.h>

#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 

#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

#include <avr/interrupt.h> 
//USB lib
#include <usbhub.h>
//PTP lib
#include <ptp.h>
#include "__defines.h"

boolean usbPlugged = false;

FastSerialPort0(Serial);
#ifdef MEGABOARD
FastSerialPort1(Serial1);
FastSerialPort2(Serial2);
FastSerialPort3(Serial3);
#endif

//MAVLINK and companions
#include "AP_Math_AVR_Compat.h"
#include "../mavlink/include/mavlink.h"
#include "cam_ctrl_state.h"
CamCtrlState cam_ctrl_state;
#include "Attitude_defs.h"
#include "MAVcomm.h"

//PTP + CHDK implem dependences
#include <simpletimer.h>
#include "pseventparser.h"
#include "ptp_parser.h"

//PTP CHDK: extended CanonPS class
#include "arducam_chdk_ptp.h"

class CamStateHandlers : public PSStateHandlersCHDK //New CHDK Parser
{
      enum CamStates { stInitial, stDisconnected, stConnected };
      CamStates stateConnected;
    
public:
      CamStateHandlers() : stateConnected(stInitial) {};
      
      virtual void OnDeviceDisconnectedState(PTP *ptp);
      virtual void OnDeviceInitializedState(PTP *ptp);
      virtual void OnDeviceBusyState(PTP *ptp);
} CamStates;

USB         Usb;
//USBHub      Hub1(&Usb);
ArduCAM_PTP Ps(&Usb, &CamStates, AUTOSTART); //New CHDK PTP

SimpleTimer  mavlinkTimer;
SimpleTimer  captureTimer;

double lastShot = 0;
int16_t minShotInt = 1500;
uint8_t xcount = 0;

void CamStateHandlers::OnDeviceDisconnectedState(PTP *ptp)
{
    if (stateConnected == stConnected || stateConnected == stInitial)
    {
        stateConnected = stDisconnected;
        usbPlugged = false;
        captureTimer.Disable();
        
        E_Notify(PSTR("Camera disconnected\r\n"),0x80);
    }
}

void CamStateHandlers::OnDeviceInitializedState(PTP *ptp)
{
    if (stateConnected == stDisconnected || stateConnected == stInitial)
    {
        stateConnected = stConnected;
        usbPlugged = true;
        E_Notify(PSTR("Camera connected\r\n"),0x80);
        captureTimer.Enable();
    }
}

void CamStateHandlers::OnDeviceBusyState(PTP *ptp)
{
    //E_Notify(PSTR("I'm Busy!\r\n"),0x80); 
}

void setup() 
{
  
  pinMode(6,  OUTPUT); //LED Blue (Status) //Old Board
  digitalWrite(6, HIGH); //unplug OSD CHIP //Old Board
  
  //Initialize PINS
  pinMode(4,  OUTPUT); //LED Blue (Status)
  pinMode(10,  OUTPUT); //USB OUT
  pinMode(7,  OUTPUT); //RESET
  pinMode(8,  INPUT); //APM Trigg
  digitalWrite(10, LOW); //plug USB CHIP
  digitalWrite(7, HIGH); //set max reset as it were tied to reset's pull up

  Serial.begin(TELEMETRY_SPEED);
#ifdef MEGABOARD
  Serial1.begin(TELEMETRY_SPEED);
#endif
  Serial.printf_P(PSTR("Start...\n\r"));

  if (Usb.Init() == -1)
            Serial.println("OSC did not start.");

  captureTimer.Set(&OnCaptureTimer, 200);
  mavlinkTimer.Set(&OnMavlinkTimer, 2000);
  mavlinkTimer.Enable();
  delay( 200 );

#ifdef ISR_PIN_D3
/**** My Old prototype: Trigger Pin D3 (PORTD3 INT0) ****/
  EICRA |= (1<<ISC10)|(1<<ISC11);
  EIMSK |= (1<<INT1);
  sei();//Global Int Enabled
#endif

#ifdef ISR_PIN_D8
/**** The New prototype CCB: Trigger Pin D8 (PORTB0 PCINT0)  ****/
  PCMSK0 |= (1<<PCINT0);//Chosen Pin Set by Mask 
  PCICR  |= (1<<PCIE0);//Int source Set
  sei();//Global Int Enabled
#endif
}

void loop() 
{
  //while(!usbPlugged) Usb.Task();
  Usb.Task();
  comm_receive();
  mavlinkTimer.Run();
  captureTimer.Run();
}

void OnCaptureTimer()
{
#ifdef MEGABOARD
  readSerialCommand(); //Commands sent by USB Serial on MegaBoards for testing
#endif
   //Serial.printf_P(PSTR("On capture timer...\n\r"));
  if(cam_ctrl_state.session == 2){ //Check for running session state
    char cm_buffer[20];
    cm_buffer[0] = '\0';
    if(cam_ctrl_state.shot > 0 && (millis() - lastShot) > minShotInt){ //check for FC's "virtual servo" channel for shooting
       cam_ctrl_state.shot = 0;
       geoPic();
     }
     else if(cam_ctrl_state.zoom_pos > 0){ // check for FC's "RC input" channel for shooting
       sprintf(cm_buffer,"set_zoom(%i);", cam_ctrl_state.zoom_pos);
       Ps.ExecScriptCHDK((char*)cm_buffer);
       delay(2000);
       cam_ctrl_state.zoom_pos = -1;
     }
     else if(cam_ctrl_state.focus_lock >= 0){ // check for FC's "RC input" channel for focus ctrl
       sprintf(cm_buffer,"set_aflock(%i);", cam_ctrl_state.focus_lock);
       Ps.ExecScriptCHDK((char*)cm_buffer);
       delay(1500);
       cam_ctrl_state.focus_lock = -1;
     }
  }
  else if(cam_ctrl_state.session >= 0){ //Check for start session command
   //Serial.printf_P(PSTR("On session step...\n\r"));
    if(cam_ctrl_state.session == 1){ //Need do start
      Ps.InitCHDK(true);
      cam_ctrl_state.session = 2; //Now it's running...
    }
    else{// Need to close
      Ps.InitCHDK(false);
      cam_ctrl_state.session = -1; //Now it's closed...  
    }
    delay(2000);
  }
}

#ifdef MEGABOARD
void readSerialCommand() {
  uint16_t res;
  char queryType;
  char buffer[256] = "";
  buffer[0] = '\0';

  if (Serial.available()) {
    queryType = Serial.read();
    switch (queryType) {

    case 'A': //Activate Capture
      res = Ps.InitCHDK(true);
      Serial.printf("Result: 0x%04x\n\r",res);
      delay(3000);
      break;

    case 'C': //Capture!!!
      res = Ps.ExecScriptCHDK("shoot();");
      Serial.printf("Result: 0x%04x\n\r",res);
      delay(2000);
      break;

    case 'D': //Deactivate Capture
      res = Ps.InitCHDK(false);
      Serial.printf("Result: 0x%04x\n\r",res);
      delay(2000);
      break;

    case 'Z': //Set Zoom 0 - 10 (Example: Z5 or Z10)
      strcat(buffer, "set_zoom(");
      strcat(buffer, readCommandSerial());
      strcat(buffer, ");");
      res = Ps.ExecScriptCHDK((char*)buffer);
      Serial.printf("Result: 0x%04x\n\r",res);
      delay(1000);
      break;

    case '#': //Custom Commands
      strcat(buffer, readCommandSerial());
      res = Ps.ExecScriptCHDK((char*)buffer);
      Serial.printf("Result: 0x%04x\n\r",res);
      delay(2000);
      break;
    }
  }
}

char* readCommandSerial() {
  byte index = 0;
  byte timeout = 0;
  char data[256] = "";

  do {
    if (Serial.available() == 0) {
      delay(10);
      timeout++;
    }
    else {
      data[index] = Serial.read();
      timeout = 0;
      index++;
    }
  }  
  while ((data[constrain(index-1, 0, 256)] != ';') && (timeout < 10) && (index < 256));
  return data;
}
#endif

void OnMavlinkTimer()
{
  
  //Serial.println("On mavlink timer...");
#ifdef MAVREPORT
  Serial.printf("Pitch: %4.0i Roll: %4.0i Heading: %4.2f\n\r", drone_pitch, drone_roll, drone_heading);
  Serial.printf("Lat: %11.6f Lon: %11.6f Alt: %5.2f\n\r", drone_lat, drone_lon, drone_alt_asl);
  Serial.printf("RC5: %4.0i, RC6: %4.0i, RC7: %4.0i, RC8: %4.0i\n\r", drone_chan[0], drone_chan[1], drone_chan[2], drone_chan[3]);
  Serial.printf("CH7: %4.0i, CH8: %4.0i\n\r", drone_servo[0], drone_servo[1]);
#endif

#ifdef TESTPIC
  if(xcount > 2){
    xcount = 0;
    PORTD |= (1 << PORTD4);
    takePic = true;
    Serial.printf_P(PSTR("Pic...\n\r"));
  }
  xcount++;
#endif
}

void geoPic()
{
  //buffers
  char g_buffer[128] = "";
  g_buffer[0] = '\0';
  char c_lat[11];
  char c_lon[11];
  char c_alt[9];
  char c_heading[8];

  //Format
  const char* g_form = "loadfile('A/CHDK/SCRIPTS/geoshot.lua')('%s','%s','%s','%i','%i','%s');";
  
  //Converting Floats
  dtostrf(drone_lat,1,6,c_lat);
  dtostrf(drone_lon,1,6,c_lon);
  dtostrf(drone_alt_asl,1,2,c_alt);
  dtostrf(drone_heading,1,2,c_heading);
  
  sprintf(g_buffer,g_form, c_lat, c_lon, c_alt, drone_pitch, drone_roll, c_heading);
  Ps.ExecScriptCHDK((char*)g_buffer);
  //#loadfile('A/CHDK/SCRIPTS/geoshot.lua')('47.266273','11.302096','1083.22','7','-35','210.00');
  //Ps.ExecScriptCHDK("shoot();");
  digitalWrite(4,  LOW); //LED OFF
  lastShot = millis();
}

#ifdef ISR_PIN_D3
/**** My Old prototype: Trigger Pin D3 (PORTD3 INT0) ****/
ISR (INT1_vect) {
  PORTD |= (1 << PORTD4);
  takePic = true;
}
#endif

#ifdef ISR_PIN_D8
/**** The New prototype CCB: Trigger Pin D8 (PORTB0 PCINT0) ****/
ISR (PCINT0_vect) { 
  if(PORTB & (1 << PORTB0) == (1 << PORTB0)){
    PORTD |= (1 << PORTD4);
    takePic = true;
  }
}
#endif
