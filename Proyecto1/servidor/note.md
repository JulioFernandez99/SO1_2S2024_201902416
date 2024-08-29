
# Comandos 

- Comando para crear el entorno -> ```python3 -m venv env``` 

- Para activar el entorno -> ```source env/bin/activate```

- Instalar fastapi -> ```pip install "fastapi[standard]"```

- Levantar fastApi -> ```fastapi dev main.py```

- Creando requirenments.txr -> ```pip freeze > requirenments.txt```

- Crear imagen con el Dockerfile -> ```sudo docker build -t py_image .```

- Correr imagen ```sudo docker run -d --name py_container -p 8000:8000 py_image```

- Para entrar al docker -> ```sudo docker exec -it py_container bash```

- Detener el contenedor -> ```sudo docker stop py_container```

- Para levantar el docker-compose -> ```sudo docker compose up -d```

- Bajar el docker compose -> ```sudo docker compose down```


