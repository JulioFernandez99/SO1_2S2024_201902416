#!/bin/bash

# Este script crea 10 contenedores de Docker con imágenes aleatorias de la lista de imágenes disponibles

# Array de imágenes disponibles
images=("high-image-1" "high-image-2" "low-image-1" "low-image-2")




# Función para generar un nombre aleatorio
random_name() {
    echo "container_$(cat /dev/urandom | tr -dc 'a-z0-9' | fold -w 8 | head -n 1)"
}

# Crear 10 contenedores
for i in {1..10}
do
    # Seleccionar una imagen aleatoria
    random_image=${images[$RANDOM % ${#images[@]}]}
    
    # Generar un nombre aleatorio
    name=$(random_name)
    
    # Intentar crear el contenedor
    if docker run -d --name $name $random_image; then
        echo "Created container $name from image $random_image"
    else
        echo "Failed to create container $name from image $random_image"
    fi
    
    # Opcional: Agregar un retraso para evitar problemas de rapidez
    sleep 1
done

# Para ver los contenedores creados, usa el comando docker ps -a
# Para eliminar todos los contenedores creados, usa el comando docker rm -f $(docker ps -a -q)
