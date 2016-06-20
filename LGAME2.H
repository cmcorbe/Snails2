typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

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

