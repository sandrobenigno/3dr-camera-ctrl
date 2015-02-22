/*
ArduCam PTP CHDK - Arduino based Remote CHDK Camera Control.
Copyright (c) 2011.  All rights reserved.
This firmware is part of the ArduCAM OSD Project: an open source Arduino based OSD and Camera Control.

Author(s): Sandro Benigno
                  Legacy PTP commands from Canon's PS-ReCSDK 1.1.0e
                  Custom extended PTP commands from CHDK's PTP implementation
                  USB host and PTP library from Oleg Mazurov - circuitsathome.com
                  
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.  You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <FastSerial.h>
#include "arducam_chdk_ptp.h"

void PSStateHandlersCHDK::OnSessionOpenedState(PTP *ptp)
{	
	if(getAutoStart()){
            if (FAILED(((ArduCAM_PTP*)ptp)->InitCHDK(true)) )
		PTPTRACE("Initialization error\r\n");
	}
	ptp->SetState(PTP_STATE_DEVICE_INITIALIZED);
}

void PSStateHandlersCHDK::setAutoStart(bool _val){_autostart = _val;}
bool PSStateHandlersCHDK::getAutoStart(){return _autostart;}

ArduCAM_PTP::ArduCAM_PTP(USB *pusb, PSStateHandlersCHDK *s, bool astart)
: CanonPS(pusb, s)
{
  s->setAutoStart(astart);
}

//Event Parser
uint16_t ArduCAM_PTP::EventCheck(PTPReadParser *parser)
{
  uint16_t        ptp_error       = PTP_RC_GeneralError;
  OperFlags       flags           = { 0, 0, 0, 1, 1, 0 };
  
  if ( (ptp_error = Transaction(PS_OC_CheckEvent, &flags, NULL, parser)) != PTP_RC_OK)
            PTPTRACE2("EventCheck error: ", ptp_error);
  
  return ptp_error;
}

//Extending CanonPS class with CHDK initialization
uint16_t ArduCAM_PTP::InitCHDK(bool binit)
{
  uint16_t ptp_error = PTP_RC_GeneralError;
  if (binit){
    if ((ptp_error = ExecScriptCHDK("switch_mode_usb(1);")) != PTP_RC_OK)
            PTPTRACE2("StartShootingMode failed: ", ptp_error);
  }
  else{
    if ((ptp_error = ExecScriptCHDK("switch_mode_usb(0);")) != PTP_RC_OK)
            PTPTRACE2("EndShootingMode failed: ", ptp_error);
  }
  return ptp_error;
}

//Extending CanonPS class with CHDK Scripts Execution
uint16_t ArduCAM_PTP::ExecScriptCHDK(char* script)
{
  uint16_t ptp_error = PTP_RC_GeneralError;
  OperFlags flags = { 2, 2, 1, 1, 3, 0};
  uint32_t params[2];
  
  params[0] = PTP_CHDK_ExecuteScript; //from CHDK's PTP header
  params[1] = PTP_CHDK_SL_LUA; //from CHDK's PTP header
  flags.dataSize = (uint16_t)(strlen(script) + 1); //mandatory on CHDK ptp transactions
  /*
  Serial.print("Script length: ");
  Serial.println((uint32_t)(strlen(script) + 1));
  Serial.print("Flag dataSize: ");
  Serial.println(flags.dataSize);
  */
  if ( (ptp_error = Transaction(PTP_OC_CHDK, &flags, params, (char*)script)) != PTP_RC_OK)
            PTPTRACE2("Executing CHDK script error:", ptp_error);
#ifdef PTPREPORT
  Serial.print("Received: ");
  Serial.println(script);
#endif
  return ptp_error;
}
