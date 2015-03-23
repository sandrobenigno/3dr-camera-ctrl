#ifndef __PTPOBJINFOPARSER_H__
#define __PTPOBJINFOPARSER_H__

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
as published by the Free Software Foundation; either version 2+
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <message.h>
#include <parsetools.h>

#include <ptp.h>
#include <mtpconst.h>

const char msgUndefined		[] PROGMEM = "Undefined";

// Ancillary formats
const char msgAssociation	[] PROGMEM = "Association";
const char msgScript		[] PROGMEM = "Script";		
const char msgExecutable	[] PROGMEM = "Executable";	
const char msgText			[] PROGMEM = "Text";			
const char msgHTML			[] PROGMEM = "HTML";			
const char msgDPOF			[] PROGMEM = "DPOF";			
const char msgAIFF	 		[] PROGMEM = "AIFF";	 		
const char msgWAV			[] PROGMEM = "WAV";			
const char msgMP3			[] PROGMEM = "MP3";			
const char msgAVI			[] PROGMEM = "AVI";			
const char msgMPEG			[] PROGMEM = "MPEG";			
const char msgASF			[] PROGMEM = "ASF";			
const char msgQT			[] PROGMEM = "QT";			

// Image formats
const char msgEXIF_JPEG			[] PROGMEM = "EXIF_JPEG";
const char msgTIFF_EP			[] PROGMEM = "TIFF_EP";			
const char msgFlashPix			[] PROGMEM = "FlashPix";			
const char msgBMP				[] PROGMEM = "BMP";				
const char msgCIFF				[] PROGMEM = "CIFF";				
const char msgUndefined_0x3806	[] PROGMEM = "Undefined_0x3806";	
const char msgGIF				[] PROGMEM = "GIF";				
const char msgJFIF				[] PROGMEM = "JFIF";				
const char msgPCD				[] PROGMEM = "PCD";				
const char msgPICT				[] PROGMEM = "PICT";				
const char msgPNG				[] PROGMEM = "PNG";				
const char msgUndefined_0x380C	[] PROGMEM = "Undefined_0x380C";	
const char msgTIFF				[] PROGMEM = "TIFF";				
const char msgTIFF_IT			[] PROGMEM = "TIFF_IT";			
const char msgJP2				[] PROGMEM = "JP2";				
const char msgJPX				[] PROGMEM = "JPX";				

// MTP Object Formats
const char msgUndefined_Firmware				[] PROGMEM = "Undefined_Firmware";	
const char msgWindows_Image_Format				[] PROGMEM = "Windows_Image_Format";	
const char msgUndefined_Audio					[] PROGMEM = "Undefined_Audio";		
const char msgWMA								[] PROGMEM = "WMA";					
const char msgOGG								[] PROGMEM = "OGG";					
const char msgAAC								[] PROGMEM = "AAC";					
const char msgAudible							[] PROGMEM = "Audible";				
const char msgFLAC								[] PROGMEM = "FLAC";					
const char msgUndefined_Video					[] PROGMEM = "Undefined_Video";		
const char msgWMV								[] PROGMEM = "WMV";					
const char msgMP4_Container						[] PROGMEM = "MP4_Container";			
const char msgMP2								[] PROGMEM = "MP2";					
const char msg3GP_Container						[] PROGMEM = "3GP_Container";			


class PTPObjInfoParser : public PTPReadParser
{
	static const char* acNames[];
	static const char* imNames[];

	MultiValueBuffer				theBuffer;
	uint32_t						varBuffer;
	uint8_t							nStage;

	MultiByteValueParser			valueParser;
	PTPListParser					arrayParser;

	static void PrintChar(MultiValueBuffer *p)
	{
		if (((unsigned char*)p->pValue)[0])
			Serial.write(((unsigned char*)p->pValue)[0]);
	};
	void PrintFormat(uint16_t op);

public:
	PTPObjInfoParser() : nStage(0) { theBuffer.pValue = (uint8_t*)&varBuffer; };
	virtual void Parse(const uint16_t len, const uint8_t *pbuf, const uint32_t &offset);
};

#endif // __PTPOBJINFOPARSER_H__

