#include "extra.h"
#include <errno.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 4096
#define MAX_FIELDS      128

char **leer_linea_csv(FILE *archivo, char separador) {
    static char linea[MAX_LINE_LENGTH];
    static char *campos[MAX_FIELDS];
    int idx = 0;

    if (fgets(linea, MAX_LINE_LENGTH, archivo) == NULL)
        return NULL;  // fin de fichero

    // quitar salto de línea
    linea[strcspn(linea, "\r\n")] = '\0';

    char *ptr = linea;
    while (*ptr && idx < MAX_FIELDS - 1) {
        char *start;

        if (*ptr == '\"') {
            // campo entrecomillado
            ptr++;              // saltar la comilla inicial
            start = ptr;

            // compactar contenido: convertir "" → " y copiar el resto
            char *dest = ptr;
            while (*ptr) {
                if (*ptr == '\"' && *(ptr + 1) == '\"') {
                    *dest++ = '\"';  // una comilla literal
                    ptr += 2;        // saltar ambas
                }
                else if (*ptr == '\"') {
                    ptr++;           // fin del campo
                    break;
                }
                else {
                    *dest++ = *ptr++;
                }
            }
            *dest = '\0';        // terminar cadena

            // ahora ptr apunta justo después de la comilla de cierre
            if (*ptr == separador) ptr++;
        }
        else {
            // campo sin comillas
            start = ptr;
            while (*ptr && *ptr != separador)
                ptr++;
            if (*ptr == separador) {
                *ptr = '\0';
                ptr++;
            }
        }

        campos[idx++] = start;
    }

    campos[idx] = NULL;
    return campos;
}


List *split_string(const char *str, const char *delim)
{
    List *result = list_create();
    char *token = strtok((char *)str, delim);

    while (token != NULL)
    {
        // Eliminar espacios en blanco al inicio del token
        while (*token == ' ')
        {
            token++;
        }

    // Eliminar espacios en blanco al final del token
    char *end = token + strlen(token) - 1;
    while (*end == ' ' && end > token) {
      *end = '\0';
      end--;
    }

    // Copiar el token en un nuevo string
    char *new_token = strdup(token);

    // Agregar el nuevo string a la lista
    list_pushBack(result, new_token);

    // Obtener el siguiente token
    token = strtok(NULL, delim);
  }

  return result;
}

// Función para limpiar la pantalla
void limpiarPantalla()
{
    system("cls");
}

void presioneTeclaParaContinuar()
{
    puts("Presione una tecla para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int leerEnteroPositivo(const char *mensaje)
{
    char buffer[128];
    char *ptrFinal;
    long numero;

    while (1)
    {
        printf("%s", mensaje);
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            // Si hay EOF o error de I/O, limpiamos y reintentamos
            clearerr(stdin);
            continue;
        }
        // Quitamos el salto de línea final
        buffer[strcspn(buffer, "\r\n")] = '\0';

        errno = 0;
        numero = strtol(buffer, &ptrFinal, 10);

        // Validamos rango, formato y positividad
        if (errno == ERANGE ||
            ptrFinal == buffer ||
            *ptrFinal != '\0' ||
            numero <= 0)
        {
            printf("Entrada invalida. Por favor, ingrese un numero entero positivo mayor que cero.\n");
        }
        else
        {
            return (int)numero;
        }
    }
}

static char *trimWhitespace(char *str)
{
    char *end;
    // Quitar espacios al inicio
    while (isspace((unsigned char)*str)) str++;
    if (*str == '\0') return str;
    // Quitar espacios al final
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

void leerTitulo(const char *mensaje, char *buffer, size_t size)
{
    char *trimmed;
    while (1)
    {
        printf("%s", mensaje);
        if (!fgets(buffer, size, stdin))
        {
            clearerr(stdin);
            continue;
        }
        buffer[strcspn(buffer, "\r\n")] = '\0';
        trimmed = trimWhitespace(buffer);
        if (trimmed[0] == '\0') {
            printf("Entrada invalida. El titulo no puede estar vacio.\n");
        }
        else
        {
            // Mover trimmed al inicio de buffer
            memmove(buffer, trimmed, strlen(trimmed) + 1);
            break;
        }
    }
}