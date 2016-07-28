#include <stdio.h>
//#include <dos.h>
//#include <conio.h>

typedef unsigned char byte;

byte *keys;
//extern byte keys[];


//void __interrupt (*oldkeybh)();
//void __interrupt keybh();

byte CapturarTeclado(byte *pkeys)
{
    /*oldkeybh=_dos_getvect(9);
    _dos_setvect(9,keybh);*/
    keys=pkeys;
    return(0);
}

byte LiberarTeclado()
{
    //_dos_setvect(9,oldkeybh);
    return(0);
}

//void __interrupt keybh()
//{
//    byte kinfo;

    /*Leer el byte del puerto:
      si el bit 7 esta en 0: la tecla fue presionada
      si el bit 7 esta en 1: la tecla fue soltada
      los 7 bits mas bajos (del 0 al 6) indican el scan code de la tecla*/
    /*kinfo=inp(0x60);
    if (kinfo & 0x80)
        keys[kinfo & 0x7F]=0;
    else
        keys[kinfo & 0x7F]=1;
        
    outp(0x20,0x20);  //Mensaje de interrupcion procesada
    * */
//}


