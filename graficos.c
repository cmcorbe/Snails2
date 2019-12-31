#include <stdio.h>

#include "comunes.h"
#include "graficos.h"
#include "pcxdecod.h"

extern tipo_imagen graf_back_buffer;
extern tipo_clipregion crpantalla;

// Fuente
tipo_sizefuente sizetextos[256];
byte imagen_fuente1[715*20];
tipo_imagen graf_fuente1;

typedef union tag_u_dword
{
    byte b;
    word w;
    dword dw;
} _u_dword;

typedef union tag_u_word
{
    byte b;
    word w;
} _u_word;


void cargar_fuente()
{
    trgbtriple paleta[256];
    FILE* archivo = fopen("fuente1.inf", "rb");
    fread(&sizetextos, sizeof(sizetextos[0]), 256, archivo);
    fclose(archivo);

    loadpcx("fuente1.pcx", imagen_fuente1, paleta);

    graf_fuente1.imagen=imagen_fuente1;
    graf_fuente1.ancho=747;
    graf_fuente1.alto=17;
}


void escribir(int desx, int desy, char *texto)
{
    int c=0,ea;
    tipo_clipregion crfuente1;

    ea=sizetextos['A'].y0;

    while (texto[c] != 0)
    {
        crfuente1.x0=sizetextos[texto[c]].x0/*+1*/;
        crfuente1.y0=sizetextos[texto[c]].y0;
        crfuente1.x1=sizetextos[texto[c]].x1+crfuente1.x0;
        crfuente1.y1=sizetextos[texto[c]].y1+crfuente1.y0;


        blt(desx, desy+crfuente1.y0-ea, &crfuente1, &crpantalla, &graf_fuente1, &graf_back_buffer, flag_n);

        desx+=sizetextos[texto[c]].x1;
        c++;
    }
}

void blt(int desx, int desy, tipo_clipregion *regorigen, tipo_clipregion *regdestino,
         tipo_imagen *origen, tipo_imagen *destino, int flags)
{
    int ox,oy,dx,dy,oyy,dyy;
    dword i,j;
    byte *oimagen, *dimagen;

    oimagen=origen->imagen;
    dimagen=destino->imagen;

    dx=desx;
    dy=desy;

    switch (flags)
    {
        case flag_n:
        {
            for (oy=regorigen->y0;oy<regorigen->y1;oy++)
            {
                dyy=dy*destino->ancho;
                oyy=oy*origen->ancho;
                for (ox=regorigen->x0; ox<regorigen->x1; ox++)
                {
                    if (dx>=regdestino->x0 && dx<regdestino->x1)
                        if (dy>=regdestino->y0 && dy<regdestino->y1)
                        {
                            i=ox+oyy;
                            if (oimagen[i]!=color_transparente)
                            {
                                j=dx+dyy;
                                dimagen[j]=oimagen[i];
                            }
                        }
                    dx++;
                }
                dy++;
                dx=desx;
            }
            break;
        }
        case flag_x:
        {
            for (oy=regorigen->y0;oy<regorigen->y1;oy++)
            {
                dyy=dy*destino->ancho;
                oyy=oy*origen->ancho;
                for (ox=regorigen->x1-1;ox>=regorigen->x0;ox--)
                {
                    if (dx>=regdestino->x0 && dx<regdestino->x1)
                        if (dy>=regdestino->y0 && dy<regdestino->y1)
                        {
                            i=ox+oyy;
                            if (oimagen[i]!=color_transparente)
                            {
                                j=dx+dyy;
                                dimagen[j]=oimagen[i];
                            }
                        }
                    dx++;
                }
                dy++;
                dx=desx;
            }
            break;
        }
        case flag_y:
        {
            for (oy=regorigen->y1-1;oy>=regorigen->y0;oy--)
            {
                dyy=dy*destino->ancho;
                oyy=oy*origen->ancho;
                for (ox=regorigen->x0; ox<regorigen->x1; ox++)
                {
                    if (dx>=regdestino->x0 && dx<regdestino->x1)
                        if (dy>=regdestino->y0 && dy<regdestino->y1)
                        {
                            i=ox+oyy;
                            if (oimagen[i]!=color_transparente)
                            {
                                j=dx+dyy;
                                dimagen[j]=oimagen[i];
                            }
                        }
                    dx++;
                }
                dy++;
                dx=desx;
            }
            break;
        }
        case flag_xy:
        {
            for (oy=regorigen->y1-1;oy>=regorigen->y0;oy--)
            {
                dyy=dy*destino->ancho;
                oyy=oy*origen->ancho;
                for (ox=regorigen->x1-1; ox>=regorigen->x0; ox--)
                {
                    if (dx>=regdestino->x0 && dx<regdestino->x1)
                        if (dy>=regdestino->y0 && dy<regdestino->y1)
                        {
                            i=ox+oyy;
                            if (oimagen[i]!=color_transparente)
                            {
                                j=dx+dyy;
                                dimagen[j]=oimagen[i];
                            }
                        }
                    dx++;
                }
                dy++;
                dx=desx;
            }
            break;
        }
    }
}

void bltfast(int desx, int desy, tipo_clipregion *regorigen, tipo_imagen *origen, tipo_imagen *destino)
{
    int oy,dx,dy;
    dword i,dwancho;
    _u_dword *dwoimagen,*dwdimagen;
    _u_word *woimagen,*wdimagen;
    byte par;

    dx=desx;
    dy=desy;
    dwancho=(regorigen->x1-regorigen->x0)/4;
    par=((regorigen->x1-regorigen->x0) % 4);

    if (par==0)
        for (oy=regorigen->y0;oy<regorigen->y1;oy++)
        {
            dwoimagen=(_u_dword*) &origen->imagen[oy*origen->ancho+regorigen->x0];
            dwdimagen=(_u_dword*) &destino->imagen[dy*destino->ancho+desx];

            for (i=0; i<dwancho; i++)
                dwdimagen[i].dw=dwoimagen[i].dw;
            dy++;
        }
    else
        if (par==2)
            for (oy=regorigen->y0;oy<regorigen->y1;oy++)
            {
                dwoimagen=(_u_dword*) &origen->imagen[oy*origen->ancho+regorigen->x0];
                dwdimagen=(_u_dword*) &destino->imagen[dy*destino->ancho+desx];

                for (i=0; i<dwancho; i++)
                    dwdimagen[i].dw=dwoimagen[i].dw;
                dwdimagen[i].w=dwoimagen[i].w;
                dy++;
            }
        else
            if (par==1)
                for (oy=regorigen->y0;oy<regorigen->y1;oy++)
                {
                    dwoimagen=(_u_dword*) &origen->imagen[oy*origen->ancho+regorigen->x0];
                    dwdimagen=(_u_dword*) &destino->imagen[dy*destino->ancho+desx];

                    for (i=0; i<dwancho; i++)
                        dwdimagen[i].dw=dwoimagen[i].dw;
                    dwdimagen[i].b=dwoimagen[i].b;
                    dy++;
                }
            else
                if (par==3)
                    for (oy=regorigen->y0;oy<regorigen->y1;oy++)
                    {
                        dwoimagen=(_u_dword*) &origen->imagen[oy*origen->ancho+regorigen->x0];
                        dwdimagen=(_u_dword*) &destino->imagen[dy*destino->ancho+desx];

                        for (i=0; i<dwancho; i++)
                            dwdimagen[i].dw=dwoimagen[i].dw;

                        wdimagen=(_u_word*) &dwdimagen[i];
                        woimagen=(_u_word*) &dwoimagen[i];
                        wdimagen[0].w=woimagen[0].w;
                        wdimagen[1].b=woimagen[1].b;
                        dy++;
                    }
}

