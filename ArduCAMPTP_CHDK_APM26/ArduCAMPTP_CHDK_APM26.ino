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

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.  You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <FastSerial.h>

#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 

#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

FastSerialPort0(Serial);

#define MAVLINK_COMM_NUM_BUFFERS 1
#define TELEMETRY_SPEED 57600

#include "__defines.h"

#include "AP_Math_AVR_Compat.h"
#include "../mavlink/include/mavlink.h"

#include "cam_state.h"
CamState statecam_ctrl;

#include "Attitude_defs.h"
#include "MAVcomm.h"

#include <inttypes.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h> 

#include <avrpins.h>
#include <max3421e.h>
#include <usbhost.h>
#include <usb_ch9.h>
#include <Usb.h>
#include <usbhub.h>
#include <address.h>

#include <message.h>
#include <parsetools.h>

#include <ptp.h>
#include <ptpdebug.h>
#include <canonps.h>
#include <simpletimer.h>
#include "pseventparser.h"
#include "ptp_parser.h"

#include "arducam_chdk_ptp.h"

boolean usbPlugged = false;
volatile boolean takePic = false;

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
USBHub      Hub1(&Usb);
ArduCAM_PTP Ps(&Usb, &CamStates); //New CHDK PTP

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
   //Serial.printf_P(PSTR("On capture timer...\n\r"));
   if(takePic && (millis() - lastShot) > minShotInt){
     takePic = false;
     geoPic();
   }
}

void OnMavlinkTimer()
{
  //Serial.println("On mavlink timer...");
#ifdef MAVREPORT
  Serial.printf("Pitch: %4.0i Roll: %4.0i Heading: %4.2f\n\r", drone_pitch, drone_roll, drone_heading);
  Serial.printf("Lat: %11.6f Lon: %11.6f Alt: %5.2f\n\r", drone_lat, drone_lon, drone_alt_asl);
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
