from fastapi import FastAPI  # type: ignore
import os
import json
from models.models import SystemLog

app = FastAPI()

@app.get("/")
def read_root():
    return {"Hello": "World"}

@app.post("/logs")
def get_logs(logs: SystemLog):
    logs_file = 'logs/logs.json'

    # Verificamos si el archivo existe
    if os.path.exists(logs_file):
        with open(logs_file, 'r') as file:
            existing_logs = json.load(file)
    else:
        existing_logs = []

    # Convertimos el nuevo log a dict y lo agregamos a los logs existentes
    new_log = logs.dict()
    existing_logs.append(new_log)

    # Escribimos los logs en el archivo logs.json
    with open(logs_file, 'w') as file:
        json.dump(existing_logs, file, indent=4)
    
    return {"received": True}
