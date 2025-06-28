#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestor_series.h"

// -----------------------------------------------------------------------------
// Funciones relacionadas con TipoSerie
// -----------------------------------------------------------------------------

/**
 * Crea una nueva serie.
 * - Duplica el título (reserva memoria para la cadena).
 * - Asigna la lista de capitulos por temporada.
 * - Inicializa el progreso en temporada 1, capítulo 1.
 */
TipoSerie* crearSerie(const char *titulo, List *capsPorTemp)
{
    if (!titulo || !capsPorTemp)
    {
        return NULL;
    }
    TipoSerie *nuevaSerie = malloc(sizeof(TipoSerie));     // Reservar estructura
    if (!nuevaSerie)
    {
        return NULL;
    }
    nuevaSerie->titulo           = strdup(titulo);         // Copiar titulo
    nuevaSerie->capsPorTemp      = capsPorTemp;            // Asignar lista de capitulos
    nuevaSerie->temporadaActual  = 1;                      // Inicializar temporada
    nuevaSerie->capituloActual   = 1;                      // Inicializar capítulo
    return nuevaSerie;                                     // retornar la nueva serie
}

/**
 * Imprime en pantalla los datos de la serie:
 * - Titulo
 * - Progreso (temporadaActual / capituloActual)
 * - Lista de capítulos por temporada
 */
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
    // Recorre cada cap en la lista de capotulos
    for (int *aux_capPorTemp = list_first(serie->capsPorTemp); aux_capPorTemp; aux_capPorTemp = list_next(serie->capsPorTemp), numeroTemporada++)
    {
        printf("   T%d: %d capitulos\n", numeroTemporada, *aux_capPorTemp);
    }
    puts("");
}

/**
 * Agrega una nueva temporada al final de la lista:
 * - Reserva un int para numero de capitulos y lo añade a la lista.
 */
void gestor_agregarTemporada(GestorSeries *gestor, TipoSerie *serie, int numCaps)
{
    if (!gestor || !serie || numCaps <= 0)
    {
        return;
    }
    // 1) Apilar la acción inversa (quitar temporada)
    Action *act = malloc(sizeof *act);
    act->type   = ADD_SEASON;
    act->titulo = strdup(serie->titulo);
    list_pushFront(gestor->undoStack, act);

    // 2) Ejecutar realmente la operación
    int *nuevoCaps = malloc(sizeof *nuevoCaps);
    *nuevoCaps     = numCaps;
    list_pushBack(serie->capsPorTemp, nuevoCaps);         
}

/**
 * Actualiza el progreso de la serie:
 * - Verifica que la temporada y capitulo sea valido.
 * - Si es valido, actualiza temporadaActual y capituloActual.
 */
int gestor_actualizarProgreso(GestorSeries *gestor, TipoSerie *serie, int temporada, int capitulo)
{
    if (!gestor || !serie)
    {
        return 0;
    }

    // 1) Comprobar rango de temporada
    int totalTemp = list_size(serie->capsPorTemp);
    if (temporada < 1 || temporada > totalTemp)
    {
        printf("Error: temporada invalida (1–%d)\n", totalTemp);
        return 0;
    }

    // 2) Extraer nº de capítulos de la temporada solicitada
    //    list_first/list_next devuelven punteros a int con cada count
    int *ptrCaps = list_first(serie->capsPorTemp);
    for (int i = 1; i < temporada; ++i)
    {
        ptrCaps = list_next(serie->capsPorTemp);
    }
    int maxCaps = *ptrCaps;

    // 3) Comprobar rango de capítulo dentro de esa temporada
    if (capitulo < 1 || capitulo > maxCaps)
    {
        printf("Error: capítulo inválido para la temporada %d (1–%d)\n", temporada, maxCaps);
        return 0;
    }

    // 4) Apilar estado anterior y actualizar
    Action *act = malloc(sizeof *act);
    act->type     = UPDATE_PROGRESS;
    act->titulo   = strdup(serie->titulo);
    act->prevTemp = serie->temporadaActual;
    act->prevCap  = serie->capituloActual;
    list_pushFront(gestor->undoStack, act);

    serie->temporadaActual = temporada;
    serie->capituloActual  = capitulo;
    return 1;
}


/**
 * Libera toda la memoria asociada a una serie:
 * - free del titulo
 * - free de cada entero en capsPorTemp
 * - limpia y libera la lista capsPorTemp
 * - free de la propia estructura TipoSerie
 */
void gestor_LiberarYEliminarSerie(TipoSerie *serie)
{
    if (!serie)
    {
        return;
    }
    free(serie->titulo);                                    // Liberar cadena título
    // Liberar cada entero en la lista de capítulos
    int *aux_capPorTemp;
    while ((aux_capPorTemp = list_popFront(serie->capsPorTemp)) != NULL)
    {
        free(aux_capPorTemp);
    }
    list_clean(serie->capsPorTemp);                         // Limpiar lista
    free(serie->capsPorTemp);                               // Liberar estructura List
    free(serie);                                            // Liberar struct TipoSerie
}

// -----------------------------------------------------------------------------
// Funciones relacionadas con GestorSeries
// -----------------------------------------------------------------------------

/**
 * Inicializa un GestorSeries:
 * - Crea el hashmap con capacidadInicial buckets.
 * - Crea la lista vacía de favoritas.
 * - Crea el stack de undo
 */
void gestor_crearGestorSeries(GestorSeries *gestor, size_t capacidadInicial)
{
    gestor->mapa = map_create((long)capacidadInicial);
    gestor->favoritas = list_create();
    gestor->undoStack = list_create();
}

/**
 * Libera toda la memoria del GestorSeries:
 * - Recorre el mapa: quita cada entrada, libera la serie y la clave.
 * - Libera buckets y estructura del mapa.
 * - Limpia y libera la lista de favoritas (sin liberar las series otra vez).
 */
void gestor_LiberarMemoriaMap(GestorSeries *gestor)
{
    if (!gestor)
    {
        return;
    }
    MapPair *pair;
    // Mientras haya elementos en el mapa, extrae y libera cada uno.
    while ((pair = map_first(gestor->mapa)) != NULL)
    {
        map_remove(gestor->mapa, pair->key);                // Quitar entrada
        gestor_LiberarYEliminarSerie((TipoSerie*)pair->value);// Liberar serie
        free(pair->key);                                    // Liberar clave
        free(pair);                                         // Liberar MapPair
    }
    // Liberar buckets y estructura Map
    free(gestor->mapa->buckets);
    free(gestor->mapa);
    // Limpiar y liberar lista de favoritas
    list_clean(gestor->favoritas);
    free(gestor->favoritas);
    // limpiar pila de undo aunque no suele haber restos
    Action *accion;
    while((accion = list_popFront(gestor->undoStack)))
    {
        free(accion->titulo);
        free(accion);
    }
    list_clean(gestor->undoStack);
    free(gestor->undoStack);
}

/**
 * Agrega una serie al gestor:
 * - Si ya existe, retorna 0.
 * - Si no, inserta copia de la clave y el valor, retorna 1.
 */
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
    // Apilar undo
    Action *accion = malloc(sizeof *accion);
    accion->type   = ADD_SERIE;
    accion->titulo = strdup(serie->titulo);
    list_pushFront(gestor->undoStack, accion);
    return 1;
}

/**
 * Busca una serie por titulo:
 * - Retorna el puntero a TipoSerie si la encuentra, o NULL.
 */
TipoSerie* gestor_buscarSerie(const GestorSeries *gestor, const char *titulo)
{
    if (!gestor || !titulo)
    {
        return NULL;
    }
    MapPair *pair = map_search(gestor->mapa, (char*)titulo);
    return pair ? (TipoSerie*)pair->value : NULL;
}

/**
 * Lista todas las series del gestor:
 * - Itera todas las entradas del mapa e imprime cada serie.
 */
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

/**
 * Elimina una serie del gestor:
 * - Quita la entrada del mapa, libera serie y clave, retorna 1;
 * - Si no existe, retorna 0.
 */
int gestor_eliminarYliberar(GestorSeries *gestor, const char *titulo)
{
    if (!gestor || !titulo)
    {
        return 0;
    }
    // 1) buscar la entrada
    MapPair *pair = map_search(gestor->mapa, (char*)titulo);
    if (!pair)
    {
        return 0;
    }

    // 2) guardar clave y valor para liberar
    char *clave = pair->key;
    TipoSerie *serie = (TipoSerie*)pair->value;

    // 3) eliminar de la tabla (marca pair->key = NULL), pero NO libera pair
    map_remove(gestor->mapa, clave);

    // 4) liberar la memoria de la serie y de la cadena de la clave
    gestor_LiberarYEliminarSerie(serie);
    free(clave);

    return 1;
}

/**
 * Marca una serie como favorita:
 * - Si no existe o ya está marcada, retorna 0.
 * - Si es nueva favorita, la añade y retorna 1.
 */
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
    // Apilar undo
    Action *accion = malloc(sizeof *accion);
    accion->type   = MARK_FAVORITE;
    accion->titulo = strdup(titulo);
    list_pushFront(gestor->undoStack, accion);
    return 1;
}

/**
 * Muestra todas las series favoritas:
 * - Si la lista está vacía, imprime mensaje indicándolo.
 */
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

void gestor_undo(GestorSeries *gestor)
{
    Action *accion = list_popFront(gestor->undoStack);
    if (!accion)
    {
        puts("Nada que deshacer.");
        return;
    }
    switch(accion->type)
    {
      case ADD_SERIE:
        gestor_eliminarYliberar(gestor, accion->titulo);
        break;
      case ADD_SEASON:
      {
        TipoSerie *s = gestor_buscarSerie(gestor,accion->titulo);
        if (s) list_popBack(s->capsPorTemp);
        break;
      }
      case UPDATE_PROGRESS:
      {

        TipoSerie *s = gestor_buscarSerie(gestor, accion->titulo);
        if (s)
        {
            // Restauramos directamente sin apilar nada
            s->temporadaActual = accion->prevTemp;
            s->capituloActual  = accion->prevCap;
        }
        break;
    }
      case MARK_FAVORITE:
        for(TipoSerie *sf=list_first(gestor->favoritas);
            sf;
            sf=list_next(gestor->favoritas))
        {
          if(strcmp(sf->titulo, accion->titulo)==0) {
            list_popCurrent(gestor->favoritas);
            break;
          }
        }
        break;
    }
    free(accion->titulo);
    free(accion);
}
