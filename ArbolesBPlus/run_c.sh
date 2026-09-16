#!/bin/bash

# Utilizamos el nuevo Makefile para compilar
echo "======================================"
echo " Ejecutando 'make' para compilar... "
echo "======================================"
make

# Verificamos si la compilación fue exitosa
if [ $? -eq 0 ]; then
    echo "======================================"
    echo " Compilación exitosa. Iniciando... "
    echo "======================================"
    # Ejecutamos el binario
    ./bplus_tree
else
    echo "======================================"
    echo " Error en la compilación. "
    echo "======================================"
fi
