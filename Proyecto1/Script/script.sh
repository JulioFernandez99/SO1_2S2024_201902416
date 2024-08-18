
#!/bin/bash


# Este scrip crea 10 contenedores de docker con imagenes aleatorias de la lista de imagenes disponibles

# Array of available images

images=("alpine:latest" "busybox:latest" "ubuntu:latest" "node:latest")


# Imagenes livianas -> "alpine:latest" "busybox:latest"
# Imagenes pesadas -> "ubuntu:latest" "node:latest"


# Function to generate a random name
random_name() {
    echo "container_$(cat /dev/urandom | tr -dc 'a-z0-9' | fold -w 8 | head -n 1)"
}

# Create 10 containers
for i in {1..10}  # Corregido para usar llaves y dos puntos
do
    # Select a random image
    random_image=${images[$RANDOM % ${#images[@]}]}
    
    # Generate a random name
    name=$(random_name)
    
    # Create the container
    docker run -d --name $name $random_image  # Corregido el nombre de la opción -name a --name
    
    echo "Created container $name from image $random_image"
done


# para ver las imagenes creadas se puede usar el comando docker ps -a
# para eliminar todas las imagenes creadas se puede usar el comando docker rm -f $(docker ps -a -q)