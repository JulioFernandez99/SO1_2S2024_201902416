# Dockerfile para un contenedor de alto consumo
FROM python:3.8-slim

COPY high.py /high.py 

CMD ["python", "/high.py"]

# COPY : Copia el archivo high.py al contenedor
# CMD : Ejecuta el archivo high.py
# Para construir la imagen se ejecuta el comando: sudo docker build -t high .