![](https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif)

<div style="text-align: center;">
    <span style="font-size: 18px;">Universidad de San Carlos de Guatemala</span><br>
    <span style="font-size: 18px;">Facultad de Ingeniería</span><br>
    <span style="font-size: 18px;">Escuela de Ciencias y Sistemas</span><br>
    <span style="font-size: 18px;">Laboratorio de Sistemas de Bases de Datos 1 Sección B</span><br>
    <span style="font-size: 18px;">Julio Alfredo Fernández Rodríguez 201902416</span><br>

</div>
<br>

![](https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif)

<br>

# Proyecto 2 - Kubernetes

## Instalacion de minikube

### 1. Instalar Dependencias
Primero, se asegura de que todas las dependencias necesarias estén instaladas. Se recomienda actualizar los paquetes e instalar `curl` y `conntrack` con:

```bash
sudo apt update
sudo apt install -y curl conntrack
```

### 2. Instalar `kubectl`
Minikube necesita `kubectl` para gestionar el clúster. Para instalar `kubectl`, se puede ejecutar lo siguiente:

```bash
curl -LO "https://dl.k8s.io/release/$(curl -L -s https://dl.k8s.io/release/stable.txt)/bin/linux/amd64/kubectl"
sudo install -o root -g root -m 0755 kubectl /usr/local/bin/kubectl
rm kubectl
```

### 3. Descargar e Instalar Minikube
Después, se descarga e instala Minikube con estos comandos:

```bash
curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64
sudo install minikube-linux-amd64 /usr/local/bin/minikube
rm minikube-linux-amd64
```

### 4. Iniciar Minikube
Para iniciar Minikube, se ejecuta el comando siguiente. Se puede especificar el controlador deseado, como `docker` o `virtualbox`:

```bash
minikube start --driver=docker
```

Si `docker` no está instalado en el sistema, se puede instalar o elegir otro controlador compatible.

### 5. Verificar el Estado de Minikube
Se asegura de que Minikube esté funcionando correctamente con:

```bash
minikube status
```

### Opcional: Habilitar el Panel de Control de Kubernetes
Para acceder al panel de control de Kubernetes, se puede ejecutar:

```bash
minikube dashboard
```

Esto abrirá el panel de control en el navegador. Con estos pasos, Minikube debería estar instalado y en funcionamiento en Ubuntu.

## Para iniciar con gRPC, primero se debe instalar de manera local `Protocol Buffer Compiler Installation`

**Insalación en el SO**
```bash
# Para linux con debian | ubuntu
apt install -y protobuf-compiler
protoc --version  # Ensure compiler version is 3+

# Para fedora
sudo dnf install protobuf-compiler
```

## Golang setup

**Luego debemos instalar un paquete para golang para poder usar gRPC y otros paquetes globales**
```bash
go get google.golang.org/grpc

go install google.golang.org/protobuf/cmd/protoc-gen-go@latest

go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@latest
```

**Actualizar en el PATH para poder compilar**
```bash
export PATH=$PATH:$(go env GOPATH)/bin
```

**Instalar fiber para crear APIs rápido**

```bash
go get github.com/gofiber/fiber/v2 # fiber
```

**Comando para generar compilados dentro de la carpeta `/proto`**
```bash
protoc --go_out=. --go-grpc_out=. $(name).proto # compilar el archivo .proto  
```

## Rust setup para construir un gRPC client

**Primero creamos un proyecto en Rust**

```bash
cargo new grpc-client
cd grpc-client
```

**Creamos la carpeta /proto y su archivo**

```bash
mkdir proto
touch proto/client.proto
```

**Ahora vamos a configurar el archivo `Cargo.toml`**

```toml
[package]
name = "grpc-client"
version = "0.1.0"
edition = "2021"

[[bin]] # Para retornar el gRPC server
name = "studentgrpc-server"
path = "src/server.rs"

[[bin]] # Para retornar el gRPC client
name = "studentgrpc-client"
path = "src/client.rs"

[dependencies]
tonic = "0.12"
prost = "0.13"
tokio = {version = "1.0", features = ["macros", "rt-multi-thread"]}

[build-dependencies]
tonic-build ="0.12"
```

Aquí tienes la documentación del código de un servidor gRPC en Go orientado al servicio de agronomía. Además, se incluye una breve explicación de gRPC y los comandos necesarios para compilar y configurar el entorno.

---

## Explicación de gRPC para servidor de agronomia (golang)
gRPC (gRPC Remote Procedure Call) es un marco de trabajo de código abierto diseñado por Google para la creación de servicios distribuidos. Utiliza HTTP/2 para el transporte y Protocol Buffers (protobuf) como lenguaje de definición de interfaz. gRPC permite a los clientes ejecutar funciones en un servidor remoto como si fueran locales, facilitando la comunicación entre diferentes servicios o aplicaciones a través de redes, con alta eficiencia y soporte para múltiples lenguajes.

## Requisitos de Configuración y Compilación

1. **Instalación de gRPC y Protocol Buffers en Go**  
   Para utilizar gRPC y Protocol Buffers, se deben instalar las siguientes dependencias:

   ```bash
   go get google.golang.org/grpc
   go install google.golang.org/protobuf/cmd/protoc-gen-go@latest
   go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@latest
   ```

2. **Actualizar el PATH**  
   Para permitir que el sistema reconozca las herramientas instaladas en el PATH, se agrega el directorio de binarios de Go:

   ```bash
   export PATH=$PATH:$(go env GOPATH)/bin
   ```

3. **Instalación de Fiber**  
   Fiber es un marco rápido y minimalista de creación de APIs en Go:

   ```bash
   go get github.com/gofiber/fiber/v2
   ```

4. **Compilación de archivos `.proto`**  
   Se deben compilar los archivos de definición de servicio en la carpeta `proto` con el siguiente comando, reemplazando `$(name)` por el nombre del archivo `.proto` (en este caso `student`):

   ```bash
   protoc --go_out=. --go-grpc_out=. $(name).proto
   ```

---

## Descripción del archivo `student.proto`

El archivo `student.proto` define el servicio y los mensajes que el servidor gRPC maneja.

```proto
syntax = "proto3";
option go_package = "./;student";
package student;
```

1. **Servicio**  
   Define el servicio `Student`, que incluye el método `GetStudent`, el cual espera un mensaje `StudentRequest` y devuelve un mensaje `StudentResponse`.

   ```proto
   service Student {
       rpc GetStudent(StudentRequest) returns (StudentResponse);
   }
   ```

2. **Mensajes**  
   Define las estructuras de los mensajes que se enviarán y recibirán en cada solicitud. Los mensajes se describen a continuación:
   
   - `StudentRequest`: Incluye información sobre el estudiante (nombre, edad, facultad y disciplina).
   - `Discipline`: Es una enumeración que define los valores posibles para la disciplina (con asignaciones numéricas para evitar problemas de compatibilidad en producción).
   - `StudentResponse`: Un mensaje sencillo que indica el éxito de la operación con un valor booleano.

---

## Código del Servidor en Go (`main.go`)

El archivo `main.go` implementa un servicio REST en Go utilizando Fiber, que interactúa con diferentes servidores gRPC según la disciplina seleccionada.

1. **Definición de Servidores**
   Un mapa define las direcciones de los servidores gRPC según la disciplina:

   ```go
   var servers = map[int]string{
       1: "go-server-natacion-service:50051", // Natación
       2: "go-server-atletismo-service:50052", // Atletismo
       3: "go-server-boxeo-service:50053",     // Boxeo
   }
   ```

2. **Estructura de `Student`**
   Define la estructura `Student` para recibir y procesar los datos de solicitud de los estudiantes desde la API REST.

   ```go
   type Student struct {
       Student   string `json:"student"`
       Age       int    `json:"age"`
       Faculty   string `json:"faculty"`
       Discipline int   `json:"discipline"`
   }
   ```

3. **Función `sendData`**
   La función `sendData` es el núcleo de la lógica del servidor y realiza lo siguiente:
   
   - Recibe y procesa los datos de solicitud.
   - Valida la disciplina y busca el servidor correspondiente.
   - Establece una conexión con el servidor gRPC y envía los datos mediante una solicitud `GetStudent`.
   - Espera la respuesta o un error, y devuelve el resultado a través de la API REST.

4. **Función Principal (`main`)**
   En el `main`, se configura el servidor REST con Fiber y se define una ruta para recibir solicitudes en el endpoint `/Agronomia`. El servidor escucha en el puerto `8080`.

---
## Explicación de gRPC para servidor de ingenieria (rust)

Este código proporciona un servicio de agronomía para la gestión de estudiantes según sus disciplinas, utilizando una arquitectura distribuida y escalable con gRPC y Fiber.

Aquí se muestra una implementación de un servicio de gRPC utilizando Rust y Actix-web para construir una API rápida y sencilla que interactúe con servidores específicos de disciplina. La aplicación permite enviar datos de estudiantes a un servidor gRPC externo basado en la disciplina seleccionada.

### Estructura del Código `student.proto` para Rust

El archivo `student.proto` define el contrato de comunicación para los servicios y mensajes de estudiantes. Contiene servicios, mensajes y enumeraciones que serán utilizados por ambos, cliente y servidor, para garantizar que los datos enviados y recibidos sigan una estructura fija.

### Componentes Clave del Código Rust

#### Importaciones y Módulos
- **Actix-web**: El framework Actix-web maneja el servidor HTTP y las rutas de la API.
- **Tonic**: La biblioteca de Rust para gRPC permite conectar clientes gRPC y realizar llamadas a los métodos definidos en `student.proto`.
- **Tokio**: Ejecuta el servidor de manera asincrónica para manejar las conexiones gRPC concurrentemente.
- **Serde**: Se utiliza para serializar y deserializar el JSON de entrada, mapeando `StudentData` a los datos del estudiante.

#### `student.proto` en Rust
El código Rust requiere la conversión de `student.proto` en un módulo de Rust utilizando `tonic`, que crea las interfaces necesarias para realizar llamadas gRPC en Rust. El comando para generar este módulo es:
```bash
protoc --rust_out=. --rust-grpc_out=. student.proto
```

#### Estructura de Datos `StudentData`
La estructura `StudentData` mapea los campos del JSON a los campos de Rust, utilizando `serde` para la deserialización de JSON. La entrada del JSON se recibe en el siguiente formato:
```json
{
  "student": "John Doe",
  "age": 21,
  "faculty": "Agronomy",
  "discipline": 1
}
```

#### Servidores por Disciplina
La constante `SERVERS` define las direcciones de los servidores gRPC de acuerdo con la disciplina, asegurando que cada disciplina tenga un servidor designado:
- **Disciplina 1 (Natación)**: `"http://go-server-natacion-service:50051"`
- **Disciplina 2 (Atletismo)**: `"http://go-server-atletismo-service:50052"`
- **Disciplina 3 (Boxeo)**: `"http://go-server-boxeo-service:50053"`

#### Función `handle_student`
La función `handle_student` procesa los datos de los estudiantes y envía la solicitud al servidor gRPC correspondiente:
1. **Validación de Disciplina**: Verifica que el campo `discipline` esté en el rango de 1 a 3.
2. **Conexión al Servidor gRPC**: Selecciona la dirección del servidor de acuerdo con la disciplina.
3. **Llamada gRPC Asíncrona**: Utiliza `tokio::spawn` para manejar la llamada a `GetStudent` de manera concurrente.
4. **Respuesta Inmediata**: La función devuelve una respuesta `HttpResponse::Accepted` al cliente mientras el procesamiento continúa en segundo plano.

#### Función Principal `main`
La función `main` inicia el servidor HTTP en `0.0.0.0:8081` y define la ruta `/Ingenieria`, que dirige las solicitudes POST a `handle_student`.

### Documentación Adicional

#### gRPC y Compilación de Protocolo
Para generar los archivos necesarios en Rust, instale los paquetes gRPC y Protobuf con los comandos:
```bash
cargo install protoc
cargo install protoc-gen-tonic
```

Luego, ejecute el comando:
```bash
protoc --rust_out=. --rust-grpc_out=. student.proto
```

Este proyecto utiliza Actix-web y Tonic para una integración rápida de gRPC en Rust.

Claro, aquí tienes la documentación estructurada con la explicación sobre el funcionamiento del código y sin los comandos de ejecución:

---

<br>

# Sistema gRPC y Kafka para Gestión de Disciplina de Estudiantes

Este sistema implementa servidores gRPC para manejar información sobre estudiantes en disciplinas deportivas específicas: **Natación**, **Atletismo** y **Boxeo**. Cada servidor opera en un puerto independiente y recibe datos de estudiantes, determinando aleatoriamente si el estudiante es un "ganador" o "perdedor". Los resultados se envían a un broker Kafka en distintos tópicos.

## Funcionalidad y Configuración del Sistema

### Componentes del Sistema

1. **Servidor gRPC**:
   - Implementa una interfaz `StudentServer` para recibir solicitudes con datos de estudiantes.
   - Cada servidor de disciplina (Natación, Atletismo, Boxeo) opera en un puerto específico y recibe detalles como nombre del estudiante, edad, facultad y disciplina.

2. **Integración con Kafka**:
   - Un `Kafka Broker` recibe mensajes enviados por cada servidor en función del resultado del estudiante (ganador o perdedor).
   - Dos tópicos están configurados en Kafka: 
     - `winners`: Para estudiantes clasificados como ganadores.
     - `losers`: Para estudiantes clasificados como perdedores.

### Detalle de Funcionalidades del Código

1. **Configuración del Servidor y Kafka**:
   - **Port**: Define el puerto en el cual se ejecuta el servidor, dependiendo de la disciplina. La configuración de puerto es:
     - **Natación**: `50051`
     - **Atletismo**: `50052`
     - **Boxeo**: `50053`
   - **kafkaBroker**: Especifica la dirección del broker Kafka para manejar la mensajería.
   - **kafkaWriter**: Un escritor Kafka reutilizable, configurado con `kafka.NewWriter`, permite optimizar la conexión Kafka, enviando mensajes en función del resultado del estudiante.

2. **Estructura del Servidor**:
   - El servidor implementa `pb.UnimplementedStudentServer` para manejar solicitudes gRPC del método `GetStudent`.

3. **Método `GetStudent`**:
   - Este método es el controlador principal que procesa las solicitudes gRPC. Cuando recibe una solicitud, realiza los siguientes pasos:
     - **Registro de Datos**: Los datos del estudiante recibidos (nombre, edad, facultad, disciplina) se registran en el log.
     - **Cálculo del Resultado**: Determina aleatoriamente si el estudiante es "ganador" o "perdedor" mediante un cálculo de número aleatorio.
     - **Asignación de Tópico Kafka**: El tópico `winners` se asigna para ganadores y `losers` para perdedores.
     - **Mensaje a Kafka**: Crea un mensaje con los datos del estudiante y lo envía al broker Kafka en el tópico correspondiente usando `produceToKafka`.

4. **Configuración y Envío a Kafka**:
   - **Inicialización del Escritor Kafka (`initKafkaWriter`)**: Esta función configura `kafkaWriter`, permitiendo su reutilización para enviar mensajes a Kafka.
   - **Función `produceToKafka`**: Envía el mensaje al broker Kafka en el tópico indicado (`winners` o `losers`). Acepta parámetros como el `topic` y `message`, que se procesan en `kafkaWriter.WriteMessages` con una clave y valor descriptivo del estudiante.

5. **Función `main`**:
   - Inicializa el servidor gRPC y se asegura de que cada servidor se configure en el puerto apropiado para cada disciplina.
   - Llama a `initKafkaWriter` al inicio para configurar el escritor de Kafka, que se reutilizará para las solicitudes recibidas.
   - Finalmente, inicia el servidor gRPC, manteniéndolo en escucha para las solicitudes de estudiantes en cada disciplina.

### Ejecución para Cada Disciplina

- **Natación**: Se asigna al puerto `50051`.
- **Atletismo**: Utiliza el puerto `50052`.
- **Boxeo**: Escucha en el puerto `50053`.

Cada instancia del servidor es específica para una disciplina y un puerto, permitiendo modularidad en el procesamiento de solicitudes de estudiantes y en el envío de resultados al sistema Kafka. 

---

Este diseño asegura que los mensajes se dirijan a los tópicos de Kafka según el resultado del estudiante, optimizando la separación de disciplinas y la gestión de los resultados en la mensajería.

Aquí tienes la documentación actualizada que incluye el script `build-images.sh` para la construcción y el despliegue de las imágenes en Docker Hub:

---

## Construcción y Despliegue de Imágenes Docker

Para facilitar la implementación y despliegue de cada componente, se ha creado el script `build-images.sh`, que permite construir las imágenes de Docker para cada servidor y cliente, etiquetarlas, y subirlas a Docker Hub.

### Detalle del Script `build-images.sh`

Este script realiza los siguientes pasos:

1. **Elimina Imágenes Existentes**: Se eliminan todas las imágenes Docker existentes para evitar conflictos.
2. **Configuración de Variables**: Define variables para cada imagen Docker y para el usuario de Docker Hub.
3. **Construcción de Imágenes**: Construye las imágenes de Docker para cada cliente y servidor específico (comentarios indican que sólo se construye el cliente en Rust).
4. **Etiquetado y Subida a Docker Hub**: Etiqueta cada imagen con el nombre de usuario y versión (`TAG`), luego las sube a Docker Hub.

### Script Completo

```bash
#!/bin/bash

# Remove all Docker images
sudo docker rmi -f $(sudo docker images -a -q)

# Variables for the Docker images
RUST_CLIENT_IMAGE="rust-client-grpc"
DOCKERHUB_USERNAME="juliofernandez99"
TAG="1.7"

# Build the Docker image for the Rust client
sudo docker build -t $RUST_CLIENT_IMAGE ./gRPC/grpc-client

# Tag the Docker image
docker tag $RUST_CLIENT_IMAGE "$DOCKERHUB_USERNAME/$RUST_CLIENT_IMAGE:$TAG"

# Push the Docker image to DockerHub
docker push "$DOCKERHUB_USERNAME/$RUST_CLIENT_IMAGE:$TAG"

echo "Docker images pushed successfully."
```

---

Con este script, todas las imágenes necesarias se crean y despliegan en Docker Hub, lo cual permite implementar cada uno de los servicios en la nube o en entornos distribuidos de manera eficiente.


--Continuar con los deployments de ingenieria,agronomia y las disciplinas