#ifndef GESTOR_SERIES_H
#define GESTOR_SERIES_H

#include <stddef.h>
#include "TDAs/map.h"
#include "TDAs/list.h"

// --- TipoSerie: datos de cada serie ---
typedef struct {
    char *titulo;
    int temporadaActual;
    int capituloActual;
    List *capsPorTemp;    // cada elemento apunta a un int con nº de capítulos en esa temporada
} TipoSerie;

// --- GestorSeries: mantiene mapa, lista de favoritas y undoStack ---
typedef struct {
    Map  *mapa;        // título → TipoSerie*
    List *favoritas;   // lista de TipoSerie* marcadas como favoritas
    List *undoStack;   // pila de Action* (Pila implementada con lista)
} GestorSeries;

typedef enum {
    ADD_SERIE,        // Tipo Agregar Serie
    ADD_SEASON,       // Tipo Agregar Temporada
    UPDATE_PROGRESS, // Tipo Actualizar Progreso
    MARK_FAVORITE       // Tipo Marcar Favorito
} ActionType;

typedef struct {
    ActionType type;
    char *titulo;      // clave de la serie
    int prevTemp;      // para UPDATE_PROGRESS
    int prevCap;       // para UPDATE_PROGRESS
} Action;

// Crea una nueva Serie con el título dado y lista de capítulos por temporada.
// Retorna un puntero a TipoSerie o NULL si hay error de memoria.
TipoSerie* crearSerie(const char *titulo, List *capsPorTemp);

// Imprime en pantalla los datos de la serie: título, temporadaActual y capituloActual.
// También muestra el número de capítulos de cada temporada.
void gestor_imprimirSerie(const TipoSerie *s);

// Agrega una nueva temporada al final de capsPorTemp con numCaps capítulos.
void gestor_agregarTemporada(GestorSeries *g, TipoSerie *s, int numCaps);

// Actualiza el progreso de la serie a la temporada `temp` y capítulo `cap`.
void gestor_actualizarProgreso(GestorSeries *g, TipoSerie *s, int temp, int cap);

// Libera memoria asociada a la Serie:
// - libera el string de título
// - libera la lista capsPorTemp y sus enteros
// - libera la estructura TipoSerie
void gestor_LiberarYEliminarSerie(TipoSerie *s);

// Inicializa el GestorSeries:
// - crea internamente el mapa con capacidad inicial
// - crea la lista de favoritas vacía
void gestor_crearGestorSeries(GestorSeries *g, size_t capacidadInicial);

// Libera memoria del GestorSeries:
// - recorre el mapa y libera cada Serie con gestor_LiberarYEliminarSerie
// - libera la lista de favoritas
// - libera el mapa
void gestor_LiberarMemoriaMap(GestorSeries *g);

// Inserta una Serie en el mapa interno.
// Retorna 1 si se agregó correctamente, 0 si ya existía una Serie con ese título.
int gestor_agregar(GestorSeries *g, TipoSerie *s);

// Busca una Serie por título en el gestor.
// Retorna un puntero a TipoSerie si la encuentra, o NULL si no existe.
TipoSerie* gestor_buscarSerie(const GestorSeries *g, const char *titulo);

// Lista todas las Series almacenadas en el gestor.
// imprimiendo sus datos en pantalla.
void gestor_MostrarSeries(const GestorSeries *g);

// Elimina del mapa la Serie con el titulo dado.
// libera su memoria y retorna 1 si tuvo exito o 0 si no la encontró.
int gestor_eliminarYliberar(GestorSeries *g, const char *titulo);

// Marca una Serie como favorita:
// busca la Serie por titulo y, si existe y no estaba en favoritas,
// la añade a la lista de favoritas, retornando 1.
// Si no existe o ya está marcada, retorna 0.
int gestor_agregarFavorito(GestorSeries *g, const char *titulo);

// Recorre la lista de favoritas e imprime cada Serie marcada.
// Si no hay favoritas, muestra un mensaje indicándolo.
void gestor_mostrarFavoritos(const GestorSeries *g);

// Funcion Undo para deshacer la ultima accion
void gestor_undo(GestorSeries *g);



#endif // GESTOR_SERIES_H //
