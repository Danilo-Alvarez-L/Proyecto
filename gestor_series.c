#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestor_series.h"


void gestor_agregarTemporada(TipoSerie *serie, int numCaps)
{
    if (!serie || numCaps <= 0)
    {
        return;
    }
    int *nuevoCaps = malloc(sizeof(int));                  
    *nuevoCaps = numCaps;                                  
    list_pushBack(serie->capsPorTemp, nuevoCaps);          
}

void gestor_actualizarProgreso(TipoSerie *serie, int temporada, int capitulo)
{
    if (!serie)
    {
        return;
    }
    int totalTemporadas = list_size(serie->capsPorTemp);
    if (temporada < 1 || temporada > totalTemporadas)
    {
        printf("  Error: temporada invalida. Debe estar entre 1 y %d.\n", totalTemporadas);
        return;
    }
    int indiceTemporada = 1;
    for (int *aux_capPorTemp = list_first(serie->capsPorTemp); aux_capPorTemp; aux_capPorTemp = list_next(serie->capsPorTemp), indiceTemporada++)
    {
        if (indiceTemporada == temporada)
        {
            if (capitulo < 1 || capitulo > *aux_capPorTemp)
            {
                printf("  Error: capítulo inválido. Debe estar entre 1 y %d.\n", *aux_capPorTemp);
                return;
            }
            serie->temporadaActual = temporada;
            serie->capituloActual  = capitulo;
            return;
        }
    }
}