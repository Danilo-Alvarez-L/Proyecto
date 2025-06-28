# Gestor de Series

## Descripción

**Gestor de Series** es una aplicación de consola escrita en C para llevar el control de tus series de TV o streaming. Permite:

- Registrar nuevas series indicando número de temporadas y capítulos por temporada.  
- Mostrar todas las series pendientes.  
- Buscar los datos de una serie por su título.  
- Actualizar el progreso:
  - **Agregar temporada** con un número de capítulos dado.  
  - **Avanzar capítulo** en una temporada concreta.  
- Eliminar series.  
- Marcar series como favoritas y listarlas.  
- **Deshacer (undo)** la última operación válida.  
- Validaciones robustas de entrada (títulos no vacíos, enteros positivos, rangos de temporadas y capítulos correctos).

---

# Cómo compilar y ejecutar

## Requisitos previos:

- Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
- Instalar la extensión en [Visual Studio Code] **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**)(https://platzi.com/tutoriales/1189-algoritmos-2017/2765-compilando-cc-desde-visual-studio-code-windows-10/). Si estás en Windows (), se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

---

## Estructura de carpetas

  ```
  .
├── main.c
├── gestor_series.c
├── gestor_series.h
├── TDAs/
│   ├── list.c
│   ├── list.h
│   ├── map.c
│   ├── map.h
│   ├── extra.c
│   ├── extra.h
└── README.md
  ```

---

### Pasos para compilar y ejecutar:

1. **Clona el repositorio** en tu máquina local:
   ```bash
   git clone https://github.com/Danilo-Alvarez-L/Proyecto.git
   ```
2. **Abre el proyecto en Visual Studio Code**  
   - Inicia Visual Studio Code.  
   - Selecciona `Archivo > Abrir carpeta…` y elige la carpeta `Proyecto` que acabas de clonar.

3. **Compila el código**  
   - Abre la terminal integrada (`Terminal > Nueva terminal`).  
   - Ejecuta el comando (ajusta la ruta si es necesario):
   ```
   gcc -Wall -Wextra -I .\TDAs \ .\main.c .\gestor_series.c .\extra.c \ .\TDAs\list.c .\TDAs\map.c \ -o gestor_series.exe
   ```

4. **Ejecuta la aplicación:**
   - Una vez compilado, puedes ejecutar la aplicación con:

   ```bash
   ./gestor_series tambien puedes usar .\gestor_series.exe
   ```

---

## Ejemplo Uso
Al iniciar, el programa mostrará el menú:

```
1) Ingresar serie
2) Mostrar series pendientes
3) Buscar serie
4) Actualizar pendiente
   a) Agregar temporada
   b) Avanzar capítulo
5) Eliminar serie
6) Marcar favorita
7) Mostrar favoritas
8) Salir
9) Deshacer última acción
```

Sigue las instrucciones en pantalla. Recuerda:
- Títulos: no pueden estar vacíos ni solo espacios.  
- Números: enteros positivos dentro de rangos válidos.

---

## Ejemplo de flujo

---

```bash
$ ./gestor_series
========================================
         Gestor de Series
========================================
1) Ingresar serie
...
Seleccione una opción: 1
Título de la serie: Breaking Bad
Número de temporadas: 5
  Caps. temporada 1: 7
  Caps. temporada 2: 13
  Caps. temporada 3: 13
  Caps. temporada 4: 13
  Caps. temporada 5: 16

Serie "Breaking Bad" agregada correctamente.

# Avanzar capítulo
Seleccione una opción: 4
----------------------------------------
    Actualizar pendiente
----------------------------------------
a) Agregar temporada
b) Avanzar capítulo
Seleccione una opción: b
Título de la serie: Breaking Bad
Temporada (actual): 1
Capítulo (actual): 8
Error: capítulo inválido para la temporada 1 (1–7)

Presione una tecla para continuar...

# Deshacer última acción
Seleccione una opción: 9 // se elimino la serie "Breaking Bad" ya que la ultima accion guardada fue la opcion 1
```

---

## Problemas conocidos

- **Sin persistencia de datos:** al cerrar el programa se pierde todo el registro, no hay carga/guardado a disco.    
- **Búsqueda case-sensitive:** “Breaking Bad” y “breaking bad” se consideran diferentes.  
- **Memory leaks en `map_remove`:** al eliminar individualmente series no siempre se libera la estructura interna `MapPair`, lo que puede acumular fugas de memoria en ejecuciones prolongadas.  
- **Longitudes no controladas:** no hay límite real para títulos (más allá de `MAX_TITLE_LEN`), lo que podría truncar o corromper entradas muy largas.  

---

## A mejorar

- **Persistencia:** implementar guardado/carga en archivo (JSON, CSV o binario) para conservar el estado entre ejecuciones.
- **Redo y más niveles de undo:** extender el mecanismo de deshacer para soportar rehacer acciones o navegar por un historial completo.
- **Búsqueda insensible a mayúsculas:** normalizar títulos al buscar para mejorar la UX.
- **Liberación completa de recursos:** revisar y corregir `map_remove` para liberar siempre todos los `MapPair` y evitar fugas.
- **Validación de longitudes:** imponer o configurar un límite máximo de caracteres en títulos y nombres de temporadas.
- **Configuración externa:** permitir definir parámetros (límites de temporadas, capítulos, ruta de datos) desde un archivo de configuración.

---

## Contribución de los Integrantes

- **Danilo Álvarez Lara**  
  - Comentarios y documentación de funciones en `gestor_series.c` y `gestor_series.h`.  
  - Corrección de errores y verificación exhaustiva de cada función.  
  - Validación robusta de entradas (`leerEnteroPositivo()`, `leerTitulo()`).  
  - Corregir validación de rango de progreso por temporada.  
  - Implementación y ajuste de la función de **undo**.
  - Integración de `main.c` completo con validaciones, menús y llamadas a funciones.  
  - Pruebas de compilación y funcionamiento correcto.  

- **Joaquín Muñoz**  
  - Manejo de memoria y prevención de fugas en `gestor_eliminarYliberar`.  
  - Implementación de operaciones CRUD básicas (agregar, buscar, mostrar, eliminar, marcar favorita).  
  - Integración y pruebas de los TDAs `List` y `Map`.

- **Joaquín Tapia**  
  - Definición de estructuras y prototipos en `gestor_series.h`.  
  - Desarrollo de funciones de “Agregar temporada” y “Actualizar progreso” en `gestor_series.c`.

- **Joaquín Guarda**  
  - Comentarios y Documentación de `gestor_series.h` y revisión presencial de los headers.  
  - Apoyo en el diseño del esquema de datos y flujo general del programa.

---

Este repositorio utiliza Git, con commits para documentar el progreso.

