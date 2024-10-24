import random
import json
import os

# Función para generar un array de objetos de estudiantes
def generate_students(n):
    students = []
    for i in range(n):
        student = {
            "student": f"nombre{i+1}",
            "age": random.randint(19, 33),  # Edad mayor a 18
            "faculty": random.choice(["Agronomia", "Ingenieria"]),
            "discipline": random.choice([1, 2, 3])
        }
        students.append(student)
    return students

# Preguntar cuántos datos se desean generar
n = int(input("¿Cuántos estudiantes deseas generar? "))

# Generar los estudiantes
students = generate_students(n)

# Obtener la ruta donde está el script y guardar el archivo en ese mismo directorio
script_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.join(script_dir, 'students.json')

# Guardar el array en un archivo JSON
with open(file_path, 'w') as file:
    json.dump(students, file, indent=2)

print(f"Archivo 'students.json' generado con éxito en: {file_path}")
