#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "TDAs/extra.h"
#include "gestor_series.h"

#define MAX_TITLE_LEN 100

static void mostrarMenuPrincipal()
{
    limpiarPantalla();
    puts("========================================");
    puts("         Gestor de Series");
    puts("========================================");
    puts("1) Ingresar serie");
    puts("2) Mostrar series pendientes");
    puts("3) Buscar serie");
    puts("4) Actualizar pendiente");
    puts("5) Eliminar serie");
    puts("6) Marcar favorita");
    puts("7) Mostrar favoritas");
    puts("8) Salir");
    puts("9) Deshacer ultima accion");
    printf("Seleccione una opcion: ");
}

static void mostrarMenuActualizar()
{
    puts("----------------------------------------");
    puts("    Actualizar pendiente");
    puts("----------------------------------------");
    puts("a) Agregar temporada");
    puts("b) Avanzar capitulo");
    printf("Seleccione una opcion: ");
}

int main()
{
    GestorSeries gestor;
    char opcion;
    char buffer[MAX_TITLE_LEN];
    int _c;

    // Inicializa el gestor
    gestor_crearGestorSeries(&gestor, 10);

    do {
        mostrarMenuPrincipal();
        scanf(" %c", &opcion);
        while ((_c = getchar()) != '\n' && _c != EOF) { }

        switch (opcion)
        {
            case '1':
            {
                leerTitulo("Titulo de la serie: ", buffer, MAX_TITLE_LEN);

                // 2) Verificación temprana:
                if (gestor_buscarSerie(&gestor, buffer)) {
                    printf("\nLa serie \"%s\" ya existe.\n", buffer);
                    presioneTeclaParaContinuar();
                    break;
                }

                // 3) Sólo si no existe, pedimos datos
                List *capsPorTemp = list_create();
                int numTemp = leerEnteroPositivo("Numero de temporadas: ");
                for (int i = 1; i <= numTemp; i++)
                {
                    char mensaje[64];
                    snprintf(mensaje, sizeof(mensaje), "  Caps. temporada %d: ", i);
                    int caps = leerEnteroPositivo(mensaje);
                    int *numCapsPtr = malloc(sizeof(int));
                    *numCapsPtr = caps;
                    list_pushBack(capsPorTemp, numCapsPtr);
                }
                TipoSerie *serie = crearSerie(buffer, capsPorTemp);
                gestor_agregar(&gestor, serie);
                printf("\nSerie \"%s\" agregada correctamente.\n", buffer);
                presioneTeclaParaContinuar();
                break;
            }

            case '2':  // Mostrar series pendientes
                limpiarPantalla();
                puts("== Series pendientes ==");
                gestor_MostrarSeries(&gestor);
                presioneTeclaParaContinuar();
                break;

            case '3':
            {  // Buscar serie
                leerTitulo("Titulo de la serie: ", buffer, MAX_TITLE_LEN);

                TipoSerie *s = gestor_buscarSerie(&gestor, buffer);
                if (s)
                {
                    gestor_imprimirSerie(s);
                }
                else
                {
                    printf("No se encontro la serie \"%s\".\n", buffer);
                }
                presioneTeclaParaContinuar();
                break;
            }

            case '4':
            {
                mostrarMenuActualizar();
                char subop;
                scanf(" %c", &subop);
                while ((_c = getchar()) != '\n' && _c != EOF) { }

                if (subop == 'a')
                {
                    // 1) Primero pedimos el título y verificamos existencia
                    leerTitulo("Titulo de la serie: ", buffer, MAX_TITLE_LEN);
                    TipoSerie *s = gestor_buscarSerie(&gestor, buffer);
                    if (!s)
                    {
                        printf("\nLa serie \"%s\" no existe.\n", buffer);
                        presioneTeclaParaContinuar();
                        break;
                    }

                    // 2) Sólo si existe, pedimos el número de capítulos
                    int caps = leerEnteroPositivo("Número de capítulos de la nueva temporada: ");
                    gestor_agregarTemporada(&gestor, s, caps);
                    printf("Temporada agregada a \"%s\".\n", buffer);
                    presioneTeclaParaContinuar();
                }
                else if (subop == 'b')
                {
                    // 1) Primero pedimos el título y verificamos existencia
                    leerTitulo("Titulo de la serie: ", buffer, MAX_TITLE_LEN);
                    TipoSerie *s = gestor_buscarSerie(&gestor, buffer);
                    if (!s)
                    {
                        printf("\nLa serie \"%s\" no existe.\n", buffer);
                        presioneTeclaParaContinuar();
                        break;
                    }

                    // 2) Sólo si existe, pedimos temporada y capítulo
                    int temp = leerEnteroPositivo("Temporada (actual): ");
                    int cap  = leerEnteroPositivo("Capítulo (actual): ");

                    if (gestor_actualizarProgreso(&gestor, s, temp, cap))
                        printf("Progreso actualizado en \"%s\".\n", buffer);
                    // si falla, gestor_actualizarProgreso ya imprimió el error

                    presioneTeclaParaContinuar();
                }
                else
                {
                    printf("Opción no válida.\n");
                    presioneTeclaParaContinuar();
                }
                break;
            }

            case '5':
            {  // Eliminar serie
                leerTitulo("Titulo de la serie: ", buffer, MAX_TITLE_LEN);

                if (gestor_eliminarYliberar(&gestor, buffer))
                {
                    printf("Serie \"%s\" eliminada.\n", buffer);
                }
                else
                {
                    printf("No se encontro la serie \"%s\".\n", buffer);
                }
                presioneTeclaParaContinuar();
                break;
            }

            case '6':
            {  // Marcar favorita
                leerTitulo("Titulo de la serie a marcar favorita: ", buffer, MAX_TITLE_LEN);

                if (gestor_agregarFavorito(&gestor, buffer))
                {
                    printf("\"%s\" ha sido marcada como favorita.\n", buffer);
                }
                else
                {
                    printf("No se pudo marcar \"%s\".\n", buffer);
                }
                presioneTeclaParaContinuar();
                break;
            }

            case '7':  // Mostrar favoritas
                limpiarPantalla();
                puts("== Series favoritas ==");
                gestor_mostrarFavoritos(&gestor);
                presioneTeclaParaContinuar();
                break;

            case '8':  // Salir
                puts("Saliendo... ¡Gracias por usar Gestor de Series!");
                break;
            
            case '9': //UNDO
                gestor_undo(&gestor);
                presioneTeclaParaContinuar();
                break;

            default:
                printf("Opcion no valida.\n");
                presioneTeclaParaContinuar();
        }
    } while (opcion != '8');

    // Libera toda la memoria del gestor
    gestor_LiberarMemoriaMap(&gestor);
    return 0;
}