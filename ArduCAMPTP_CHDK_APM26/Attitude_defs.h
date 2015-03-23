#ifndef __ATTITUDE_DEFS_H__
#define __ATTITUDE_DEFS_H__

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
as published by the Free Software Foundation; either version 2+
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdarg.h>

//IMU
static int16_t drone_pitch = 0;
static int16_t drone_roll = 0;
static int16_t drone_yaw = 0;

//HUD
static float drone_heading = 0.0;
static float drone_alt_asl = 0.0;

//GPS
static float drone_lat = 0.0;
static float drone_lon = 0.0;
static uint8_t drone_fix_type = 0.0;
static uint8_t drone_satellites_visible = 0;

//FC IN CHANNELS
static int16_t drone_chan[4] = {0,0,0,0};//IN5, IN6, IN7, and IN8

//Mapped RC IN are compared to Previous State Variables
//Won't be needed when DIGICAM messages are implemented
//because those messages are not sent in a loop like RC IN.
static int16_t prev_lenses_in = 0;
static int16_t prev_zoom_in = 0;
static int16_t prev_flock_in = 0;

//FC OUT CHANNELS
static int16_t drone_servo[2] = {0,0};//RC7 and RC8

/*
char* geoData(float lat, float lon, float alt_asl, int16_t pitch, int16_t roll, float heading){
  char _buffer[70];
  _buffer[0] = '\0';
  const char* form = "'%s','%s','%s','%i','%i','%s'";
  
  char c_lat[11];
  char c_lon[11];
  char c_alt[9];
  char c_heading[8];

  dtostrf(lat,1,6,c_lat);
  dtostrf(lon,1,6,c_lon);
  dtostrf(alt_asl,1,2,c_alt);
  dtostrf(heading,7,2,c_heading);
  sprintf(_buffer, form, c_lat, c_lon, c_alt, pitch, roll, c_heading);
  
  return (char*)_buffer;
  //Example: '50.167958','-97.133185','700.00','360.00','360.00','360.00'
}
*/

#endif  // !defined ATTITUDE_DEFS_H
