#include "comunes.h"
#include "snails.h"
#include <SDL/SDL.h>

void configurar_teclas(int num_jugadores, int num_controles)
{
    int contj, contc, ccapturado, c;
    char * nom_controles[num_controles];
    byte conf_control[num_jugadores][num_controles];
    FILE *archivo;
    byte* keyboard = SDL_GetKeyState(NULL);

    nom_controles[c_arriba]="UP";
    nom_controles[c_abajo]="DOWN";
    nom_controles[c_izquierda]="LEFT";
    nom_controles[c_derecha]="RIGHT";
    nom_controles[c_disparo]="FIRE";
    nom_controles[c_salto]="JUMP";
    nom_controles[c_cambio]="CHANGE";

    for (contj=0; contj<num_jugadores; contj++)
    {
        printf("\nKeyboard Configuration Program V1.0 - Copyright 2002\n");

        printf("PLAYER [%i]:\n\n", contj+1);

        for (contc=0; contc<num_controles; contc++)
        {
            printf("PRESS KEY [%s]\n", nom_controles[contc]);
            ccapturado = -1;
            while (ccapturado == -1)
            {
                SDL_PumpEvents();
                printf("Enter=%d\n", keyboard[SDLK_RETURN]);
                for (c=0; c<128; c++)
                {
                    if(keyboard[c]) ccapturado=c;
                }
            }
            conf_control[contj][contc]=ccapturado;

            // esperar a que suelte la tecla
            while (keyboard[ccapturado])
                SDL_PumpEvents();
            printf("Tecla registrada");
        }
    }
    archivo = fopen("keys.cfg", "wb");
    fwrite(&conf_control, sizeof(conf_control), 1, archivo);
    fclose(archivo);
}
