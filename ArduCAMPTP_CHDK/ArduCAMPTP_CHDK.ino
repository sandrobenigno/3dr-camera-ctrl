/*
ArduCam PTP CHDK - Arduino based Remote Camera Control Project.
Copyright (c) 2011.  All rights reserved.

Code : ArduCamPTP_CHDK
Version : v0.95, 03 aug 2013

Author(s): Sandro Benigno
                  Legacy PTP commands from Canon's PS-ReCSDK 1.1.0e
                  Extended PTP commands from CHDK's PTP implementation
                  USB host and PTP library from Oleg Mazurov - circuitsathome.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.  You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <inttypes.h>
#include <avr/pgmspace.h>

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
#include "ptpobjinfoparser.h"

#include "arducam_chdk_ptp.h"


boolean usbPlugged = false;
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

SimpleTimer  eventTimer, captureTimer;

void CamStateHandlers::OnDeviceDisconnectedState(PTP *ptp)
{
    if (stateConnected == stConnected || stateConnected == stInitial)
    {
        stateConnected = stDisconnected;
        usbPlugged = false;
        eventTimer.Disable();
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
        eventTimer.Enable();
        captureTimer.Enable();
    }
}

void CamStateHandlers::OnDeviceBusyState(PTP *ptp)
{
    Serial.println("I'm Busy!"); 
}

void setup() 
{
  //Initialize PINS
  pinMode(6,  OUTPUT); //OSD OUT
  pinMode(10,  OUTPUT); //USB OUT
  pinMode(7,  OUTPUT); //RESET
  digitalWrite(6,  HIGH); //unplug OSD CHIP
  digitalWrite(10, LOW); //plug USB CHIP
  digitalWrite(7, HIGH); //set max reset as it were tied to reset's pull up

  Serial.begin( 115200 );
  Serial.println("Start");

  if (Usb.Init() == -1)
            Serial.println("OSC did not start.");

  eventTimer.Set(&OnEventTimer, 500);
  captureTimer.Set(&OnCaptureTimer, 200);
  delay( 200 );
}

void loop() 
{
  while(!usbPlugged)
    Usb.Task();

  eventTimer.Run();
  captureTimer.Run();
  //readSerialCommand();
  Usb.Task();
}

void OnCaptureTimer()
{
   //Serial.println("On capture timer...");
   readSerialCommand();
}

void OnEventTimer()
{
    //Serial.println("On event timer...");
  /*PSEventParser  prs;
    Ps.EventCheck(&prs);
    
    if (uint32_t handle = prs.GetObjHandle())
    {
                PTPObjInfoParser     inf;
                Ps.GetObjectInfo(handle, &inf);
    }*/
}

void readSerialCommand() {
  char queryType;
  char buffer[128] = "";
  buffer[0] = '\0';

  if (Serial.available()) {
    queryType = Serial.read();
    switch (queryType) {
    case 'A': //Activate Capture
      Ps.InitCHDK(true);
      delay(3000);
      break;
    case 'C': //Capture!!!
      Ps.ExecScriptCHDK("shoot();");
      delay(3000);
      break;
    case 'D': //Deactivate Capture
      Ps.InitCHDK(false);
      delay(2000);
      break;
    case '#': //Custom Commands
      strcat(buffer, readCommandSerial());
      Ps.ExecScriptCHDK((char*)buffer);
      delay(2000);
      break;
    }
  }
}

char* readCommandSerial() {
  byte index = 0;
  byte timeout = 0;
  char data[128] = "";

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
  while ((data[constrain(index-1, 0, 128)] != ';') && (timeout < 5) && (index < 128));
  return data;
}

float readFloatSerial() {
  byte index = 0;
  byte timeout = 0;
  char data[128] = "";

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
  while ((data[constrain(index-1, 0, 128)] != ';') && (timeout < 5) && (index < 128));
  return atof(data);
}

char* join3Strings(char* string1, char* string2, char* string3) {
  char bigstring[128];  // enough room for all strings together
  bigstring[0] = '\0';          // start with a null string:
  strcat (bigstring, string1);   // add first string
  strcat (bigstring, string2);
  strcat (bigstring, string3);

  return bigstring;
}
