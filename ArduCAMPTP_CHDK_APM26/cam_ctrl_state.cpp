#include "cam_ctrl_state.h"

CamCtrlState::CamCtrlState(){
  target_system = 0;//< System ID
  target_component = 0; //< Component ID
  session = -1; //< 0: stop, 1: start or keep it up //Session control e.g. show/hide lens
  zoom_pos = -1; //< 1 to N //Zoom's absolute position (0 means ignore)
  focus_lock = -1; //< 0: unlock focus and keep unlocked, 1: lock focus or reset the focus and keep locked
  shot = 0; //< 0: ignore, 1: shot or start filming
  lua_script[100] = "\0"; //< Custom Lua script call. String with up to 100 bytes.
  command_id = 0; //< Command Identity (incremental loop: 0 to 255)//A command sent multiple times will be executed or pooled just once
  extra_param = 0; //< Extra parameters enumeration (0 means ignore)
  extra_value = 0; //< Correspondent value to given extra_param
}

