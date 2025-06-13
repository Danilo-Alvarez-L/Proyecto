#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "map.h"

typedef struct Map Map;
int enlarge_called = 0;

MapPair *createMapPair(char *key, void *value)
{
    MapPair *new = (MapPair *)malloc(sizeof(MapPair));
    new->key = key;
    new->value = value;
    return new;
}

long hash(char *key, long capacity)
{
    unsigned long hash = 0;
    char *ptr;
    for (ptr = key; *ptr != '\0'; ptr++)
    {
        hash += hash * 32 + tolower(*ptr);
    }
    return hash % capacity;
}

int is_equal(void *key1, void *key2)
{
    if (key1 == NULL || key2 == NULL)
    {
            return 0;
    }
    return strcmp((char *)key1, (char *)key2) == 0;
}

void map_insert(Map *map, char *key, void *value)
{
    long pos = hash(key, map->capacity);
    long original_pos = pos;

    while (map->buckets[pos] != NULL && map->buckets[pos]->key != NULL)
    {
        if (is_equal(map->buckets[pos]->key, key))
        {
            return;
        }
        pos = (pos + 1) % map->capacity;
        if (pos == original_pos)
        {
            return;
        }
    }

    map->current = pos;
    map->buckets[pos] = createMapPair(key, value);
    map->size++;

    if ((double)map->size / map->capacity > 0.7)
    {
        map_enlarge(map);
    }
}

void map_enlarge(Map *map)
{
    enlarge_called = 1;

    MapPair **old_buckets = map->buckets;
    long old_capacity = map->capacity;

    map->capacity *= 2;
    map->buckets = (MapPair **)calloc(map->capacity, sizeof(MapPair *));
    map->size = 0;

    for (long i = 0; i < old_capacity; i++)
    {
        if (old_buckets[i] != NULL && old_buckets[i]->key != NULL)
        {
            map_insert(map, old_buckets[i]->key, old_buckets[i]->value);
        }
    }

    free(old_buckets);
}

Map *map_create(long capacity)
{
    Map *map = (Map *)malloc(sizeof(Map));
    map->buckets = (MapPair **)calloc(capacity, sizeof(MapPair *));
    map->size = 0;
    map->current = -1;
    map->capacity = capacity;
    return map;
}

void map_remove(Map *map, char *key)
{
    MapPair *pair = map_search(map, key);
    if (pair == NULL)
    {
        return;
    }
    pair->key = NULL;
    map->size--;
}

MapPair *map_search(Map *map, char *key)
{
    long pos = hash(key, map->capacity);
    long original_pos = pos;

    while (map->buckets[pos] != NULL)
    {
        if (map->buckets[pos]->key != NULL && is_equal(map->buckets[pos]->key, key))
        {
            map->current = pos;
            return map->buckets[pos];
        }
        pos = (pos + 1) % map->capacity;
        if (pos == original_pos)
        {
            return NULL;
        }
    }

    return NULL;
}

MapPair *map_first(Map *map) {
    for (long i = 0; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

MapPair *map_next(Map *map)
{
    for (long i = map->current + 1; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}