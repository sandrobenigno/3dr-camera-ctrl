#ifndef __PSEVENTPARSER_H__
#define __PSEVENTPARSER_H__

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

#include "Usb.h"
#include "ptpdebug.h"
#include <canonps.h>
//#include "ptp_from_chdk.h"


class PSEventParser : public PTPReadParser
{
	MultiValueBuffer					theBuffer;
	uint32_t							varBuffer;
	uint8_t								nStage;
        uint16_t							eventCode;
	uint32_t							objHandle;

	MultiByteValueParser				valueParser;

public:
	PSEventParser() : nStage(0), varBuffer(0), objHandle(0) 
	{ 
		theBuffer.pValue = &varBuffer;
	};
	uint32_t GetObjHandle() { return objHandle; };
	virtual void Parse(const uint16_t len, const uint8_t *pbuf, const uint32_t &offset);
};

#endif // __PSEVENTPARSER_H__
