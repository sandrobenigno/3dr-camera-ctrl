/*
ArduCam OSD - The Arduino based Remote Camera Control and OSD.
Copyright (c) 2011.  All rights reserved.
An Open Source Arduino based OSD and Camera Control project.

Author(s): Sandro Benigno
                  Legacy PTP commands from Canon's PS-ReCSDK 1.1.0e
                  Extended PTP commands from CHDK's PTP implementation
                  USB host and PTP library from Oleg Mazurov - circuitsathome.com
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
  
    /*uint16_t SetRecordMode(int mode);
    uint16_t ExecLua(String script);
    uint16_t SetZoom(int zoom);
    uint16_t IncrementZoom(bool zoom);
    uint16_t Shutdown();*/
};

#endif //__ARDUCAM_CHDK_H__
