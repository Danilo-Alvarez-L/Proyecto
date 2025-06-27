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

TipoSerie* crearSerie(const char *titulo, List *capsPorTemp)
{
    if (!titulo || !capsPorTemp)
    {
        return NULL;
    }
    TipoSerie *nuevaSerie = malloc(sizeof(TipoSerie));
    if (!nuevaSerie)
    {
        return NULL;
    }
    nuevaSerie->titulo           = strdup(titulo);
    nuevaSerie->capsPorTemp      = capsPorTemp;
    nuevaSerie->temporadaActual  = 1;
    nuevaSerie->capituloActual   = 1;
    return nuevaSerie;
}

void gestor_imprimirSerie(const TipoSerie *serie)
{
    if (!serie)
    {
        return;
    }
    printf("Titulo: %s\n", serie->titulo);                 
    printf(" Progreso: temporada %d, capitulo %d\n", serie->temporadaActual, serie->capituloActual);
    printf(" Capitulos por temporada:\n");
    int numeroTemporada = 1;

    for (int *aux_capPorTemp = list_first(serie->capsPorTemp); aux_capPorTemp; aux_capPorTemp = list_next(serie->capsPorTemp), numeroTemporada++)
    {
        printf("   T%d: %d capitulos\n", numeroTemporada, *aux_capPorTemp);
    }
    puts("");
}

void gestor_crearGestorSeries(GestorSeries *gestor, size_t capacidadInicial)
{
    gestor->mapa = map_create((long)capacidadInicial);
    gestor->favoritas = list_create();
}

void gestor_LiberarMemoriaMap(GestorSeries *gestor)
{
    if (!gestor)
    {
        return;
    }
    MapPair *pair;

    while ((pair = map_first(gestor->mapa)) != NULL)
    {
        map_remove(gestor->mapa, pair->key);                
        gestor_LiberarYEliminarSerie((TipoSerie*)pair->value);
        free(pair->key);                                    
        free(pair);                                         
    }
    
    free(gestor->mapa->buckets);
    free(gestor->mapa);
    
    list_clean(gestor->favoritas);
    free(gestor->favoritas);
}

int gestor_agregar(GestorSeries *gestor, TipoSerie *serie)
{
    if (!gestor || !serie)
    {
        return 0;
    }
    if (map_search(gestor->mapa, serie->titulo))
    {
        return 0;
    }
    map_insert(gestor->mapa, strdup(serie->titulo), serie);
    return 1;
}


TipoSerie* gestor_buscarSerie(const GestorSeries *gestor, const char *titulo)
{
    if (!gestor || !titulo)
    {
        return NULL;
    }
    MapPair *pair = map_search(gestor->mapa, (char*)titulo);
    return pair ? (TipoSerie*)pair->value : NULL;
}


void gestor_MostrarSeries(const GestorSeries *gestor)
{
    if (!gestor)
    {
        return;
    }
    for (MapPair *pair = map_first(gestor->mapa);
         pair;
         pair = map_next(gestor->mapa))
    {
        gestor_imprimirSerie((TipoSerie*)pair->value);
    }
}


int gestor_eliminarYliberar(GestorSeries *gestor, const char *titulo)
{
    if (!gestor || !titulo)
    {
        return 0;
    }
    MapPair *pair = map_search(gestor->mapa, (char*)titulo);
    if (!pair)
    {
        return 0;
    }

    char *clave = pair->key;
    TipoSerie *serie = (TipoSerie*)pair->value;

    map_remove(gestor->mapa, clave);

    gestor_LiberarYEliminarSerie(serie);
    free(clave);

    return 1;
}

int gestor_agregarFavorito(GestorSeries *gestor, const char *titulo)
{
    if (!gestor || !titulo)
    {
        return 0;
    }
    TipoSerie *serie = gestor_buscarSerie(gestor, titulo);
    if (!serie)
    {
        return 0;
    }
    for (TipoSerie *serieFav = list_first(gestor->favoritas);
         serieFav;
         serieFav = list_next(gestor->favoritas))
    {
        if (strcmp(serieFav->titulo, titulo) == 0)
        {
            return 0;
        }
    }
    list_pushBack(gestor->favoritas, serie);
    return 1;
}

void gestor_mostrarFavoritos(const GestorSeries *gestor)
{
    if (!gestor)
    {
        return;
    }
    if (list_size(gestor->favoritas) == 0)
    {
        puts("No hay series favoritas marcadas.\n");
        return;
    }
    for (TipoSerie *serieFav = list_first(gestor->favoritas);
         serieFav;
         serieFav = list_next(gestor->favoritas))
    {
        gestor_imprimirSerie(serieFav);
    }
}