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
