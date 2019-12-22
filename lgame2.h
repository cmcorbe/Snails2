#include <stdint.h>

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;

typedef struct tagtrgbtriple
{
    byte r;
    byte g;
    byte b;
} trgbtriple;


#define FALSE 0
#define TRUE 1


byte CapturarTeclado(byte *pkeys);
byte LiberarTeclado();
int loadpcx(char *nomarch, byte *imgbuf, trgbtriple *palbuf);

