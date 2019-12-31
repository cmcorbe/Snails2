#ifndef _PCXDECOD_H_
#define _PCXDECOD_H_

#include "comunes.h"

typedef struct tagtrgbtriple
{
    byte r;
    byte g;
    byte b;
} trgbtriple;

int loadpcx(char *nomarch, byte *imgbuf, trgbtriple *palbuf);

#endif