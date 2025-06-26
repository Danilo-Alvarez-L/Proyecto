#ifndef GESTOR_SERIES_H
#define GESTOR_SERIES_H

#include <stddef.h>
#include "TDAs/map.h"
#include "TDAs/list.h"

typedef struct {
    char *titulo;
    int temporadaActual;
    int capituloActual;
    List *capsPorTemp;    
} TipoSerie;

TipoSerie* crearSerie(const char *titulo, List *capsPorTemp);

void gestor_imprimirSerie(const TipoSerie *s);

void gestor_agregarTemporada(TipoSerie *s, int numCaps);

void gestor_actualizarProgreso(TipoSerie *s, int temp, int cap);

void gestor_LiberarYEliminarSerie(TipoSerie *s);

typedef struct {
    Map  *mapa;        
    List *favoritas;   
} GestorSeries;

void gestor_crearGestorSeries(GestorSeries *g, size_t capacidadInicial);

void gestor_LiberarMemoriaMap(GestorSeries *g);

int gestor_agregar(GestorSeries *g, TipoSerie *s);

TipoSerie* gestor_buscar(const GestorSeries *g, const char *titulo);

void gestor_MostrarSeries(const GestorSeries *g);

int gestor_eliminarYliberar(GestorSeries *g, const char *titulo);

int gestor_agregarFavorito(GestorSeries *g, const char *titulo);

void gestor_mostrarFavoritos(const GestorSeries *g);

#endif
