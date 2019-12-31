#ifndef _GRAFICOS_H_
#define _GRAFICOS_H_

#include "comunes.h"

// Tamaño de pantalla
#define ancho_pantalla 320
#define alto_pantalla 200

#define color_transparente 0

//BLT Flags
#define flag_n 0
#define flag_x 1
#define flag_y 2
#define flag_xy 3

typedef struct tagtipo_imagen
{
    byte *imagen;
    int ancho;
    int alto;
} tipo_imagen;

typedef struct tagtipo_clipregion
{
    int x0,y0,x1,y1;
} tipo_clipregion;

typedef struct tagtipo_sizefuente
{
    short x0,y0,x1,y1;
} tipo_sizefuente;


void cargar_fuente();
void escribir(int desx, int desy, char *texto);
void bltfast(int desx, int desy, tipo_clipregion *regorigen, tipo_imagen *origen, tipo_imagen *destino);
void blt(int desx, int desy, tipo_clipregion *regorigen, tipo_clipregion *regdestino, tipo_imagen *origen, tipo_imagen *destino, int flags);

#endif