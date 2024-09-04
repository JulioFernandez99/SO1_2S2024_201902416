from fastapi import FastAPI, HTTPException  # type: ignore
import os
import json
from models.models import SystemLog
import matplotlib.pyplot as plt

app = FastAPI()

@app.get("/")
def read_root():
    return {"Hello": "World!"}

@app.post("/logs")
def get_logs(logs: SystemLog):
    logs_file = 'logs/logs.json'

    try:
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

        # Generar gráfico basado en el uso de CPU
        generate_plot(existing_logs)

        return {"received": True}

    except Exception as e:
        # Manejar excepciones y registrar el error
        with open('logs/error.log', 'a') as file:
            file.write(f"Error: {e}\n")
        raise HTTPException(status_code=500, detail="Internal Server Error")

def generate_plot(logs):
    try:
        # Inicializar listas para valores de CPU de los diferentes contenedores
        cpu_usages = []
        labels = []

        for log in logs:
            # Procesar 'killed_containers'
            for container in log.get('killed_containers', []):
                cpu_usages.append(container.get('cpu_usage', 0))
                labels.append(f"Killed {container.get('pid')}")

            # Procesar 'high_consumption_containers'
            for container in log.get('high_consumption_containers', []):
                cpu_usages.append(container.get('CPUUsage', 0))
                labels.append(f"High {container.get('PID')}")

            # Procesar 'low_consumption_containers'
            for container in log.get('low_consumption_containers', []):
                cpu_usages.append(container.get('CPUUsage', 0))
                labels.append(f"Low {container.get('PID')}")

        # Crear la gráfica
        plt.figure(figsize=(12, 6))
        plt.bar(labels, cpu_usages, color='blue')
        plt.title('CPU Usage per Container')
        plt.xlabel('Container PID')
        plt.ylabel('CPU Usage (%)')
        plt.xticks(rotation=45, ha='right')
        plt.tight_layout()
        plt.grid(True)

        # Guardar la gráfica en el archivo
        plt.savefig('/code/logs/plot.png')  # Asegúrate de que esta ruta exista en tu contenedor

    except Exception as e:
        with open('logs/error.log', 'a') as file:
            file.write(f"Plotting Error: {e}\n")
