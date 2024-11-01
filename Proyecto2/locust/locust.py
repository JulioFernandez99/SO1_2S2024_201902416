import json
from random import randrange
from locust import HttpUser, between, task

debug = False

def printDebug(msg):
    if debug:
        print(msg)

class Reader():
    def __init__(self):
        self.array = []

    def pickRandom(self):
        lenght = len(self.array)

        if (lenght > 0):
            random_index = randrange(0, lenght - 1) if lenght > 1 else 0
            return self.array.pop(random_index)
        else:
            print(">> Reader: No encontramos ningún valor o registro en el archivo.")
            return None

    def load(self):
        print(">> Reader: Estamos iniciando la lectura del archivo de datos.")
        try:
            with open("students.json", 'r') as data_file:
                self.array = json.loads(data_file.read())
        except Exception as error:
            print(f'>> Reader: No se cargaron los datos, error: {error}')

class MessageTraffic(HttpUser):
    wait_time = between(0.1, 0.9)
    reader = Reader()
    reader.load()

    def on_start(self):
        print(">> MessageTraffic: Iniciamos el envío de tráfico")

    @task
    def post_student(self):
        random_data = self.reader.pickRandom()
        if random_data is not None:
            data_to_send = json.dumps(random_data)
            printDebug(data_to_send)

            # Verificar el valor de faculty para determinar el endpoint
            if random_data["faculty"] == "Ingenieria":
                self.client.post("/Ingenieria", json=random_data)
            elif random_data["faculty"] == "Agronomia":
                self.client.post("/Agronomia", json=random_data)
        else:
            print(">> MessageTraffic: No hay más registros para enviar.")

