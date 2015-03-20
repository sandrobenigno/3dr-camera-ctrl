#include "ptp_parser.h"

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

/*
void PTPObjHandleParser::Parse(const uint16_t len, const uint8_t *pbuf, const uint32_t &offset)
{
  uint16_t cntdn = (uint16_t)len;
  uint32_t *p = (uint32_t*)pbuf;
  for(int i = 0; i<17; i++)
  {
    prevBuf[i] = inBuf[i];
  }

  for(int i = 0; i<16; i++)
  {
    inBuf[i] = p[i];
  }

  inBuf[16] = (uint32_t)cntdn/4;
  varBuffer = inBuf;
  if(inBuf[16] == 0)
  {
    if(inBuf[0] == 0)
    {
      prevBuf[16] = 16;
      varBuffer = prevBuf;
    }
    else 
      varBuffer = inBuf;
  }
  else{
    varBuffer = inBuf;
  }
}*/
