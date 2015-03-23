#ifndef __MAVCOMM_H__
#define __MAVCOMM_H__

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

//#include "Arduino.h"

#define ToRad(x) radians(x)	// *pi/180
#define ToDeg(x) degrees(x)	// *180/pi

static uint8_t      apm_mav_system; 
static uint8_t      apm_mav_component;

/* The default UART header for CAM Control MavBeats*/ 

uint8_t sysid = 66;                   //< ID 66 for this airplane
uint8_t compid = MAV_COMP_ID_CAMERA;  //< The component sending the message is the Camera Control
uint8_t type = MAV_TYPE_GENERIC;   //< This system is generic

// Define the system type, in this case not an autopilot
uint8_t system_type = MAV_TYPE_ENUM_END;
uint8_t autopilot_type = MAV_AUTOPILOT_ENUM_END; //< I isn't an autopilot
uint8_t system_mode = MAV_MODE_ENUM_END; //< I isn't an autopilot
uint32_t custom_mode = 0;                 //< Custom mode, can be defined by user/adopter
uint8_t system_state = MAV_STATE_ACTIVE; //< System active... probably MAV_STATE_ENUM_END is better
// Initialize the required buffers

void do_mav_beat(){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
 
  // Pack the message
  mavlink_msg_heartbeat_pack(sysid,compid, &msg, type, autopilot_type, system_mode, custom_mode, system_state);
 
  // Copy the message to the send buffer
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
 
  // Send the message with the standard UART
  Serial.write(buf, len);
}

void comm_receive() {

  mavlink_message_t msg;
  mavlink_status_t status;
  
  // COMMUNICATION THROUGH EXTERNAL UART PORT (XBee serial)
  while(Serialx.available() > 0 )
  {
    uint8_t c = Serialx.read();
    // Try to get a new message
    if(mavlink_parse_char(MAVLINK_CHANNEL, c, &msg, &status))
    {
      // Handle message
      switch(msg.msgid)
      {
        case MAVLINK_MSG_ID_HEARTBEAT:
        {
          apm_mav_system    = msg.sysid;
          apm_mav_component = msg.compid;
          // E.g. read GCS heartbeat and go into
          // comm lost mode if timer times out
#ifdef MAVREPORT
          Serial.printf_P(PSTR("Beat it!\n\r"));
#endif
        }
        break;

        case MAVLINK_MSG_ID_COMMAND_LONG:
        {
          // decode
          mavlink_command_long_t packet;
          mavlink_msg_command_long_decode(&msg, &packet);

          // idetifying and parssing commands
          switch(packet.command)
          {
            case MAV_CMD_DO_DIGICAM_CONTROL:
            {
              cam_ctrl_state.session     = packet.param1;//session
              cam_ctrl_state.zoom_pos    = packet.param2;//zoom_pos
              cam_ctrl_state.focus_lock  = packet.param4;//focus_lock
              cam_ctrl_state.shot        = packet.param5;//shot
              cam_ctrl_state.command_id  = packet.param6;//command_id
              cam_ctrl_state.extra_param = packet.param7;//lua script
            }
            break; 

            default:
              //Do nothing
            break;
          }//End of parsing commands
        }
        break; //End of --> MAVLINK_MSG_ID_COMMAND_LONG

        case MAVLINK_MSG_ID_ATTITUDE:
        {
          drone_pitch = ToDeg(mavlink_msg_attitude_get_pitch(&msg));
          drone_roll = ToDeg(mavlink_msg_attitude_get_roll(&msg));
          drone_yaw = ToDeg(mavlink_msg_attitude_get_yaw(&msg));
        }
        break;

        case MAVLINK_MSG_ID_GPS_RAW_INT:
        {
          drone_lat = mavlink_msg_gps_raw_int_get_lat(&msg) / 10000000.0f;
          drone_lon = mavlink_msg_gps_raw_int_get_lon(&msg) / 10000000.0f;
          drone_fix_type = mavlink_msg_gps_raw_int_get_fix_type(&msg);
          drone_satellites_visible = mavlink_msg_gps_raw_int_get_satellites_visible(&msg);
        }
        break;

        case MAVLINK_MSG_ID_VFR_HUD:
        {
            drone_heading = mavlink_msg_vfr_hud_get_heading(&msg); // 0..360 deg, 0=north
            drone_alt_asl = mavlink_msg_vfr_hud_get_alt(&msg);
        }
        break;

        case MAVLINK_MSG_ID_DIGICAM_CONTROL: //MAV_CMD_DO_DIGICAM_CONTROL:
        {
          cam_ctrl_state.session     = mavlink_msg_digicam_control_get_session(&msg);
          cam_ctrl_state.zoom_pos    = mavlink_msg_digicam_control_get_zoom_pos(&msg);
          cam_ctrl_state.focus_lock  = mavlink_msg_digicam_control_get_focus_lock(&msg);
          cam_ctrl_state.shot        = mavlink_msg_digicam_control_get_shot(&msg);
          cam_ctrl_state.command_id  = mavlink_msg_digicam_control_get_command_id(&msg);
          cam_ctrl_state.extra_param = mavlink_msg_digicam_control_get_extra_param(&msg);
          cam_ctrl_state.extra_value = mavlink_msg_digicam_control_get_extra_value(&msg);
        }
        break; 

        case MAVLINK_MSG_ID_RC_CHANNELS_RAW: //RC INPUT BYPASSED BY APM:
        {
          drone_chan[0] = mavlink_msg_rc_channels_raw_get_chan5_raw(&msg);
          drone_chan[1] = mavlink_msg_rc_channels_raw_get_chan6_raw(&msg);
          drone_chan[2] = mavlink_msg_rc_channels_raw_get_chan7_raw(&msg);
          drone_chan[3] = mavlink_msg_rc_channels_raw_get_chan8_raw(&msg);
        }
#ifdef DRONE_LENSES
          if(drone_chan[DRONE_LENSES] < prev_lenses_in - 100 || drone_chan[DRONE_LENSES] > prev_lenses_in + 100)//ON/OFF sensing
          {
            //Check for FC's "RC input" channel for focus ctrl
            (drone_chan[DRONE_LENSES] > 1600)?cam_ctrl_state.session = 1:cam_ctrl_state.session = 0;
            prev_lenses_in = drone_chan[DRONE_LENSES];
            Serial.printf_P(PSTR("SESSION: "));
            Serial.println(cam_ctrl_state.session,DEC);
          }
#endif
#ifdef DRONE_FLOCK
          if(drone_chan[DRONE_FLOCK] < prev_flock_in - 100 || drone_chan[DRONE_FLOCK] > prev_flock_in + 100){//ON/OFF sensing
            (drone_chan[DRONE_FLOCK] > 1600)?cam_ctrl_state.focus_lock = 1:cam_ctrl_state.focus_lock = 0;
            prev_flock_in = drone_chan[DRONE_FLOCK];
          }
#endif
#ifdef DRONE_ZOOM
          if(drone_chan[DRONE_ZOOM] < prev_zoom_in - 20 || drone_chan[DRONE_ZOOM] > prev_zoom_in + 20){//1000-2000 sensing
            int16_t zooming = constrain(drone_chan[DRONE_ZOOM],1000,2000);
            cam_ctrl_state.zoom_pos = (int8_t)(map(zooming,1000,2000,0,MAX_ZOOM_STEPS));
            prev_zoom_in = drone_chan[DRONE_ZOOM];
          }
#endif
        break; 

        case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW: //FLYGHT CONTROL OUTPUT:
        {
          drone_servo[0] = mavlink_msg_servo_output_raw_get_servo7_raw(&msg);
          drone_servo[1] = mavlink_msg_servo_output_raw_get_servo8_raw(&msg);
#ifdef DRONE_OUT
          if(cam_ctrl_state.shot <= 0 && (drone_servo[DRONE_OUT] > 1500)) cam_ctrl_state.shot = 1;
#endif
        }
        break; 

        default:
          //Do nothing
        break;
      }
    }
    delayMicroseconds(138);
      // And get the next one
  }
}

#endif  // !defined MAVCOMM_H

