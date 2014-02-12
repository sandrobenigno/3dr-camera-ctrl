#ifndef __PTP_PARSER_H__
#define __PTP_PARSER_H__

#include <inttypes.h>
#include <avr/pgmspace.h>
#include <parsetools.h>
#include "ptpcallback.h"
#include "ptpdebug.h"

class PTPObjHandleParser : public PTPReadParser
{
  MultiValueBuffer theBuffer;
  uint8_t	nStage;
  PTPListParser arrayParser;

public:
  virtual void Parse(const uint16_t len, const uint8_t *pbuf, const uint32_t &offset);
  uint32_t inBuf[17];
  uint32_t prevBuf[17];
  uint32_t* varBuffer;
};


#endif // __PTP_PARSER_H__
