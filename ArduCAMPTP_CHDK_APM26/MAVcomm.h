#ifndef __MAVCOMM_H__
#define __MAVCOMM_H__

//#include "Arduino.h"

#define ToRad(x) radians(x)	// *pi/180
#define ToDeg(x) degrees(x)	// *180/pi

//328P Boards
#define MAVLINK_CHANNEL MAVLINK_COMM_0

//1280 or 2560 Boards
//#define MAVLINK_CHANNEL MAVLINK_COMM_1

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
  while(Serial.available() > 0 )
  {
    uint8_t c = Serial.read();
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
          Serial.printf_P(PSTR("Beat it!\n\r"));
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
              statecam_ctrl.session     = packet.param1;//session
              statecam_ctrl.zoom_pos    = packet.param2;//zoom_pos
              statecam_ctrl.focus_lock  = packet.param4;//focus_lock
              statecam_ctrl.shot        = packet.param5;//shot
              statecam_ctrl.command_id  = packet.param6;//command_id
              statecam_ctrl.extra_param = packet.param7;//lua script
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
          statecam_ctrl.session     = mavlink_msg_digicam_control_get_session(&msg);
          statecam_ctrl.zoom_pos    = mavlink_msg_digicam_control_get_zoom_pos(&msg);
          statecam_ctrl.focus_lock  = mavlink_msg_digicam_control_get_focus_lock(&msg);
          statecam_ctrl.shot        = mavlink_msg_digicam_control_get_shot(&msg);
          statecam_ctrl.command_id  = mavlink_msg_digicam_control_get_command_id(&msg);
          statecam_ctrl.extra_param = mavlink_msg_digicam_control_get_extra_param(&msg);
          statecam_ctrl.extra_value = mavlink_msg_digicam_control_get_extra_value(&msg);
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

