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

#include "pseventparser.h"

void PSEventParser::Parse(const uint16_t len, const uint8_t *pbuf, const uint32_t &offset)
{
	uint16_t	cntdn	= (uint16_t)len;
	uint8_t		*p		= (uint8_t*)pbuf;

	switch (nStage)
	{
	case 0:
		p		+= 12;
		cntdn	-= 12;

		if (!cntdn)
			return;
		nStage	++;

	case 1:
		//Notify(PSTR("\r\nEvent Block Size:\t"));
		theBuffer.valueSize = 4;
		valueParser.Initialize(&theBuffer);
		nStage ++;
	case 2:
		if (!valueParser.Parse(&p, &cntdn))
			return;

		//PrintHex<uint32_t>(*((uint32_t*)theBuffer.pValue));
		nStage ++;
	case 3:
		//Notify(PSTR("\r\nNumber of Fields:\t"));
		theBuffer.valueSize = 2;
		valueParser.Initialize(&theBuffer);
		nStage ++;
	case 4:
		if (!valueParser.Parse(&p, &cntdn))
			return;

		//PrintHex<uint16_t>(*((uint16_t*)theBuffer.pValue));
		nStage ++;
	case 5:
		//Notify(PSTR("\r\nEvent Code:\t"));
		theBuffer.valueSize = 2;
		valueParser.Initialize(&theBuffer);
		nStage ++;
	case 6:
		if (!valueParser.Parse(&p, &cntdn))
			return;

        eventCode = *((uint16_t*)theBuffer.pValue);
		//PrintHex<uint16_t>(*((uint16_t*)theBuffer.pValue));
		nStage ++;
	case 7:
		//Notify(PSTR("\r\nTransaction ID:\t"));
		theBuffer.valueSize = 4;
		valueParser.Initialize(&theBuffer);
		nStage ++;
	case 8:
		if (!valueParser.Parse(&p, &cntdn))
			return;

		//PrintHex<uint32_t>(*((uint32_t*)theBuffer.pValue));
		nStage ++;
    case 9:
        if (eventCode == PTP_EC_ObjectAdded)
			Notify(PSTR("\r\nObject Added:\t\t"));

		theBuffer.valueSize = 4;
		valueParser.Initialize(&theBuffer);
		nStage ++;
    case 10:
        if (eventCode == PTP_EC_ObjectAdded)
        {
			if (!valueParser.Parse(&p, &cntdn))
				return;

			objHandle = *((uint32_t*)theBuffer.pValue);
			//PrintHex<uint32_t>(*((uint32_t*)theBuffer.pValue));
			Notify(PSTR("\r\n"));
        }
		if (eventCode == PTP_EC_CaptureComplete)
			Notify(PSTR("\r\nCapture complete.\r\n"));
		nStage ++;
    case 11:
		nStage = 0;
	}
}