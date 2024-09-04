FROM python:3.9-slim

WORKDIR /code

# Copiar el archivo de dependencias
COPY ./requirements.txt /code/requirements.txt

# Instalar las dependencias
RUN pip install --no-cache-dir --upgrade pip && \
    pip install --no-cache-dir -r /code/requirements.txt

# Copiar el código de la aplicación
COPY ./ /code/

# Usar uvicorn para ejecutar la aplicación FastAPI
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
