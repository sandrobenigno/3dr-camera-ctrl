#ifndef __DEFINES__
#define __DEFINES__

/*
ArduCam PTP CHDK - Arduino based Remote CHDK Camera Control.
Copyright (c) 2011.  All rights reserved.
This firmware is part of the ArduCAM OSD Project: an open source Arduino based OSD and Camera Control.

Author(s): Sandro Benigno
                  Legacy PTP commands from Canon's PS-ReCSDK 1.1.0e
                  Custom extended PTP commands from CHDK's PTP implementation
                  USB host and PTP library from Oleg Mazurov - circuitsathome.com
                  
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

//uncoment if using Mega or ADK boards
//#define MEGABOARD 1

#define AUTOSTART 0 //Expose lens on start (Startr on Record Mode)
#define TELEMETRY_SPEED 57600
#define MAX_ZOOM_STEPS 10

#ifdef MEGABOARD
  //1280 or 2560 Boards
  #define MAVLINK_COMM_NUM_BUFFERS 2
  #define MAVLINK_CHANNEL MAVLINK_COMM_1
  #define Serialx Serial1
#else
  //328P Boards
  #define MAVLINK_COMM_NUM_BUFFERS 1
  #define MAVLINK_CHANNEL MAVLINK_COMM_0
  #define Serialx Serial
#endif

//Define mapping of FC IN channel to coltroll each function
//Pay attention to the RC IN chosen as Flight Control Mode
#define DRONE_LENSES 0 //RC IN from 5 to 8 (0,1,2,3)(OFF/ON)
#define DRONE_FLOCK 2 //RC IN from 5 to 8 (0,1,2,3)(OFF/ON)
#define DRONE_ZOOM 3 //RC IN from 5 to 8 (0,1,2,3)(Range from 1000 to 2000 as 0 to 100%)

//Define what FC OUT channel is the trigger
//#define DRONE_OUT 0 //Shutter RC7
#define DRONE_OUT 1 //Shutter RC8

//*Uncomento to Enable PTP Scripts being executed
//#define PTPREPORT 1

//Uncomment to Enable Test pics
//#define TESTPIC 1

//*Uncomment to see Attitude plus the monitored drone's IN/OUT Data
//*Do ***NOT*** enable that on 328p boards ("Capture Timer" have to be disabled for testing it on 328p)
//#define MAVREPORT 1

//Uncomment to Enable ISR Triggering on Pin D3 (PORTD3 INT0)
//#define ISR_PIN_D3

//Uncomment to Enable ISR Triggering on Pin D8 (PORTB0 PCINT0
//#define ISR_PIN_D8

#endif //!defined DEFINES

