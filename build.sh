#!/bin/bash

# Compilar todo indicando que las cabeceras (.h) están en la carpeta 'include'
gcc src/*.c -Iinclude -o vein -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

if [ $? -eq 0 ]; then
    echo "Compilación exitosa. Ejecutando..."
    ./vein
else
    echo "Hubo un error en la compilación. Revisa tu código."
    fi
