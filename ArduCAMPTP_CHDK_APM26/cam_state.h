#ifndef __CAM_STATE_H__
#define __CAM_STATE_H__

#include "Arduino.h"

class CamState
{
 public:
 CamState(void);
 uint8_t target_system; //< System ID
 uint8_t target_component; //< Component ID
 uint8_t session; //< 0: stop, 1: start or keep it up Session control e.g. show/hide lens
 uint8_t zoom_pos; //< 1 to N Zoom's absolute position (0 means ignore)
 uint8_t focus_lock; //< 0: unlock focus and keep unlocked, 1: lock focus or reset the focus and keep locked
 uint8_t shot; //< 0: ignore, 1: shot or start filming
 char *lua_script[100]; //< Custom Lua script call. String with up to 100 bytes.
 uint8_t command_id; //< Command Identity (incremental loop: 0 to 255)//A command sent multiple times will be executed or pooled just once
 uint8_t extra_param; //< Extra parameters enumeration (0 means ignore)
 float extra_value; //< Correspondent value to given extra_param
};

#endif //#defined CAM_STATE_H
