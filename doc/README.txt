=============	
Cómo compilar
=============
1. Clonar el repositorio
2. Abrir CodeBlocks
3. File → New → Project → Console application → C
4. Seleccionar el directorio del repositorio
5. Agregar archivos de /src e /include (archivos .c y .h)
6. Configurar Build options:
   - Linker: agregar -lgbt
   - Search Directories (Compiler): apuntar a lib/GBT/include/
   - Search Directories (Linker): apuntar a lib/GBT/lib/
7. Copiar gbt.dll a bin/Debug/ después de compilar
8. Compilar y ejecutar