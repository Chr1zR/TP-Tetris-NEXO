=============	
Cómo compilar
=============

0. DESCARGAR DEPENDENCIA (OBLIGATORIO)
--------------------------------------
Descargar la librería GBT:

    https://gitlab.com/RodrigoMaranzana/libgbt-dist

Descomprimir el .zip -> libgbt-dist-main/

Dentro del zip se encuentra una carpeta con nombre similar a: GBT_v202x.xx.xx

1. Clonar el repositorio
   Desde carpeta donde quiero almacenar el juego(Descargas,Documentos,etc):
      git clone git@github.com:Chr1zR/TP-Tetris-NEXO.git

2. Luego copiar y pegar la carpeta: libgbt-dist-main/release/GBT_v202x.xx.xx dentro del proyecto:

   TP-Tetris-NEXO/lib/

La estructura final debe quedar así:
TP-tetris-NEXO/lib/GBT_v202x.xx.xx/
    include/
    lib/
    bin/

3. Abrir CodeBlocks
4. File → New → Project → Console application → C
5. Seleccionar el directorio del repositorio
6. Eliminar main.c vinculado al proyecto
   Sobre la solapa Sources en seccion Management -> click derecho en main.c -> Remove file from project

7. Agregar archivos de /src e /include (archivos .c y .h)
   
   AGREGAR SOURCES (.c):
   - Click derecho en "Sources"
   - Add files...
   - Seleccionar TODOS los archivos .c dentro de /src

   AGREGAR HEADERS (.h):
   - Click derecho en el proyecto o en "Headers"
   - Add files...
   - Seleccionar TODOS los archivos .h dentro de /include

8. Configurar Build options:
   - Linker: agregar -lgbt
   - Search Directories (Compiler): apuntar a lib/GBT_v202x.xx.xx/include/
   - Search Directories (Linker): apuntar a lib/GBT_v202x.xx.xx/lib/

9. Compilar (Build)
10. Copiar y pegar TP-Tetris-NEXO/lib/GBT_v202x.xx.xx/bin/gbt.dll en bin/Debug/
11. Compilar y ejecutar (Build and run)