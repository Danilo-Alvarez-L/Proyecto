TipoSerie* crearSerie(const char *titulo, List *capsPorTemp)
{
    if (!titulo || !capsPorTemp)
    {
        return NULL;
    }
    TipoSerie *nuevaSerie = malloc(sizeof(TipoSerie));
    if (!nuevaSerie)
    {
        return NULL;
    }
    nuevaSerie->titulo           = strdup(titulo);
    nuevaSerie->capsPorTemp      = capsPorTemp;
    nuevaSerie->temporadaActual  = 1;
    nuevaSerie->capituloActual   = 1;
    return nuevaSerie;
}

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

    for (int *aux_capPorTemp = list_first(serie->capsPorTemp); aux_capPorTemp; aux_capPorTemp = list_next(serie->capsPorTemp), numeroTemporada++)
    {
        printf("   T%d: %d capitulos\n", numeroTemporada, *aux_capPorTemp);
    }
    puts("");
}

void gestor_crearGestorSeries(GestorSeries *gestor, size_t capacidadInicial)
{
    gestor->mapa = map_create((long)capacidadInicial);
    gestor->favoritas = list_create();
}