#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <SDL/SDL.h>
#include "lgame2.h"
#include "fps_limiter.h"


//CONSTANTES
#define pi 3.14159264
#define FPS 60
#define ancho_pantalla 320//320
#define alto_pantalla 200//200
//#define ancho_pantalla_video 640
//#define alto_pantalla_video 480
#define alto_tablero 10//20
#define alto_barra_energia 2
#define separador_pantallas 2//Pares
#define color_transparente 0
#define color_barra_energia 75
#define color_negro 16
#define max_jugadores 2

//BLT Flags
#define flag_n 0
#define flag_x 1
#define flag_y 2
#define flag_xy 3

//Generador de Terreno
#define ancho_bloque_ter 256
#define alto_bloque_ter 256
#define ancho_bloque_fon 256
#define alto_bloque_fon 256
#define colores_piso 160
#define colores_techo 164

//Terreno
#define ancho_terreno 504//504//159
#define alto_terreno alto_pantalla//350//180
#define gravedad 0.03//0.04//0.03
#define piso 1//Piso que no se carcome
#define colores_fondo 240//Comienzan los colores reservados para el fondo
#define colores_tierra 128//Comienzan los colores reservados para la tierra
#define fin_colores_tierra 160//Terminan los colores reservados para la tierra


//Particulas
#define max_particulas 400
#define rebote_particula 3//3
#define colores_sangre 73
#define fin_colores_sangre 75
#define cuagular 10//10//El cuagulo tira sangre cada x cuadros
#define p_tierra 0
#define p_sangre 1
#define p_cuagulo 2

//Restos
#define max_restos 40
#define ancho_resto 7
#define alto_resto 5
#define restos_por_muerte 5
#define cuagulo_por_muerte 30//30

//Gusano
#define energia_ini 1000
#define alto_gusano 11
#define ancho_gusano 11
#define g_alto_gusano 11
#define g_ancho_gusano 15
#define altura_s_laterales 3
#define altura_s_arriba 1
#define max_animacion 4
#define color_variable1 4
#define color_variable2 5
#define total_col_variables 2
#define acel 0.1
#define friccion 0.05
#define friccion_aire 0.002//0.002
#define max_vel 0.3//0.3
#define rebote 5//5
#define fuerza_salto 1.4//1.4
//#define target_vel 0.04
#define mira_dist 19
#define r_ani_caminar 7//7
#define radio_escavar 6//6
#define dist_escavar 5//5

//Soga
#define soga_vel 6//6
#define soga_tenvel 30//alarga acorta
#define soga_ini 1100
#define soga_min 400
#define soga_max 4000
#define color_soga 12
#define graf_soga 5
#define soga_agarre_dist 6


//Armas
#define max_armas 3//Por jugador
#define g_alto_arma 25
#define g_ancho_arma 25
#define total_armas 20
#define max_animacion_armas 3
#define max_angulos_armas 4
#define desp_guscuadro_0 0
#define desp_guscuadro_1 -1
#define desp_guscuadro_2 -2
#define desp_guscuadro_3 1
#define arma_rot_cambio4al1 -0.3
#define arma_rot_cambio1al2 0.3
#define arma_rot_cambio2al3 1
#define cargada 0
#define disparando 1
#define recargando 2
//Clasificacion ARMAS
#define a_shotgun 0
#define a_ak47 1
#define a_granada 2

//Balas
#define max_balas 1000//100
#define g_ancho_bala 7
#define g_alto_bala 7
#define total_graf_balas 7

//Explosiones

#define max_explosiones 10
#define g_ancho_explo1 25
#define g_alto_explo1 25
#define explo1_max_anim 10
#define r_ani_explo 1

//Camara
#define mostrar_muerte 70
#define cam_vel 30//Bigger, slower
#define cam_normal 0
#define cam_vibrar 1
#define cam_muerto 2

//Keyboard Reference
#define key_esc 1

//Controles
#define max_controles 7
#define c_arriba 0
#define c_abajo 1
#define c_izquierda 2
#define c_derecha 3
#define c_disparo 4
#define c_salto 5
#define c_cambio 6


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

typedef struct tagtipo_scroll{
    int x,y;
}tipo_scroll;

typedef struct tagtipo_camara{
    float x,y;
    int efecto,c;
}tipo_camara;

typedef struct tagtipo_gusano{
    float x,y,velx,vely,angle,mirax,miray,target_vel;
    int jugador,energia,graph,c_animacion,c_barra_energia;
    int gx,gy,flags,escavo,cambio,vida,soga,key_soga;
}tipo_gusano;

typedef struct tagtipo_arma{
    int gx,gy;
    int graph,c_animacion,tipo,ammo,reload,estado,ultimo_graph;
}tipo_arma;

typedef struct tagtipo_bala{
    float x,y,velx,vely,velporcuadro,angle,brebote,caida;
    int gx,gy,flags,graph,ini_graph,jugador,tiempo;
    int c_animacion,multi_dir,vida,empuje,radio,sacar,cambiar_angle;
}tipo_bala;

typedef struct tagtipo_soga{
    float x,y,velx,vely,tension;
    int engusano;
}tipo_soga;

typedef struct tagtipo_resto{
    float x,y,velx,vely;
    int graph,vida,cont_sangre,jugador,c_animacion,ani_vel,tipo;
}tipo_resto;

typedef struct tagtipo_jugador{
    tipo_arma arma[max_armas];
    int selec,vidas;
}tipo_jugador;

typedef struct tagtipo_particula{
        float x,y,velx,vely;
        int vida,tipo,choque,c_cuagulo;
        byte color;
} tipo_particula;

typedef struct tagtipo_explosion{
        float x,y;
        int vida,tipo;
        int graph,c_animacion;
} tipo_explosion;


//PROTOTIPO de Funciones

void configurar_teclas();
void actualizar_controles();
//void crear_particulas(int tipo,int cantidad,float x,float y,float velx,float vely);
void crear_particulas(int tipo,int cantidad,float x,float y,float velx,float vely,byte color);
float random_range(float min, float max);
void advance(float *angle,float *vel,float *x1,float *y1);
void generar_terreno();
void carcomer(int desx,int desy,int radio);
void escribir(int desx,int desy,tipo_clipregion *regdestino,tipo_imagen *origen,char *texto);
void blt(int desx,int desy,tipo_clipregion *regorigen,tipo_clipregion *regdestino,tipo_imagen *origen,tipo_imagen *destino,int flags);
void bltfast(int desx,int desy,tipo_clipregion *regorigen,tipo_imagen *origen,tipo_imagen *destino);
void farma(tipo_arma *datos,int jugador);
void fbala(tipo_bala *datos);
void crear_explosion(float x, float y);
void fexplosion(tipo_explosion *datos);
void explo_volar(tipo_bala* bala);
void fgusano(tipo_gusano *datos);
void fparticula(tipo_particula *datos);
void fresto(tipo_resto *datos);
void fsoga(int jugador);
void draw_rectangle(int start_x, int start_y, int end_x, int end_y, byte color);
void fbarra_energia();


//Variables GLOBALES
    trgbtriple paleta1[256];

    byte back_buffer[ancho_pantalla*alto_pantalla];

    tipo_scroll scroll[max_jugadores];
    tipo_camara camara[max_jugadores];
    tipo_sizefuente sizetextos[256];

    tipo_gusano gusano[max_jugadores];
    //tipo_jugador jugador[max_jugadores];
    tipo_jugador jugador[max_jugadores];
    tipo_soga soga[max_jugadores];
    tipo_particula particula[max_particulas];
    tipo_bala bala[max_balas];
    tipo_resto resto[max_restos];
    tipo_explosion explosion[max_explosiones];
    //Clipers
        tipo_clipregion crtemp;//Region de Clip temporal para usos aislados
        tipo_clipregion crpantalla;
        tipo_clipregion cregionpj[max_jugadores];
        tipo_clipregion crgus;
        tipo_clipregion crmira;
        tipo_clipregion crtercam;


    //Graficos
        byte imagen_terreno[ancho_terreno*alto_terreno];
        byte imagen_fondo[ancho_terreno*alto_terreno];
        byte imagen_gusano[max_jugadores][g_ancho_gusano*(max_animacion+1+2)*g_alto_gusano];
        byte imagen_armas[total_armas*g_alto_arma*g_ancho_arma*max_animacion_armas*max_angulos_armas];
        byte imagen_fuente1[715*20];
        byte imagen_balas[g_ancho_bala*g_alto_bala*total_graf_balas];
        byte imagen_explosiones[g_ancho_explo1 * g_alto_explo1 * explo1_max_anim];
        byte imagen_temppal[1];


        tipo_imagen graf_back_buffer;
        tipo_imagen graf_fuente1;
        tipo_imagen graf_gusano[max_jugadores];
        tipo_imagen graf_terreno;
        tipo_imagen graf_fondo;
        tipo_imagen graf_armas;
        tipo_imagen graf_balas;
        tipo_imagen graf_explosiones;


    //Controles
        //byte keyboard[256];
        byte *keyboard;
        byte control[max_jugadores][max_controles];
        byte conf_control[max_jugadores][max_controles];

SDL_Surface* screen;
SDL_Surface* sdlBuffer;

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void configurar_teclas()
{
    int contj,contc,ccapturado,c;
    char * nom_controles[max_controles];
    FILE *archivo;

    nom_controles[c_arriba]="UP";
    nom_controles[c_abajo]="DOWN";
    nom_controles[c_izquierda]="LEFT";
    nom_controles[c_derecha]="RIGHT";
    nom_controles[c_disparo]="FIRE";
    nom_controles[c_salto]="JUMP";
    nom_controles[c_cambio]="CHANGE";


    //_setvideomode(_DEFAULTMODE);
    CapturarTeclado(keyboard);

    /*while (1)
    {
        //sleep(1);
        _settextposition(1,1);
        for(c=0; c<128; c++)
            printf("%X",keyboard[c]);
            printf("\n");
    }*/

    for (contj=0;contj<max_jugadores;contj++)
    {
        /*_setbkcolor(_BLACK);
        _settextcolor(7);
        _clearscreen(_GCLEARSCREEN);
        _settextcolor(15);
        _setbkcolor(_BLUE);
        _outtext(" Keyboard Configuration Program  V1.0  Copyright 2002 ");*/
        printf("\n\n");

        printf("PLAYER    %i :\n\n",contj+1);

        for (contc=0;contc<max_controles;contc++)
        {
            printf("%s %s\n","PRESS KEY",nom_controles[contc]);
            ccapturado=-1;
            while (ccapturado==-1)
            {
                SDL_PumpEvents();
                for (c=0;c<128;c++)
                {
                    if(keyboard[c])
                        ccapturado=c;
                }
            }
            conf_control[contj][contc]=ccapturado;

            printf("...........READY\n\n");
            while (keyboard[ccapturado])
                SDL_PumpEvents();
        }
    }
    LiberarTeclado();
    archivo=fopen("keys.cfg","wb");
    fwrite(&conf_control,sizeof(conf_control),1,archivo);
    fclose(archivo);
}

void actualizar_controles()
{
    int contj,contc;

    for (contj=0;contj<max_jugadores;contj++)
    {
        for (contc=0;contc<max_controles;contc++)
        {
            if (keyboard[conf_control[contj][contc]])
                control[contj][contc]=TRUE;
            else
                control[contj][contc]=FALSE;
        }
    }
}

void wait(float seconds)
{
    clock_t start = clock();
    while (((float)(clock() - start)) / CLOCKS_PER_SEC < seconds);
}


////////////////////////////////////////-> MAIN <-/////////////////////////////////////////////
int main(int argc, char *argv[])
{
    printf("todo ok");
    SDL_Init( SDL_INIT_EVERYTHING );
    screen = SDL_SetVideoMode(ancho_pantalla, alto_pantalla, 8, SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF);
    sdlBuffer = SDL_CreateRGBSurfaceFrom(&back_buffer, ancho_pantalla, alto_pantalla, 8, ancho_pantalla, 0, 0, 0, 0);
    keyboard = SDL_GetKeyState(NULL);

    clock_t tiempo;
    float t2,fps=0;
    int c=0, j=0, c_relativasX, c_relativasY, ccuadros=0;
    FILE *archivo;
    char cfps[10];

    crpantalla.x0=0;
    crpantalla.y0=0;
    crpantalla.x1=ancho_pantalla;
    crpantalla.y1=alto_pantalla;

    cregionpj[0].x0=0;
    cregionpj[0].y0=0;
    cregionpj[0].x1=ancho_pantalla/2-separador_pantallas/2;
    cregionpj[0].y1=alto_pantalla-alto_tablero;

    cregionpj[1].x0=ancho_pantalla/2+separador_pantallas/2;
    cregionpj[1].y0=0;
    cregionpj[1].x1=ancho_pantalla;
    cregionpj[1].y1=alto_pantalla-alto_tablero;

    if (max_jugadores>2)
    {
        cregionpj[0].x0=0;
        cregionpj[0].y0=0;
        cregionpj[0].x1=ancho_pantalla/2-separador_pantallas/2;
        cregionpj[0].y1=alto_pantalla/2-alto_tablero-separador_pantallas/2;

        cregionpj[1].x0=ancho_pantalla/2+separador_pantallas/2;
        cregionpj[1].y0=0;
        cregionpj[1].x1=ancho_pantalla;
        cregionpj[1].y1=alto_pantalla/2-alto_tablero-separador_pantallas/2;

        cregionpj[2].x0=0;
        cregionpj[2].y0=alto_pantalla/2+separador_pantallas/2;
        cregionpj[2].x1=ancho_pantalla/2-separador_pantallas/2;
        cregionpj[2].y1=alto_pantalla-alto_tablero;

        if (max_jugadores>3)
        {
            cregionpj[3].x0=ancho_pantalla/2+separador_pantallas/2;
            cregionpj[3].y0=alto_pantalla/2+separador_pantallas/2;
            cregionpj[3].x1=ancho_pantalla;
            cregionpj[3].y1=alto_pantalla-alto_tablero;
        }
    }

    crgus.x0=0;
    crgus.y0=0;
    crgus.x1=g_ancho_gusano;
    crgus.y1=g_alto_gusano;

    crmira.x0=g_ancho_gusano*max_animacion;
    crmira.y0=0;
    crmira.x1=g_ancho_gusano*(max_animacion+1);
    crmira.y1=g_alto_gusano;


    graf_back_buffer.imagen=back_buffer;
    graf_back_buffer.ancho=ancho_pantalla;
    graf_back_buffer.alto=alto_pantalla;

    graf_terreno.imagen=imagen_terreno;
    graf_terreno.ancho=ancho_terreno;
    graf_terreno.alto=alto_terreno;

    graf_fondo.imagen=imagen_fondo;
    graf_fondo.ancho=ancho_terreno;
    graf_fondo.alto=alto_terreno;

    for (j=0;j<max_jugadores;j++)
    {
        graf_gusano[j].imagen=imagen_gusano[j];
        graf_gusano[j].ancho=g_ancho_gusano*(max_animacion+1+2);
        graf_gusano[j].alto=g_alto_gusano;
    }

    graf_armas.imagen=imagen_armas;
    graf_armas.ancho=g_ancho_arma*max_animacion_armas*max_angulos_armas;
    graf_armas.alto=g_alto_arma*total_armas;

    graf_balas.imagen=imagen_balas;
    graf_balas.ancho=g_ancho_bala*total_graf_balas;
    graf_balas.alto=g_alto_bala;

    graf_explosiones.imagen = imagen_explosiones;
    graf_explosiones.alto = g_alto_explo1;
    graf_explosiones.ancho = g_ancho_explo1 * explo1_max_anim;

//PROPIEDADES DE FUENTES
    graf_fuente1.imagen=imagen_fuente1;
    graf_fuente1.ancho=747;
    graf_fuente1.alto=17;

//CARGA DE ARCHIVOS
    for (c=0;c<max_jugadores;c++)
    {
        gusano[c].jugador=c;
    }

    archivo=fopen("keys.cfg","rb");
    if (archivo != NULL)
    {
        fread(&conf_control,sizeof(conf_control),1,archivo);
        fclose(archivo);
    }
    else
        configurar_teclas();

    archivo=fopen("fuente1.inf","rb");
    fread(&sizetextos,sizeof(sizetextos[0]),256,archivo);
    fclose(archivo);

    loadpcx("fuente1.pcx",imagen_fuente1,paleta1);

    for (c=0;c<max_jugadores;c++)
        loadpcx("caracol.pcx",imagen_gusano[c],paleta1);

    loadpcx("armas.pcx",imagen_armas,paleta1);
    loadpcx("balas.pcx",imagen_balas,paleta1);
    loadpcx("explo.pcx",imagen_explosiones,paleta1);



    srand((unsigned int)time(NULL));//Saca la semilla de RAND con CLOCK


    generar_terreno();


//Cambio los indices de colores de los gusanos para que sean diferentes
    for (j=0;j<max_jugadores;j++)
    {
        for (c=0;c<g_ancho_gusano*(max_animacion+1+2)*g_alto_gusano;c++)
        {
            if (imagen_gusano[j][c]==color_variable1)
                imagen_gusano[j][c]=color_variable1+total_col_variables*j;
            if (imagen_gusano[j][c]==color_variable2)
                imagen_gusano[j][c]=color_variable2+total_col_variables*j;
        }
    }

    //_setvideomode(_MRES256COLOR);


//Cargo la Paleta al Sistema
    loadpcx("gamepal.pcx", imagen_temppal, paleta1);

    SDL_Color sdlPalette[256];
    for (c = 0; c < 256; c++)
    {
        sdlPalette[c].r = paleta1[c].r;
        sdlPalette[c].g = paleta1[c].g;
        sdlPalette[c].b = paleta1[c].b;
    }

    SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, sdlPalette, 0, 256);
    SDL_SetPalette(sdlBuffer, SDL_LOGPAL | SDL_PHYSPAL, sdlPalette, 0, 256);


//Pongo el back buffer en negro
    for (c=0;c<ancho_pantalla*alto_pantalla;c++)
    {
        back_buffer[c]=16;
    }

    for (c=0;c<max_jugadores;c++)
    {
        camara[c].x=ancho_terreno/2;
        camara[c].y=alto_terreno/2;

        for (j=0;j<max_armas;j++)
            jugador[c].arma[j].tipo=j;

        gusano[c].x=random_range(ancho_gusano,ancho_terreno-ancho_gusano);
        gusano[c].y=random_range(alto_gusano,alto_terreno-alto_gusano);
        gusano[c].gx=gusano[c].x-g_ancho_gusano/2;
        gusano[c].gy=gusano[c].y-g_alto_gusano/2-1;

    }

    CapturarTeclado(keyboard);

    fps_set(FPS);
    tiempo=clock();
    while (keyboard[SDLK_ESCAPE]==FALSE)
    {
        fps_start_game_loop();

        actualizar_controles();

        for (c=0;c<max_jugadores;c++)
        {
            if (gusano[c].vida==FALSE)
            {
                camara[c].c++;
                if (camara[c].c>mostrar_muerte)
                {
                    if (fabs(gusano[c].gx-camara[c].x)>1 && fabs(gusano[c].gy-camara[c].y)>1)
                    {
                        camara[c].x+=(gusano[c].gx-camara[c].x)/cam_vel;
                        camara[c].y+=(gusano[c].gy-camara[c].y)/cam_vel;
                    }
                    else
                    {
                        camara[c].c=0;
                        camara[c].efecto=cam_normal;
                        //if (control[c][c_izquierda] || control[c][c_derecha])
                        //{
                            gusano[c].vida=TRUE;
                            gusano[c].energia=energia_ini;
                            gusano[c].c_barra_energia=0;
                            carcomer((int)gusano[c].x,(int)gusano[c].y,radio_escavar);
                        //}
                    }
                }
             }
             if (gusano[c].vida==TRUE)
                fgusano(&gusano[c]);

            scroll[c].x=(int)camara[c].x-(cregionpj[c].x1-cregionpj[c].x0)/2;
            scroll[c].y=(int)camara[c].y-(cregionpj[c].y1-cregionpj[c].y0)/2;



            if (scroll[c].x<0)
                scroll[c].x=0;
            if (scroll[c].x>ancho_terreno-(cregionpj[c].x1-cregionpj[c].x0))
                scroll[c].x=ancho_terreno-(cregionpj[c].x1-cregionpj[c].x0);
            if (scroll[c].y<0)
                scroll[c].y=0;
            if (scroll[c].y>alto_terreno-(cregionpj[c].y1-cregionpj[c].y0))
                scroll[c].y=alto_terreno-(cregionpj[c].y1-cregionpj[c].y0);
        }

//Actualizo el fondo
        for (c=0;c<max_jugadores;c++)
        {
            c_relativasX=cregionpj[c].x0;
            c_relativasY=cregionpj[c].y0;

            crtercam.x0=scroll[c].x;
            crtercam.y0=scroll[c].y;
            crtercam.x1=scroll[c].x+(cregionpj[c].x1-cregionpj[c].x0);
            crtercam.y1=scroll[c].y+(cregionpj[c].y1-cregionpj[c].y0);

            bltfast(c_relativasX,c_relativasY,&crtercam,&graf_terreno,&graf_back_buffer);

            fbarra_energia();
        }
        for (c=0;c<max_jugadores;c++)
        {
            if (gusano[c].soga==TRUE)
                fsoga(c);
        }
        for (c=0;c<max_jugadores;c++)
        {
            c_relativasX=cregionpj[c].x0;
            c_relativasY=cregionpj[c].y0;

            crtercam.x0=scroll[c].x;
            crtercam.y0=scroll[c].y;
            crtercam.x1=scroll[c].x+(cregionpj[c].x1-cregionpj[c].x0);
            crtercam.y1=scroll[c].y+(cregionpj[c].y1-cregionpj[c].y0);

            for (j=0;j<max_jugadores;j++)
            {
                if (j!=c)//El gusano siempre arriba en su pantalla
                {
                    if (gusano[j].vida==TRUE)
                    {
                    crgus.x0=gusano[j].graph*g_ancho_gusano;
                    crgus.x1=crgus.x0+g_ancho_gusano;
                    blt(gusano[j].gx+c_relativasX-scroll[c].x,gusano[j].gy+c_relativasY-scroll[c].y,&crgus,&cregionpj[c],&graf_gusano[j],&graf_back_buffer,gusano[j].flags);


                    crtemp.x0=jugador[j].arma[jugador[j].selec].graph*g_ancho_arma;
                    crtemp.x1=crtemp.x0+g_ancho_arma;
                    crtemp.y0=jugador[j].arma[jugador[j].selec].tipo*g_alto_arma;
                    crtemp.y1=crtemp.y0+g_alto_arma;
                    blt(jugador[j].arma[jugador[j].selec].gx+c_relativasX-scroll[c].x,jugador[j].arma[jugador[j].selec].gy+c_relativasY-scroll[c].y,&crtemp,&cregionpj[c],&graf_armas,&graf_back_buffer,gusano[j].flags);
                    }
                }
            }
            if (gusano[c].vida==TRUE)
            {
            crgus.x0=gusano[c].graph*g_ancho_gusano;
            crgus.x1=crgus.x0+g_ancho_gusano;

            blt(gusano[c].gx+c_relativasX-scroll[c].x,gusano[c].gy+c_relativasY-scroll[c].y,&crgus,&cregionpj[c],&graf_gusano[c],&graf_back_buffer,gusano[c].flags);

            blt((int)gusano[c].mirax+c_relativasX-scroll[c].x,(int)gusano[c].miray+c_relativasY-scroll[c].y,&crmira,&cregionpj[c],&graf_gusano[c],&graf_back_buffer,flag_n);

            crtemp.x0=jugador[c].arma[jugador[c].selec].graph*g_ancho_arma;
            crtemp.x1=crtemp.x0+g_ancho_arma;
            crtemp.y0=jugador[c].arma[jugador[c].selec].tipo*g_alto_arma;
            crtemp.y1=crtemp.y0+g_alto_arma;
            blt(jugador[c].arma[jugador[c].selec].gx+c_relativasX-scroll[c].x,jugador[c].arma[jugador[c].selec].gy+c_relativasY-scroll[c].y,&crtemp,&cregionpj[c],&graf_armas,&graf_back_buffer,gusano[c].flags);
            }
        }
        for (c=0;c<max_restos;c++)
        {
            if (resto[c].vida==TRUE)
            {
                fresto(&resto[c]);
            }
        }
        for (c=0;c<max_balas;c++)
        {
            if (bala[c].vida==TRUE)
            {
                fbala(&bala[c]);
            }
        }
        for (c=0;c<max_explosiones;c++)
        {
            if (explosion[c].vida==TRUE)
            {
                fexplosion(&explosion[c]);
            }
        }

        for (c=0;c<max_particulas;c++)
        {
            if (particula[c].vida==TRUE)
                fparticula(&particula[c]);
        }

        // Mostrar FPS
        sprintf(cfps, "FPS:%0.f", fps);
        escribir(0,alto_pantalla-alto_tablero-10-10,&crpantalla,&graf_fuente1,cfps);//170y//for  3 or 4 p 190y

        /*
        SDL_Rect destino;

        destino.x = 0;
        destino.y = 0;
        destino.h = ancho_pantalla_video;
        destino.w = alto_pantalla;
        SDL_BlitScaled(sdlBuffer, NULL, screen, &destino);
        */
        SDL_BlitSurface(sdlBuffer, NULL, screen, NULL);
        SDL_Flip(screen);
        SDL_PumpEvents();
        //wait(1.0 / 70.0);
        fps_end_game_loop();

        // Calcular FPS
        ccuadros++;
        t2 = ((float) (clock() - tiempo)) / CLOCKS_PER_SEC;
        if (t2>2)
        {
            fps=ccuadros/t2;
            ccuadros=0;
            tiempo=clock();
        }


//Pantalla actualizada
    }

    LiberarTeclado();

    //_setvideomode(_DEFAULTMODE);
    SDL_Quit();

    //tiempo=clock()/CLOCKS_PER_SEC;
    //printf("%f \n",fps);

}//FIN DEL PROGRAMA


/////////////////////////////////////////-> Blt <-/////////////////////////////////////////////
void blt(int desx,int desy,tipo_clipregion *regorigen,tipo_clipregion *regdestino,tipo_imagen *origen,tipo_imagen *destino,int flags)
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


///////////////////////////////////////-> BltFast <-////////////////////////////////////////////
void bltfast(int desx,int desy,tipo_clipregion *regorigen,tipo_imagen *origen,tipo_imagen *destino)
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


float random_range(float min,float max)
{
        float result;

        result=(float)rand()/RAND_MAX*(max-min)+min;


        return result;
}
void advance(float *angle,float *vel,float *x1,float *y1)
{
    *x1=*x1+cos(*angle)*(*vel);
    *y1=*y1-sin(*angle)*(*vel);
}



void animar(int *graph,int *c_animacion,int comienzo,int fin,int retardo)

{
    *c_animacion+=1;

    if (comienzo<fin)
    {
        if (*graph<comienzo || *graph>fin)
        {
            *graph=comienzo;
            *c_animacion=0;
        }
        if (*c_animacion>retardo)
        {
            *graph+=1;
            *c_animacion=0;
            if (*graph>fin)
                *graph=comienzo;
        }
    }
    else
    {
        if (*graph>comienzo || *graph<fin)
        {
            *graph=comienzo;
            *c_animacion=0;
        }
        if (*c_animacion>retardo)
        {
            *graph-=1;
            *c_animacion=0;
            if (*graph<fin)
                *graph=comienzo;
        }

    }
}



//////////////////////////////////////-> Escribir <-///////////////////////////////////////////
void escribir(int desx,int desy,tipo_clipregion *regdestino,tipo_imagen *origen,char *texto)
{
    int c=0,ea;
    tipo_imagen graf_backbuffer;
    tipo_clipregion crfuente1;

    graf_backbuffer.imagen=back_buffer;
    graf_backbuffer.ancho=ancho_pantalla;
    graf_backbuffer.alto=alto_pantalla;

    ea=sizetextos['A'].y0;

    while (texto[c] != 0)
    {
        crfuente1.x0=sizetextos[texto[c]].x0/*+1*/;
        crfuente1.y0=sizetextos[texto[c]].y0;
        crfuente1.x1=sizetextos[texto[c]].x1+crfuente1.x0;
        crfuente1.y1=sizetextos[texto[c]].y1+crfuente1.y0;


        blt(desx,desy+crfuente1.y0-ea,&crfuente1,&crpantalla,&graf_fuente1,&graf_backbuffer,flag_n);

        desx+=sizetextos[texto[c]].x1;
        c++;
    }
}


//////////////////////////////////////-> fGusano <-////////////////////////////////////////////
void fgusano(tipo_gusano *datos)
{
    int tempx,tempy,c,j;
    float temp,tx,ty;

    if (!(control[datos->jugador][c_derecha] && control[datos->jugador][c_izquierda]) && !control[datos->jugador][c_cambio])
    {
    if (control[datos->jugador][c_derecha])
    {
        animar(&datos->graph,&datos->c_animacion,0,3,r_ani_caminar);
        if (datos->velx<max_vel)
            datos->velx+=acel;
        if (datos->flags==flag_x)
        {
            datos->flags=flag_n;
            datos->angle=pi-datos->angle;
        }
    }
    else
    {
        if (control[datos->jugador][c_izquierda])
        {
            animar(&datos->graph,&datos->c_animacion,0,3,r_ani_caminar);
            if (datos->velx>-max_vel)
                datos->velx-=acel;
            if (datos->flags==flag_n)
            {
                datos->flags=flag_x;
                datos->angle=pi-datos->angle;
            }
        }
        else
            datos->graph=0;
    }
    }


    if (datos->flags==flag_n)
    {
        if (control[datos->jugador][c_arriba] && datos->angle < pi/2-datos->target_vel)
            datos->angle+=datos->target_vel;

        if (control[datos->jugador][c_abajo] && datos->angle > -pi/3+datos->target_vel)
            datos->angle-=datos->target_vel;
    }
    else
    {
        if (control[datos->jugador][c_arriba] && datos->angle > pi/2+datos->target_vel)
            datos->angle-=datos->target_vel;

        if (control[datos->jugador][c_abajo] && datos->angle < pi+pi/3-datos->target_vel)
            datos->angle+=datos->target_vel;
    }

    //ESCAVAR
    if (control[datos->jugador][c_izquierda] && (control[datos->jugador][c_derecha]))
    {
        if (!datos->escavo)
        {
            temp=dist_escavar;
            tx=datos->x;
            ty=datos->y;
            advance(&datos->angle,&temp,&tx,&ty);
            carcomer((int)tx,(int)ty,radio_escavar);
            datos->escavo=TRUE;
        }
    }
    else
    {
        datos->escavo=FALSE;
    }




    datos->vely+=gravedad;


    datos->x+=datos->velx;

//SENSORES-SENSORES-SENSORES-SENSORES-SENSORES-SENSORES-SENSORES-SENSORES-SENSORES-SENSORES-SENSORES-
    if (datos->vely>=0)
    {
        tempx=(int)datos->x;
        tempy=(int)datos->y+alto_gusano/2;
        if (graf_terreno.imagen[tempx+tempy*ancho_terreno]<colores_fondo || tempy>alto_terreno)
        {
            while(graf_terreno.imagen[tempx+tempy*ancho_terreno]<colores_fondo || tempy>alto_terreno)
            {
                datos->y--;
                tempy=(int)datos->y+alto_gusano/2;
            }
            datos->y++;

            if (datos->velx>friccion)
                datos->velx-=friccion;
            else
                if (datos->velx<-friccion)
                    datos->velx+=friccion;
                else
                    datos->velx=0;


            if (datos->vely>1)
            {
                datos->vely=-datos->vely/rebote;
                datos->y--;
            }
            else
            {
                datos->vely=0;
                if (control[datos->jugador][c_salto])
                    datos->vely=-fuerza_salto;
            }
        }
    }
    if (datos->vely<0)
    {
        tempx=(int)datos->x;
        tempy=(int)datos->y-alto_gusano/2+altura_s_arriba;
        if (graf_terreno.imagen[tempx+tempy*ancho_terreno]<colores_fondo || tempy<0)
        {
            while(graf_terreno.imagen[tempx+tempy*ancho_terreno]<colores_fondo || tempy<0)
            {
                datos->y++;
                tempy=(int)datos->y-alto_gusano/2+altura_s_arriba;
            }
            datos->y--;
            datos->vely=-datos->vely/rebote;
        }
    }
    if (datos->velx>0)
    {
        tempx=(int)datos->x+ancho_gusano/2;
        tempy=(int)datos->y-altura_s_laterales;
        if (graf_terreno.imagen[tempx+tempy*ancho_terreno]<colores_fondo || tempx>ancho_terreno)
        {
            while(graf_terreno.imagen[tempx+tempy*ancho_terreno]<colores_fondo || tempx>ancho_terreno)
            {
                datos->x--;
                tempx=(int)datos->x+ancho_gusano/2;
            }
            datos->x++;
            datos->velx=0;
        }
    }
    if (datos->velx<0)
    {
        tempx=(int)datos->x-ancho_gusano/2;
        tempy=(int)datos->y-altura_s_laterales;
        if (graf_terreno.imagen[tempx+tempy*ancho_terreno]<colores_fondo || tempx<0)
        {
            while(graf_terreno.imagen[tempx+tempy*ancho_terreno]<colores_fondo || tempx<0)
            {
                datos->x++;
                tempx=(int)datos->x-ancho_gusano/2;
           }
            datos->x--;
            datos->velx=0;
        }
    }
//FIN-SENSORES-FIN-SENSORES-FIN-SENSORES-FIN-SENSORES-FIN-SENSORES-FIN-SENSORES-FIN-SENSORES-FIN-SENSORES-

    datos->y+=datos->vely;


    if (datos->energia<0)
    {
        crear_particulas(p_cuagulo,cuagulo_por_muerte,datos->x,datos->y,datos->velx/2,-0.6,FALSE);//SANGRE
        for (j=0;j<restos_por_muerte;j++)
        {
            for(c=0;c<max_restos;c++)
            {
                if (resto[c].vida==FALSE)
                {
                    resto[c].jugador=datos->jugador;
                    resto[c].velx=datos->velx/2+random_range(-1.5,1.5);
                    resto[c].vely=datos->vely/2+random_range(-2,0);
                    resto[c].x=datos->x+random_range(-3,3);
                    resto[c].y=datos->y+random_range(-3,3);
                    resto[c].ani_vel=(int)random_range(4,10);
                    resto[c].vida=TRUE;
                    break;
                }
            }
        }
        jugador[datos->jugador].vidas--;
        datos->soga=FALSE;
        datos->angle=0;
        datos->flags=flag_n;
        datos->graph=0;
        datos->velx=0;
        datos->vely=0;
        datos->x=random_range(ancho_gusano,ancho_terreno-ancho_gusano);
        datos->y=random_range(alto_gusano,alto_terreno-alto_gusano);
        camara[datos->jugador].efecto=cam_muerto;

        datos->vida=FALSE;
    }
//SOGA SOGA SOGA SOGA

    if (datos->soga==TRUE)
    {
        if (control[datos->jugador][c_salto] && !datos->key_soga)
            datos->soga=FALSE;

        if (!control[datos->jugador][c_salto] && !control[datos->jugador][c_cambio])
            datos->key_soga=FALSE;


        if (control[datos->jugador][c_arriba] && control[datos->jugador][c_cambio] && soga[datos->jugador].tension>soga_min)
            soga[datos->jugador].tension-=soga_tenvel;

        if (control[datos->jugador][c_abajo] && control[datos->jugador][c_cambio] && soga[datos->jugador].tension<soga_max)
            soga[datos->jugador].tension+=soga_tenvel;

        //FRICCION AIRE
        if (datos->velx>friccion_aire)
            datos->velx-=friccion_aire;
        else
            if (datos->velx<-friccion_aire)
                datos->velx+=friccion_aire;
            else
                datos->velx=0;
        if (datos->vely>friccion_aire)
            datos->vely-=friccion_aire;
        else
            if (datos->vely<-friccion_aire)
                datos->vely+=friccion_aire;
            else
                datos->vely=0;
    }


    if (control[datos->jugador][c_salto] && control[datos->jugador][c_cambio])
    {
        if (datos->soga==FALSE)
        {
            datos->key_soga=TRUE;
            datos->soga=TRUE;
            soga[datos->jugador].x=datos->x;
            soga[datos->jugador].y=datos->y;
            soga[datos->jugador].velx=cos(datos->angle)*soga_vel;
            soga[datos->jugador].vely=-sin(datos->angle)*soga_vel;
            soga[datos->jugador].tension=soga_ini;
            soga[datos->jugador].engusano=-1;
        }
    }



    datos->gx=(int)datos->x-g_ancho_gusano/2;
    datos->gy=(int)datos->y-g_alto_gusano/2-1;//<-Guarda con el -1 por el grafico impar para que no se incrute

    temp=mira_dist;
    datos->mirax=datos->gx;
    datos->miray=datos->gy;
    advance(&datos->angle,&temp,&datos->mirax,&datos->miray);

//MANEJO DE ARMAS-MANEJO DE ARMAS-MANEJO DE ARMAS-MANEJO DE ARMAS-MANEJO DE ARMAS-MANEJO DE ARMAS-
    if (control[datos->jugador][c_cambio] && !control[datos->jugador][c_derecha] && !control[datos->jugador][c_izquierda])
        datos->cambio=FALSE;

    if (control[datos->jugador][c_derecha] && control[datos->jugador][c_cambio] && !datos->cambio)
    {
        jugador[datos->jugador].selec++;
        datos->cambio=TRUE;
        if (jugador[datos->jugador].selec>=max_armas)
            jugador[datos->jugador].selec=0;
    }

    if (control[datos->jugador][c_izquierda] && control[datos->jugador][c_cambio] && !datos->cambio)
    {
        jugador[datos->jugador].selec--;
        datos->cambio=TRUE;
        if (jugador[datos->jugador].selec<0)
            jugador[datos->jugador].selec=max_armas-1;
    }

    farma(&jugador[datos->jugador].arma[jugador[datos->jugador].selec],datos->jugador);


    if (camara[datos->jugador].efecto==cam_normal)
    {
        camara[datos->jugador].x=datos->gx;
        camara[datos->jugador].y=datos->gy;
    }
}
//////////////////////////////////////-> fSoga <-///////////////////////////////////////////
void fsoga(int jugador)
{
    float tx,ty,tvelx,tvely,norma;
    byte dibujando=TRUE,color=color_soga;
    int tx2,ty2,c,gx,gy,error=0;

    tvelx=gusano[jugador].x-soga[jugador].x;
    tvely=gusano[jugador].y-soga[jugador].y;

    norma=sqrt(pow(tvelx,2)+pow(tvely,2));


    soga[jugador].vely+=gravedad;
    if (soga[jugador].engusano==-1)
    {
        if (imagen_terreno[(int)soga[jugador].x+(int)soga[jugador].y*ancho_terreno]<colores_fondo || soga[jugador].x<=0 || soga[jugador].x>=ancho_terreno || soga[jugador].y<=0 || soga[jugador].y>=alto_terreno)
        {
            gusano[jugador].velx-=tvelx/soga[jugador].tension;//Elasticidad
            gusano[jugador].vely-=tvely/soga[jugador].tension;
            soga[jugador].velx=0;
            soga[jugador].vely=0;
        }
        else
        {
            soga[jugador].velx+=tvelx/soga[jugador].tension/2;//Elasticidad
            soga[jugador].vely+=tvely/soga[jugador].tension/2;//Doble que enganchada(/2)

            soga[jugador].x+=soga[jugador].velx;
            soga[jugador].y+=soga[jugador].vely;
            for(c=0;c<max_jugadores;c++)
            {
                if (c != jugador)
                {
                if (soga[jugador].x>gusano[c].x-ancho_gusano/2 && soga[jugador].x<gusano[c].x+ancho_gusano/2)
                {
                    if (soga[jugador].y>gusano[c].y-alto_gusano/2 && soga[jugador].y<gusano[c].y+alto_gusano/2)
                    {
                        soga[jugador].engusano=c;
                        break;
                    }
                }
                }
            }
        }
    }
    else//de en gusano
    {
        if (gusano[soga[jugador].engusano].vida==TRUE)
        {
            gusano[soga[jugador].engusano].velx+=tvelx/soga[jugador].tension;//Muevo al que
            gusano[soga[jugador].engusano].vely+=tvely/soga[jugador].tension;//enganche

            gusano[jugador].velx-=tvelx/soga[jugador].tension;//Elasticidad
            gusano[jugador].vely-=tvely/soga[jugador].tension;
            soga[jugador].velx=0;
            soga[jugador].vely=0;
            soga[jugador].x=gusano[soga[jugador].engusano].x;
            soga[jugador].y=gusano[soga[jugador].engusano].y;
        }
        else
            soga[jugador].engusano=-1;
    }


    tvelx=tvelx/norma;
    tvely=tvely/norma;

    tx=soga[jugador].x;
    ty=soga[jugador].y;

    while (dibujando)
    {
        if (fabs(tx-gusano[jugador].x)<soga_agarre_dist && fabs(ty-gusano[jugador].y)<soga_agarre_dist)
            dibujando=FALSE;

        error++;
        if (error>1000)
        {
            dibujando=FALSE;//No encontre al gusano
        }

        tx+=tvelx;
        ty+=tvely;
        if (color==color_soga)
            color=color_soga+1;
        else
            color=color_soga;


        for (c=0;c<max_jugadores;c++)//Repersentacion en pantalla
        {
            tx2=(int)tx+cregionpj[c].x0-scroll[c].x;
            ty2=(int)ty+cregionpj[c].y0-scroll[c].y;
            if (tx2>=cregionpj[c].x0 && tx2<cregionpj[c].x1 && ty2>=cregionpj[c].y0 && ty2<cregionpj[c].y1)
                back_buffer[tx2+ty2*ancho_pantalla]=color;
        }
    }


    gx=(int)soga[jugador].x-g_ancho_bala/2;
    gy=(int)soga[jugador].y-g_alto_bala/2;
    for (c=0;c<max_jugadores;c++)//Repersentacion en pantalla
    {
            crtemp.x0=graf_soga*g_ancho_bala;
            crtemp.x1=crtemp.x0+g_ancho_bala;
            crtemp.y0=0;
            crtemp.y1=g_alto_bala;

            blt(gx+cregionpj[c].x0-scroll[c].x,gy+cregionpj[c].y0-scroll[c].y,&crtemp,&cregionpj[c],&graf_balas,&graf_back_buffer,flag_n);
    }


}



//////////////////////////////////////-> fArma <-//////////////////////////////////////////
void farma(tipo_arma *datos,int jugador)
{
    int flagx=1,c,i,esp_ranimacion,esp_rshot,esp_reload,esp_ammo,esp_cantidadbalas;
    int esp_velporcuadro,esp_sacar,esp_ini_graph,esp_multi_dir,esp_tiempo;
    float esp_retrocarga,angle,esp_desvio,esp_balavel,esp_empujebala,esp_radiobala,esp_targetvel;
    float bx,by,bdist=10,esp_variarvel,temp_vel,esp_rebote,esp_caida;

    switch (datos->tipo)
    {
        case a_shotgun:
        {
            esp_ranimacion=4;//4
            esp_multi_dir=0;
            esp_ini_graph=4;
            esp_rshot=25;//25
            esp_reload=140;//140
            esp_ammo=8;//8
            esp_cantidadbalas=6;//por disparo(esquirlas)//6
            esp_retrocarga=0.7;
            esp_desvio=0.15;//0.15
            esp_targetvel=0.03;//0.04
            esp_empujebala=25;
            esp_radiobala=4;
            esp_velporcuadro=5;//5
            esp_balavel=2;//2
            esp_variarvel=0.4;//Varia al azar entre -,+ por bala
            esp_sacar=50;//50
            esp_rebote=0;
            esp_tiempo=0;
            esp_caida=0;
            break;
        }
        case a_ak47:
        {
            esp_ranimacion=2;
            esp_multi_dir=4;
            esp_ini_graph=0;
            esp_rshot=1;//1
            esp_reload=120;//120
            esp_ammo=30;
            esp_cantidadbalas=1;
            esp_retrocarga=0.2;
            esp_desvio=0.08;//0.08
            esp_targetvel=0.05;
            esp_empujebala=30;
            esp_radiobala=4;
            esp_velporcuadro=8;//16
            esp_balavel=2;
            esp_variarvel=0.08;
            esp_sacar=100;
            esp_rebote=0;
            esp_tiempo=0;
            esp_caida=0;//0
            break;
        }
        case a_granada:
        {
            esp_ranimacion=2;//2
            esp_multi_dir=0;
            esp_ini_graph=6;
            esp_rshot=1;
            esp_reload=1;//1
            esp_ammo=1;
            esp_cantidadbalas=1;
            esp_retrocarga=0;
            esp_desvio=0;//0.08
            esp_targetvel=0.07;
            esp_empujebala=0;
            esp_radiobala=10;
            esp_velporcuadro=1;//16
            esp_balavel=3;
            esp_variarvel=0.08;
            esp_sacar=300;
            esp_rebote=3;
            esp_tiempo=70*2;
            esp_caida=gravedad;
            break;
        }
    }

    if (datos->estado==cargada && datos->ammo==0)
        datos->ammo=esp_ammo;//Primera ejecucion



    if (control[jugador][c_disparo] && datos->estado==cargada)
    {
        datos->estado=disparando;
        gusano[jugador].velx-=cos(gusano[jugador].angle)*esp_retrocarga;
        gusano[jugador].vely+=sin(gusano[jugador].angle)*esp_retrocarga;

        bx=gusano[jugador].x;
        by=gusano[jugador].y;
        advance(&gusano[jugador].angle,&bdist,&bx,&by);

        for (c=0;c<esp_cantidadbalas;c++)
        {
            for(i=0;i<max_balas;i++)
            {
                if (bala[i].vida==FALSE)
                {
                    bala[i].jugador=jugador;
                    bala[i].empuje=esp_empujebala;
                    bala[i].radio=esp_radiobala;
                    bala[i].sacar=esp_sacar;
                    bala[i].x=bx;
                    bala[i].y=by;
                    bala[i].angle=gusano[jugador].angle;
                    angle=gusano[jugador].angle+random_range(-esp_desvio,esp_desvio);
                    temp_vel=esp_balavel+random_range(-esp_variarvel,esp_variarvel);
                    bala[i].velx=cos(angle)*temp_vel;
                    bala[i].vely=-sin(angle)*temp_vel;
                    bala[i].velporcuadro=esp_velporcuadro;
                    bala[i].multi_dir=esp_multi_dir;
                    bala[i].ini_graph=esp_ini_graph;
                    bala[i].cambiar_angle=TRUE;
                    bala[i].brebote=esp_rebote;
                    bala[i].tiempo=esp_tiempo;
                    bala[i].caida=esp_caida;
                    bala[i].vida=TRUE;
                    break;
                }
            }
        }
    }

    if (datos->estado==disparando)
    {
        animar(&datos->graph,&datos->c_animacion,datos->ultimo_graph,datos->ultimo_graph+max_animacion_armas,esp_ranimacion);
        if (datos->graph>=datos->ultimo_graph+max_animacion_armas)
        {
            datos->graph-=max_animacion_armas;
            datos->estado=recargando;
            datos->reload=-esp_rshot;
            datos->ammo--;
        }

    }
/*    else
    {
        datos->ultimo_graph=datos->graph;
    }*/


    if (datos->estado==recargando)
    {
        datos->reload++;
        if (datos->reload==0)
        {
            if (datos->ammo==0)
            {
                datos->reload=-esp_reload;
                datos->ammo=esp_ammo;
            }
            else
                datos->estado=cargada;
        }
    }


//Situa el grafico bien
    if (gusano[jugador].flags)
    {
        datos->gx=gusano[jugador].gx-(g_ancho_arma-g_ancho_gusano);
        flagx=-1;

        if (datos->estado!=disparando)
        {
        //datos->c_animacion=0;
        if (pi-gusano[jugador].angle>arma_rot_cambio4al1 && pi-gusano[jugador].angle<arma_rot_cambio1al2)
            datos->graph=0;
        if (pi-gusano[jugador].angle>=arma_rot_cambio1al2 && pi-gusano[jugador].angle<=arma_rot_cambio2al3)
            datos->graph=3;
        if (pi-gusano[jugador].angle>arma_rot_cambio2al3)
            datos->graph=6;
        if (pi-gusano[jugador].angle<=arma_rot_cambio4al1)
            datos->graph=9;

        datos->ultimo_graph=datos->graph;//Para moverla mientras disparo
        }

    }
    else
    {
        datos->gx=gusano[jugador].gx;

        if (datos->estado!=disparando)
        {
        //datos->c_animacion=0;
        if (gusano[jugador].angle>arma_rot_cambio4al1 && gusano[jugador].angle<arma_rot_cambio1al2)
            datos->graph=0;
        if (gusano[jugador].angle>=arma_rot_cambio1al2 && gusano[jugador].angle<=arma_rot_cambio2al3)
            datos->graph=3;
        if (gusano[jugador].angle>arma_rot_cambio2al3)
            datos->graph=6;
        if (gusano[jugador].angle<=arma_rot_cambio4al1)
            datos->graph=9;

        datos->ultimo_graph=datos->graph;//Para moverla mientras disparo
        }
    }

    switch (gusano[jugador].graph)
    {
        case 0:
        {
            datos->gx+=desp_guscuadro_0*flagx;
            break;
        }
        case 1:
        {
            datos->gx+=desp_guscuadro_1*flagx;
            break;
        }
        case 2:
        {
            datos->gx+=desp_guscuadro_2*flagx;
            break;
        }
        case 3:
        {
            datos->gx+=desp_guscuadro_3*flagx;
            break;
        }
    }

    gusano[jugador].target_vel=esp_targetvel;
    datos->gy=gusano[jugador].gy-(g_alto_arma-g_alto_gusano)/2;
}


//////////////////////////////////////-> fResto <-///////////////////////////////////////////
void fresto(tipo_resto *datos)
{
    int j,gx,gy;
    tipo_clipregion crterreno;

    datos->cont_sangre++;
    if (datos->cont_sangre>20)
    {
        crear_particulas(p_cuagulo,1,datos->x,datos->y,datos->velx*2,datos->vely,FALSE);//SANGRE
        datos->cont_sangre=0;
    }

    datos->vely+=gravedad;

    datos->x+=datos->velx;
    datos->y+=datos->vely;

    if (datos->velx>=0)
        animar(&datos->graph,&datos->c_animacion,0,3,datos->ani_vel);
    else
        animar(&datos->graph,&datos->c_animacion,3,0,datos->ani_vel);


    crtemp.x0=datos->graph*ancho_resto+75;
    crtemp.x1=crtemp.x0+ancho_resto;
    crtemp.y0=0;
    crtemp.y1=alto_resto;

    if (datos->y<0 || datos->y>alto_terreno)
    {
        datos->y-=datos->vely;
        datos->vely=-datos->vely/3;
    }
    if (datos->x<0 || datos->x>ancho_terreno)
    {
        datos->x-=datos->velx;

        datos->velx=-datos->velx/3;
    }
    gx=(int)datos->x-ancho_resto/2;
    gy=(int)datos->y-alto_resto/2;

    if (graf_terreno.imagen[(int)datos->x+(int)datos->y*ancho_terreno]<colores_fondo)
    {
        datos->x-=datos->velx;
        datos->y-=datos->vely;

        datos->velx=datos->velx/1.5;
        datos->vely=-datos->vely/3;

        if (fabs(datos->velx)<0.01 && fabs(datos->vely)<0.01)
        {
            crterreno.x0=0;
            crterreno.x1=ancho_terreno;
            crterreno.y0=0;
            crterreno.y1=alto_terreno-piso;

            blt(gx,gy,&crtemp,&crterreno,&graf_gusano[datos->jugador],&graf_terreno,flag_n);
            datos->cont_sangre=0;
            datos->vida=FALSE;
        }
    }


    for (j=0;j<max_jugadores;j++)//Repersentacion en pantalla
    {
        blt(gx+cregionpj[j].x0-scroll[j].x,gy+cregionpj[j].y0-scroll[j].y,&crtemp,&cregionpj[j],&graf_gusano[datos->jugador],&graf_back_buffer,flag_n);
    }

}


//////////////////////////////////////-> fBala <-///////////////////////////////////////////
void fbala(tipo_bala *datos)
{
    int c,j,n;



    datos->vely+=datos->caida;
    for(c=0;c<datos->velporcuadro;c++)
    {
        datos->x+=datos->velx;
        datos->y+=datos->vely;
        if (datos->x<0 || datos->x>ancho_terreno || datos->y<0 || datos->y>alto_terreno)
        {
            if (datos->tiempo==0)
            {
                datos->vida=FALSE;
            }
            else
            {
                if (datos->brebote>0)
                {
                    datos->x-=datos->velx;
                    datos->y-=datos->vely;


                    datos->velx=-datos->velx/datos->brebote;
                    datos->vely=-datos->vely/datos->brebote;
                }
                else
                {
                    datos->velx=0;
                    datos->vely=0;
                }
            }
            break;
        }
        if (graf_terreno.imagen[(int)datos->x+(int)datos->y*ancho_terreno]<colores_fondo)
        {
            if (datos->tiempo==0)
            {
                carcomer((int)datos->x,(int)datos->y,datos->radio);
                //Agregar llamado a explosion*************************************************
                datos->vida=FALSE;
            }
            else
            {
                if (datos->brebote>0)
                {
                    datos->x-=datos->velx;
                    datos->y-=datos->vely;

                    //if (abs(datos->velx)>abs(datos->vely))
                        datos->velx=-datos->velx/datos->brebote;
                    //else
                        datos->vely=-datos->vely/datos->brebote;
                }
                else
                {
                    datos->velx=0;
                    datos->vely=0;
                }
            }
        }
        for(j=0;j<max_jugadores;j++)
        {
            if (datos->x>gusano[j].x-ancho_gusano/2 && datos->x<gusano[j].x+ancho_gusano/2)
                if (datos->y>gusano[j].y-alto_gusano/2 && datos->y<gusano[j].y+alto_gusano/2)
                {
                    if (datos->tiempo==0)
                    {
                        gusano[j].velx+=(datos->velx/100)*datos->empuje;
                        gusano[j].vely+=(datos->vely/100)*datos->empuje;
                        datos->velx=gusano[j].velx;
                        datos->vely=gusano[j].vely;//Para SANGRE
                        gusano[j].energia-=datos->sacar;
                        crear_particulas(p_cuagulo,datos->sacar/50,datos->x,datos->y,datos->velx*2,datos->vely,FALSE);//SANGRE
                        datos->vida=FALSE;
                    }
                }
        }
        if (datos->vida==FALSE)
            break;
    }
    if (datos->cambiar_angle)
    {
        datos->graph=datos->ini_graph;

        if (datos->multi_dir>0)
        {
            if (datos->angle<0)
                n=abs((int)((datos->angle-pi/(4*(datos->multi_dir-1)))*2*(datos->multi_dir-1)/pi));
            else
                n=abs((int)((datos->angle+pi/(4*(datos->multi_dir-1)))*2*(datos->multi_dir-1)/pi));
            if (n>=datos->multi_dir)
                n=abs(2*(datos->multi_dir-1)-n);

            datos->graph+=n;

            datos->flags=flag_n;
            if (datos->angle>pi/2 && datos->angle<3*pi/2)
                datos->flags=flag_x;
            if (datos->angle<0 || datos->angle>pi)
                datos->flags+=flag_y;

        }

        datos->cambiar_angle=FALSE;
    }
    if (datos->tiempo>0)
    {
        datos->tiempo--;
        if (datos->tiempo<=0)
        {
            carcomer((int)datos->x,(int)datos->y,datos->radio);
            crear_explosion(datos->x, datos->y);
            explo_volar(datos);
            datos->vida=FALSE;
        }
    }

    datos->gx=datos->x-g_ancho_bala/2;
    datos->gy=datos->y-g_alto_bala/2;


    for (j=0;j<max_jugadores;j++)//Repersentacion en pantalla
    {
            crtemp.x0=datos->graph*g_ancho_bala;
            crtemp.x1=crtemp.x0+g_ancho_bala;
            crtemp.y0=0;
            crtemp.y1=g_alto_bala;

            blt(datos->gx+cregionpj[j].x0-scroll[j].x,datos->gy+cregionpj[j].y0-scroll[j].y,&crtemp,&cregionpj[j],&graf_balas,&graf_back_buffer,datos->flags);
    }
}


//////////////////////////////////////-> Carcomer <-////////////////////////////////////////////
void carcomer(int desx,int desy,int radio)
{
    //byte cpar[radio];
    int mx,my,y,x,ctemp,ctclip;

    ctclip=ancho_terreno*alto_terreno-(ancho_terreno*piso);
    for (y=desy; y<desy+radio; y++)
    {
        for (x=desx; x<desx+radio; x++)
        {
            mx=x-desx;
            my=y-desy;
            if ((mx*mx+my*my)<radio*radio)
            {
                if (desx+mx>=0 && desx+mx<ancho_terreno)
                {
                    ctemp=(desx+mx)+(desy+my)*ancho_terreno;
                    if (ctemp>=0 && ctemp<ctclip)
                    {
                        if (graf_terreno.imagen[ctemp]<colores_fondo)
                            crear_particulas(p_tierra,1,x,y,0,-1,graf_terreno.imagen[ctemp]);

                        graf_terreno.imagen[ctemp]=graf_fondo.imagen[ctemp];
                    }
                }

                if (desx-mx>=0 && desx-mx<ancho_terreno)
                {
                    ctemp=(desx-mx)+(desy+my)*ancho_terreno;
                    if (ctemp>0 && ctemp<ctclip)
                    {
                        if (graf_terreno.imagen[ctemp]<colores_fondo)
                            crear_particulas(p_tierra,1,x,y,0,-1,graf_terreno.imagen[ctemp]);

                        graf_terreno.imagen[ctemp]=graf_fondo.imagen[ctemp];
                    }
                }

                if (desx-mx>=0 && desx-mx<ancho_terreno)
                {
                    ctemp=(desx-mx)+(desy-my)*ancho_terreno;
                    if (ctemp>0 && ctemp<ctclip)
                    {
                        if (graf_terreno.imagen[ctemp]<colores_fondo)
                            crear_particulas(p_tierra,1,x,y,0,-1,graf_terreno.imagen[ctemp]);

                        graf_terreno.imagen[ctemp]=graf_fondo.imagen[ctemp];
                    }
                }

                if (desx+mx>=0 && desx+mx<ancho_terreno)
                {
                ctemp=(desx+mx)+(desy-my)*ancho_terreno;
                    if (ctemp>0 && ctemp<ctclip)
                    {
                        if (graf_terreno.imagen[ctemp]<colores_fondo)
                            crear_particulas(p_tierra,1,x,y,0,-1,graf_terreno.imagen[ctemp]);

                        graf_terreno.imagen[ctemp]=graf_fondo.imagen[ctemp];
                    }
                }
            }
        }
    }
    //crear_particulas(p_tierra,radio,desx,desy,random_range(-1,1),random_range(0,-3));
}


//////////////////////////////////////-> fParticula <-//////////////////////////////////////////
void fparticula(tipo_particula *datos)
{
    int c,tempx,tempy;

    datos->vely+=gravedad;

    datos->x+=datos->velx;
    datos->y+=datos->vely;

    for (c=0;c<max_jugadores;c++)//Repersentacion en pantalla
    {
        tempx=(int)datos->x+cregionpj[c].x0-scroll[c].x;
        tempy=(int)datos->y+cregionpj[c].y0-scroll[c].y;
        if (tempx>=cregionpj[c].x0 && tempx<cregionpj[c].x1 && tempy>=cregionpj[c].y0 && tempy<cregionpj[c].y1)
            back_buffer[tempx+tempy*ancho_pantalla]=datos->color;
    }

    if (datos->x>0 && datos->x<ancho_terreno && datos->y>0 && datos->y<alto_terreno)
    {
        if (graf_terreno.imagen[(int)datos->x+(int)datos->y*ancho_terreno]<colores_fondo)
        {
            if (datos->tipo==p_tierra)
            {
                if (datos->choque==FALSE)
                {
                    datos->vely=-datos->vely/rebote_particula;
                    datos->choque=TRUE;
                }
                else
                    datos->vida=FALSE;
            }
            if (datos->tipo>=p_sangre)
            {
                graf_terreno.imagen[(int)datos->x+(int)datos->y*ancho_terreno]=datos->color;
                datos->vida=FALSE;
            }
        }
        else
        {
            datos->choque=FALSE;
            if (datos->tipo==p_cuagulo)
            {
                datos->vely-=gravedad/2;
                datos->c_cuagulo++;
                if (datos->c_cuagulo>cuagular)
                {
                    crear_particulas(p_sangre,1,datos->x,datos->y,datos->velx/2,datos->vely,FALSE);//SANGRE
                    datos->c_cuagulo=0;
                }
            }
        }

    }
    else
        datos->vida=FALSE;
}

void crear_explosion(float x, float y)
{
    for (int j=0; j<max_explosiones; j++)
    {
        if (explosion[j].vida == FALSE)
        {
            explosion[j].vida = TRUE;
            explosion[j].x = x;
            explosion[j].y = y;
            explosion[j].graph = 0;
            explosion[j].c_animacion = 0;
            break;
        }
    }
}

//////////////////////////////////////-> fExplosion <-//////////////////////////////////////////
void fexplosion(tipo_explosion *datos)
{
    animar(&datos->graph, &datos->c_animacion, 0, explo1_max_anim, r_ani_explo);
    if (datos->graph == (explo1_max_anim - 1))
        datos->vida = FALSE;
    for (int j=0; j<max_jugadores; j++)
    {
        crtemp.x0 = datos->graph * g_ancho_explo1;
        crtemp.x1 = crtemp.x0 + g_ancho_explo1;
        crtemp.y0 = 0;
        crtemp.y1 = g_alto_explo1;
        int dest_x = datos->x + cregionpj[j].x0 - scroll[j].x - g_alto_explo1/2;
        int dest_y = datos->y + cregionpj[j].y0 - scroll[j].y - g_alto_explo1/2;
        blt(dest_x, dest_y, &crtemp, &cregionpj[j], &graf_explosiones, &graf_back_buffer, flag_n);
    }
    
}

/**
 * Aplicar el efecto explosivo al gusano
 */
void explo_volar(tipo_bala* bala)
{
    for(int j=0; j<max_jugadores; j++)
    {
        float dx = gusano[j].gx - bala->x;
        float dy = gusano[j].gy - bala->y;
        float dist = sqrt(dx*dx + dy*dy);
        dx /= dist;
        dy /= dist;
        if (dist < 5*bala->radio)
        {
            float afectacion = bala->sacar / dist;
            gusano[j].velx += dx * afectacion/10.;
            gusano[j].vely += dy * afectacion/10.;
            if (dist <= 2*bala->radio || afectacion > bala->sacar)
                gusano[j].energia -= bala->sacar;
            else
                gusano[j].energia -= afectacion;
        }
    }
}

//////////////////////////////////-> Crear_particulas <-////////////////////////////////////////
void crear_particulas(int tipo,int cantidad,float x,float y,float velx,float vely,byte color)
{
        int i;
        int encontre=0;

        for (i=0;i<max_particulas;i++)
        {
                if (particula[i].vida==FALSE)
                {
                        particula[i].vida=TRUE;
                        encontre++;
                        if (tipo==p_tierra)
                        {
                                particula[i].x=x;
                                particula[i].y=y;
                                particula[i].vely=vely+random_range(-2,2);
                                particula[i].velx=velx+random_range(-2,2);
                                particula[i].color=color;
                                particula[i].tipo=tipo;
                                //particula[i].color=(int)random_range(colores_tierra,fin_colores_tierra);
                        }
                        if (tipo==p_sangre)
                        {
                                particula[i].x=x;
                                particula[i].y=y;
                                particula[i].vely=vely;
                                particula[i].velx=velx;
                                particula[i].color=(byte)random_range(colores_sangre,fin_colores_sangre);
                                particula[i].tipo=tipo;
                        }

                        if (tipo==p_cuagulo)
                        {
                                particula[i].x=x+random_range(-1,1);
                                particula[i].y=y+random_range(-1,1);
                                particula[i].vely=vely+random_range(-1,0.5);
                                particula[i].velx=velx+random_range(-0.5,0.5);
                                particula[i].color=(byte)random_range(colores_sangre,fin_colores_sangre);
                                particula[i].tipo=tipo;
                        }

                }
                if (encontre==cantidad)
                        break;
        }

}

//////////////////////////////////-> Generar_Terreno <-/////////////////////////////////////////

void generar_terreno()
{
    float y,altura,ondulacion_piso,ondulacion_techo,vel_piso,vel_techo;
    float rugosidad_piso,rugosidad_techo;
    int x,cavernas,c,escavando,subir_techo,subir_piso,ancho,max_ancho,max_altura,max_piso_colina;
    int v_dire_piso,v_dire_techo,i,ctclip,ctemp,colina;
    byte color;
    tipo_clipregion crbloque,crterreno;
    tipo_imagen graf_bloque_ter,graf_bloque_fon;
    byte *imagen_bloque_ter;
    byte *imagen_bloque_fon;

    imagen_bloque_ter=(byte*)calloc(ancho_bloque_ter*alto_bloque_ter,1);
    imagen_bloque_fon=(byte*)calloc(ancho_bloque_fon*alto_bloque_fon,1);

    crterreno.x0=0;
    crterreno.x1=ancho_terreno;
    crterreno.y0=0;
    crterreno.y1=alto_terreno;

    //Pongo el fondo para el comienzo en terreno
    graf_bloque_fon.imagen=imagen_bloque_fon;
    graf_bloque_fon.ancho=ancho_bloque_fon;
    graf_bloque_fon.alto=alto_bloque_fon;

    crbloque.x0=0;
    crbloque.x1=ancho_bloque_fon;
    crbloque.y0=0;
    crbloque.y1=alto_bloque_fon;

    loadpcx("fon1.pcx",imagen_bloque_fon,paleta1);

    for (c=0;c<ancho_terreno;c+=ancho_bloque_fon)
    {
        for (i=0;i<alto_terreno;i+=alto_bloque_fon)
        {
            blt(c,i,&crbloque,&crterreno,&graf_bloque_fon,&graf_terreno,flag_n);
        }
    }
    //Ya puse el fondo en terreno

    //Pongo el terreno en fondo
    graf_bloque_ter.imagen=imagen_bloque_ter;
    graf_bloque_ter.ancho=ancho_bloque_ter;
    graf_bloque_ter.alto=alto_bloque_ter;

    crbloque.x0=0;
    crbloque.x1=ancho_bloque_ter;
    crbloque.y0=0;
    crbloque.y1=alto_bloque_ter;

    loadpcx("ter1.pcx",imagen_bloque_ter,paleta1);

    for (c=0;c<ancho_terreno;c+=ancho_bloque_ter)
    {
        for (i=0;i<alto_terreno;i+=alto_bloque_ter)
        {
            blt(c,i,&crbloque,&crterreno,&graf_bloque_ter,&graf_fondo,flag_n);
        }
    }
    //Ya puse el terreno en fondo

    //Hago el terreno
    ctclip=ancho_terreno*alto_terreno-(ancho_terreno*piso);

//PROPIEDADES de la GENERACION de Terreno
    cavernas=random_range(ancho_terreno*alto_terreno/10000/2,ancho_terreno*alto_terreno/10000);//min,max


    ondulacion_piso=0.08;//0.08//acel de la ondulacion piso
    ondulacion_techo=0.03;//acel de la ondulacion techo
    rugosidad_piso=0.8;//0.8
    max_piso_colina=40;//40
    rugosidad_techo=2;
    v_dire_piso=5;//5//cambio de la direccion de la pendiente
    v_dire_techo=4;//4//mientras mas grande mas dificil de cambiar(menos ondulado)

/*
    ondulacion_piso=0.1;//acel de la ondulacion piso
    ondulacion_techo=0.07;//acel de la ondulacion techo
    rugosidad_piso=0.1;
    max_piso_colina=20;
    rugosidad_techo=0.2;
    v_dire_piso=10;//cambio de la direccion de la pendiente
    v_dire_techo=3;//mientras mas grande mas dificil de cambiar(menos ondulado)
*/

    for (c=0;c<cavernas;c++)
    {
        x=random_range(0,ancho_terreno);
        y=random_range(0,alto_terreno);
        max_ancho=random_range(60,400);
        max_altura=random_range(60,130);
        ancho=0;
        altura=0;
        vel_techo=0;
        vel_piso=0;
        colina=0;
        escavando=TRUE;
        subir_techo=TRUE;
        vel_techo=-0.4;
        subir_piso=FALSE;

        while (escavando)
        {
            x++;
            ancho++;
            colina++;

            if (colina>max_piso_colina || random_range(0,v_dire_piso)<1)
            {
                if (subir_piso==FALSE)
                    subir_piso=TRUE;
                else
                    subir_piso=FALSE;

                vel_piso=-vel_piso/random_range(1.5,4);
                colina=0;
            }

            if (random_range(0,v_dire_techo)<1)
            {
                if (subir_techo==FALSE)
                    subir_techo=TRUE;
                else
                    subir_techo=FALSE;

            }

            if (subir_piso)
                vel_piso+=random_range(-ondulacion_piso,0);
            else
                vel_piso+=random_range(0,ondulacion_piso);

            if (subir_techo)
                vel_techo+=random_range(-ondulacion_techo,0);
            else
                vel_techo+=random_range(0,ondulacion_techo);

            if (ancho>max_ancho/2)
            {
                subir_techo=FALSE;
                if (altura<7 /*|| x>ancho_terreno*/)
                    escavando=FALSE;
            else
                if (altura<6)
                    subir_techo=TRUE;
            }



            y+=vel_piso+random_range(-rugosidad_piso,rugosidad_piso);
            altura-=vel_techo+random_range(-rugosidad_techo,rugosidad_techo);

            for(i=y;i>(y-altura);i--)
            {
                color=color_transparente;

                ctemp=x+i*ancho_terreno;
                if (ctemp>=0 && ctemp<ctclip)
                {
                    if (graf_fondo.imagen[ctemp]!=color_transparente && altura>6)
                    {
                        if (y-i+3>altura)//TECHO BORDE
                        {
                            color=random_range(colores_techo+i-(y-altura),colores_techo+i-(y-altura)+1);
                        }

                        if (y-i<3)//PISO BORDE
                        {
                            color=random_range(colores_piso+(y-i),colores_piso+(y-i)+1);
                        }
                    }

                    graf_fondo.imagen[ctemp]=color;
                }
            }

        }
    }

    blt(0,0,&crterreno,&crterreno,&graf_fondo,&graf_terreno,flag_n);//BLT la tierra sobre el fondo
//RESTAURO EL FONDO
    crbloque.x0=0;
    crbloque.x1=ancho_bloque_fon;
    crbloque.y0=0;
    crbloque.y1=alto_bloque_fon;

    for (c=0;c<ancho_terreno;c+=ancho_bloque_fon)
    {
        for (i=0;i<alto_terreno;i+=alto_bloque_fon)
        {
            blt(c,i,&crbloque,&crterreno,&graf_bloque_fon,&graf_fondo,flag_n);
        }
    }

    free(imagen_bloque_ter);
    free(imagen_bloque_fon);

}

void fbarra_energia()
{
    int ancho_barra = cregionpj[0].x1 - cregionpj[0].x0 - 1;
    float pixeles_por_energia = (float)ancho_barra / energia_ini;
    for (int j=0; j<max_jugadores; j++)
    {
        if (gusano[j].energia > gusano[j].c_barra_energia)
        {
            gusano[j].c_barra_energia += 1.0/pixeles_por_energia;
        }
        if (gusano[j].energia < gusano[j].c_barra_energia && gusano[j].c_barra_energia > 0)
        {
            gusano[j].c_barra_energia  -= 1.0/pixeles_por_energia;
        }
        int llenado = gusano[j].c_barra_energia * pixeles_por_energia;
        int x = cregionpj[j].x0;
        int y = cregionpj[j].y1;
        draw_rectangle(x, y, cregionpj[j].x1, y+alto_tablero, color_negro);
        y += 3;
        if (llenado > 0) draw_rectangle(x, y, x + llenado, y + alto_barra_energia-1, color_barra_energia);
    }
}

int imin(int a, int b)
{
    if (a<b) return a; else return b;
}

int imax(int a, int b)
{
    if (a>b) return a; else return b;
}

void draw_rectangle(int start_x, int start_y, int end_x, int end_y, byte color)
{
    int x0 = imax(imin(start_x, end_x), 0);
    int y0 = imax(imin(start_y, end_y), 0);
    int x1 = imin(imax(start_x, end_x), ancho_pantalla-1);
    int y1 = imin(imax(start_y, end_y), alto_pantalla-1);
    for (int y=y0; y<=y1; y++)
    {
        for (int x=x0; x<=x1; x++)
        {
            back_buffer[y*ancho_pantalla + x] = color;
        }
    }
}
