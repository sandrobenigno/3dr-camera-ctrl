/*
ArduCam PTP CHDK - Arduino based Remote CHDK Camera Control.
Copyright (c) 2011.  All rights reserved.
This firmware is part of the ArduCAM OSD Project: an open source Arduino based OSD and Camera Control.

Code : ArduCamPTP_CHDK Beta

Author(s): Sandro Benigno
                  Legacy PTP commands from Canon's PS-ReCSDK 1.1.0e
                  Custom extended PTP commands from CHDK's PTP implementation
                  USB host and PTP library from Oleg Mazurov - circuitsathome.com

Note: The PTP library used here was modified by the author.
      There are special characteristics needed to cover the scripting approach.
      The CHDK PTP Transaction has a wider RX buffer (reflected as well on the dataSize flag).

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

#ifndef __ARDUCAM_CHDK_H__
#define __ARDUCAM_CHDK_H__

#include <ptp.h>
#include <canonps.h>
#include "ptp_from_chdk.h"

class PSStateHandlersCHDK : public PTPStateHandlers
{
public:
      virtual void OnSessionOpenedState(PTP *ptp);
};

class ArduCAM_PTP: public CanonPS
{
  public:
  
    ArduCAM_PTP(USB *pusb, PTPStateHandlers *s);
  
    uint16_t Initialize(bool binit);
    uint16_t InitCHDK(bool binit);
    uint16_t ExecScriptCHDK(char *script);
    uint16_t ReadMsgScriptsCHDK();
    uint16_t EventCheck(PTPReadParser *parser);
};

#endif //__ARDUCAM_CHDK_H__