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

int main() {
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
            {  // Ingresar serie
                int numTemp;
                List *capsPorTemp = list_create();

                printf("Titulo de la serie: ");
                fgets(buffer, MAX_TITLE_LEN, stdin);
                buffer[strcspn(buffer, "\r\n")] = '\0';

                printf("Numero de temporadas: ");
                scanf("%d", &numTemp);
                while ((_c = getchar()) != '\n' && _c != EOF) { }

                for (int i = 1; i <= numTemp; i++)
                {
                    int caps;
                    printf("  Caps. temporada %d: ", i);
                    scanf("%d", &caps);
                    while ((_c = getchar()) != '\n' && _c != EOF) { }

                    int *pc = malloc(sizeof(int));
                    *pc = caps;
                    list_pushBack(capsPorTemp, pc);
                }

                TipoSerie *serie = crearSerie(buffer, capsPorTemp);
                if (gestor_agregar(&gestor, serie))
                {
                    printf("\nSerie \"%s\" agregada correctamente.\n", buffer);
                }
                else
                {
                    printf("\nLa serie \"%s\" ya existe.\n", buffer);
                    gestor_LiberarYEliminarSerie(serie);
                }
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
                printf("Titulo de la serie a buscar: ");
                fgets(buffer, MAX_TITLE_LEN, stdin);
                buffer[strcspn(buffer, "\r\n")] = '\0';

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
            {  // Actualizar pendiente
                mostrarMenuActualizar();
                char subop;
                scanf(" %c", &subop);
                while ((_c = getchar()) != '\n' && _c != EOF) { }

                if (subop == 'a')
                {
                    printf("Titulo de la serie: ");
                    fgets(buffer, MAX_TITLE_LEN, stdin);
                    buffer[strcspn(buffer, "\r\n")] = '\0';

                    int caps;
                    printf("Numero de capitulos de la nueva temporada: ");
                    scanf("%d", &caps);
                    while ((_c = getchar()) != '\n' && _c != EOF) { }

                    TipoSerie *s = gestor_buscarSerie(&gestor, buffer);
                    if (s)
                    {
                        gestor_agregarTemporada(&gestor, s, caps);
                        printf("Temporada agregada a \"%s\".\n", buffer);
                    }
                    else
                    {
                        printf("Serie \"%s\" no encontrada.\n", buffer);
                    }
                }
                else if (subop == 'b')
                {
                    printf("Titulo de la serie: ");
                    fgets(buffer, MAX_TITLE_LEN, stdin);
                    buffer[strcspn(buffer, "\r\n")] = '\0';

                    int temp, cap;
                    printf("Temporada (actual): ");
                    scanf("%d", &temp);
                    while ((_c = getchar()) != '\n' && _c != EOF) { }
                    printf("Capitulo (actual): ");
                    scanf("%d", &cap);
                    while ((_c = getchar()) != '\n' && _c != EOF) { }

                    TipoSerie *s = gestor_buscarSerie(&gestor, buffer);
                    if (s)
                    {
                        gestor_actualizarProgreso(&gestor, s, temp, cap);
                        printf("Progreso actualizado en \"%s\".\n", buffer);
                    }
                    else
                    {
                        printf("Serie \"%s\" no encontrada.\n", buffer);
                    }
                }
                else
                {
                    printf("Opcion no valida.\n");
                }
                presioneTeclaParaContinuar();
                break;
            }

            case '5':
            {  // Eliminar serie
                printf("Titulo de la serie a eliminar: ");
                fgets(buffer, MAX_TITLE_LEN, stdin);
                buffer[strcspn(buffer, "\r\n")] = '\0';

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
                printf("Titulo de la serie a marcar favorita: ");
                fgets(buffer, MAX_TITLE_LEN, stdin);
                buffer[strcspn(buffer, "\r\n")] = '\0';

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