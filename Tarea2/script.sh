#!/bin/bash

num_cont=10 #Numero de iretaciones (contenedores)

image="alpine:latest" #imagen del docker

# Función para generar un nombre aleatorio para el contenedor
random_name() {
    echo "container_$(cat /dev/urandom | tr -dc 'a-z0-9' | fold -w 5 | head -n 1)"
}

#TODOS: Creacion de contenedores

for i in $(seq 1 $num_cont)
do
    
    name=$(random_name) #nombre aleatorio del contenedor
    
    docker run -d --name $name $image #comando para crear el contenedor
    
    echo "Contenedor creado -> $name from image $image" # mensaje de confirmacion
done


#Permisos
#chmod +x script.sh

# docker ps -a para ver todas las imagenes 
# docker rm -f $(docker ps -a -q) para eliminarlos todos
