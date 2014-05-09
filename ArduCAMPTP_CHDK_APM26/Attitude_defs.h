#ifndef __ATTITUDE_DEFS_H__
#define __ATTITUDE_DEFS_H__

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
