#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//git add .             # Añade todos los cambios al área de preparación.
//git commit -m "boceto funcion 1"  # Realiza un commit con un mensaje descriptivo.
//git push origin main
//git pull: Sincroniza tu rama local con la rama remota.

typedef struct Pair {
    char* key;
    void* value;
    int is_active; // Para controlar elementos válidos (si es open addressing)
} Pair;

typedef struct HashMap {
    Pair **buckets;
    long capacity;
} HashMap;

void printHashMap(HashMap *map) {
    if (map == NULL) return;

    for (long i = 0; i < map->capacity; i++) {
        Pair *par = map->buckets[i];

        if (par != NULL && par->is_active) {
            printf("Bucket %ld -> Clave: %s | Valor: %s\n", i, par->key, (char *)par->value);
        }
    }
}

int main (){
    printf("hola soy jm");
}
