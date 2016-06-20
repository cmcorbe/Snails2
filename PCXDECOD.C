#include <stdio.h>
#include "LGame2.h"


typedef struct tagtpcxheader
{
    byte Manufacturer;
    byte Version;
    byte Encoding;
    byte BitsPerPixel;
    word Xmin;
    word Ymin;
    word Xmax;
    word Ymax;
    word Hdpi;
    word Vdpi;
    byte Colormap[48];
    byte Reserved;
    byte ColorPlanes;
    word BytesPerLine;
    word PaletteInfo;
    word HscreenSize;
    word VscreenSize;
    byte Filler[54];
} tpcxheader;


//FUNCION LECTOR DE PCX <<*>>
int loadpcx(char *nomarch, byte *imgbuf, trgbtriple *palbuf)
{
    tpcxheader header;
    //trgbtriple paleta[256];
    FILE *arch1;
    word imagewidth, imageheight;
    dword totalbytesperline, subtotal, l;
    byte byte1, byte2;
    word x, y;
    byte c,i;

    arch1=fopen(nomarch,"rb");
    if (arch1==NULL)    //Problemas al abrir archivo!
        return(5);
        
    /*Leer la cabecera*/
    //rewind(arch1);
    //return(sizeof(header));
    fread(&header,sizeof(header),1,arch1);

    /*Procesar la cabecera*/
    imagewidth=header.Xmax-header.Xmin+1;
    imageheight=header.Ymax-header.Ymin+1;
    totalbytesperline=header.ColorPlanes*header.BytesPerLine;

    if (header.Version==5)  /*Hay una paleta de 256 colores al final del archivo*/
    {
    //Encontrar el fin del fichero y la posición de la paleta
    /*fseek(arch1,0,SEEK_SET);
    l=0;
    while (feof(arch1)==0)
    {
        fread(&byte1,1,1,arch1);
        l++;
    }
    l--;*/

    //Leer tamaño del archivo
    fseek(arch1,0,SEEK_END);
    l=ftell(arch1);
    
    /*Leer paleta*/
    fseek(arch1,l-769,SEEK_SET);
    fread(&byte1,1,1,arch1);
    if (byte1==12)  /*Format adecuado*/
        fread(palbuf,3,256,arch1);
    else    /*Error: Mal formato*/
        return(1);

    }
    else    /*Error: Versi¢n no reconocida*/
        return(2);

    /*Nos posicionamos al principio de los datos de imagen*/
    fseek(arch1,sizeof(header),SEEK_SET);
    y=0;
    while (y<imageheight)
    {
    x=0; subtotal=0;
    while (x<imagewidth)
    {
        /*Leer el primer byte*/
        fread(&byte1,1,1,arch1);

        if (0xC0 == (byte1 & 0xC0))     /*Hay compresi¢n run-length*/
        {
            /*obtengamos la cantidad de pixeles*/
            c=(byte1 & 0x3F);
            /*leamos el byte de color*/
            fread(&byte2,1,1,arch1);

            /*reproduzcamos c pixeles*/
            for (i=0; i<c; i++)
            {
                if (x==imagewidth)
                    break;

                imgbuf[y*imagewidth+x]=byte2;
                /*imgbuf[(y*imagewidth+x)*4]=paleta[byte2].b;
                imgbuf[(y*imagewidth+x)*4+1]=paleta[byte2].g;
                imgbuf[(y*imagewidth+x)*4+2]=paleta[byte2].r;
                imgbuf[(y*imagewidth+x)*4+3]=0;*/

                subtotal++;
                x++;
            }
        }
        else    /*No hay compresi¢n*/
        {
            imgbuf[y*imagewidth+x]=byte1;
            /*imgbuf[(y*imagewidth+x)*4]=paleta[byte1].b;
            imgbuf[(y*imagewidth+x)*4+1]=paleta[byte1].g;
            imgbuf[(y*imagewidth+x)*4+2]=paleta[byte1].r;
            imgbuf[(y*imagewidth+x)*4+3]=0;*/

            subtotal++;
            x++;
        }
    }
    //Leer lo que falte (basura), ya que cada linea tiene siempre un numero par de bytes
    while (subtotal<totalbytesperline)
    {
        fread(&byte1,1,1,arch1);
        subtotal++;
    }

    //Proxima linea
    y++;
    }

    /*Todo listo*/
    fclose(arch1);
    return(0);
}
//FIN DE LECTOR DE PCX <<*>>

