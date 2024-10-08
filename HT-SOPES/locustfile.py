from locust import HttpUser, task

class MyUser(HttpUser):
    @task
    def send_data(self):
        payload = [
            { "student": "Juan", "age": 20, "faculty": "Agronomía", "discipline": 1 },
            { "student": "Pedro", "age": 21, "faculty": "Agronomía", "discipline": 2 }
        ]
        with self.client.post("/", json=payload, catch_response=True) as response:
            if response.status_code == 200:
                print("Response: ", response.text)  # Esto imprimirá la respuesta en la consola
            else:
                response.failure("Request failed with status code: {}".format(response.status_code))
