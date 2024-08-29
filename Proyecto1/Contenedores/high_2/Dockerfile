# Usar una imagen base de Java
FROM openjdk:11-jre-slim

# Establecer el directorio de trabajo dentro del contenedor
WORKDIR /app

# Copiar el archivo JAR en el contenedor
COPY HeavyLoadApp.jar .

# Ejecutar el archivo JAR cuando se inicie el contenedor
CMD ["java", "-jar", "HeavyLoadApp.jar"]
