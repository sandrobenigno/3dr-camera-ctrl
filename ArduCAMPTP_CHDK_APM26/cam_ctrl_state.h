#ifndef __CAM_CTRL_STATE_H__
#define __CAM_CTRL_STATE_H__

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

#include "Arduino.h"

class CamCtrlState
{
 public:
 CamCtrlState(void);
 uint8_t target_system; //< System ID
 uint8_t target_component; //< Component ID
 int8_t session; //< 0: stop, 1: start or keep it up Session control e.g. show/hide lens
 int8_t zoom_pos; //< 1 to N Zoom's absolute position (0 means ignore)
 int8_t focus_lock; //< 0: unlock focus and keep unlocked, 1: lock focus or reset the focus and keep locked
 uint8_t shot; //< 0: ignore, 1: shot or start filming
 char *lua_script[100]; //< Custom Lua script call. String with up to 100 bytes.
 uint8_t command_id; //< Command Identity (incremental loop: 0 to 255)//A command sent multiple times will be executed or pooled just once
 uint8_t extra_param; //< Extra parameters enumeration (0 means ignore)
 float extra_value; //< Correspondent value to given extra_param
};

#endif //#defined CAM_CTRL_STATE_H

