#ifndef MAP_H
#define MAP_H

typedef struct Map Map;

typedef struct MapPair
{
    char * key;
    void * value;
} MapPair;

typedef struct Map
{
    MapPair **buckets;
    long size;     
    long capacity; 
    long current;  
} Map;

Map * map_create(long capacity);

void map_insert(Map * table, char * key, void * value);

void map_remove(Map * table, char * key);

MapPair * map_search(Map * table, char * key);

MapPair * map_first(Map * table);

MapPair * map_next(Map * table);

void map_enlarge(Map * map);

#endif /* MAP_H */
