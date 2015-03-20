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

