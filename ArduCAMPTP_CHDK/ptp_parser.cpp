#include "ptp_parser.h"

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
}
