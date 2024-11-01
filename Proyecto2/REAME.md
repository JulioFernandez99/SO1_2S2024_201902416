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

## Deployment servidor de go(Agronomia)
Aquí tienes una documentación detallada del código, paso a paso, en tercera persona:

---

### Descripción General
Este código implementa un servidor HTTP en Go que actúa como intermediario entre una solicitud HTTP y un servidor gRPC específico para cada disciplina académica. El servidor utiliza el framework `Fiber` para manejar las solicitudes HTTP y establece conexiones gRPC para enviar y recibir información relacionada con estudiantes.

### Paquetes Importados
1. **`context`**: Permite administrar el contexto de la solicitud gRPC, estableciendo un límite de tiempo para evitar que una solicitud se quede esperando indefinidamente.
2. **`pb "go-client/proto"`**: Importa el paquete generado automáticamente a partir del archivo `.proto`, el cual define el servicio y los mensajes gRPC para la comunicación con el servidor de estudiantes.
3. **`log`**: Proporciona una funcionalidad de registro para la salida de mensajes de error e información.
4. **`time`**: Facilita el uso de tiempos de espera y otros temporizadores en el proceso gRPC.
5. **`github.com/gofiber/fiber/v2`**: Framework HTTP ligero para construir aplicaciones y manejar solicitudes HTTP de manera rápida y sencilla.
6. **`google.golang.org/grpc` y `google.golang.org/grpc/credentials/insecure`**: Facilita el establecimiento de una conexión gRPC con el servidor especificado y permite el uso de conexiones inseguras, es decir, sin SSL/TLS.

---

### Variables Globales
#### `servers`
Define un mapa que asocia un identificador de disciplina (tipo `int`) con la dirección de cada servidor gRPC (tipo `string`). Esta configuración permite que el servidor HTTP determine a qué servidor gRPC enviar la solicitud en función de la disciplina del estudiante.

Ejemplo de configuración:
```go
var servers = map[int]string{
	1: "go-server-natacion-service:50051", // Servidor para disciplina natación
	2: "go-server-atletismo-service:50052", // Servidor para disciplina atletismo
	3: "go-server-boxeo-service:50053",     // Servidor para disciplina boxeo
}
```

Este mapeo asegura que cada disciplina tenga un servidor gRPC asignado para recibir y procesar las solicitudes según su especialización.

---

### Estructura `Student`
Esta estructura define el modelo de datos del estudiante y contiene los siguientes campos:
- **`Student` (string)**: Nombre del estudiante.
- **`Age` (int)**: Edad del estudiante.
- **`Faculty` (string)**: Facultad a la que pertenece el estudiante.
- **`Discipline` (int)**: Número de disciplina que identifica el servidor gRPC correspondiente (1 para natación, 2 para atletismo, 3 para boxeo).

La estructura también incluye etiquetas `json` para indicar el formato esperado en el cuerpo de la solicitud JSON.

---

### Función `sendData`

Esta función maneja las solicitudes HTTP POST en la ruta `/Agronomia`, procesa los datos recibidos, establece una conexión con el servidor gRPC adecuado según la disciplina del estudiante y devuelve la respuesta.

#### Pasos de la Función

1. **Parseo del cuerpo de la solicitud**:
   La función recibe el contexto de `Fiber` (`fiberCtx`), luego intenta deserializar el cuerpo de la solicitud en formato JSON hacia la estructura `Student`. Si ocurre algún error en el parseo, la función retorna un mensaje de error en formato JSON con un código de estado HTTP 400.

   ```go
   if err := fiberCtx.BodyParser(&body); err != nil {
       return fiberCtx.Status(400).JSON(fiber.Map{
           "error": err.Error(),
       })
   }
   ```

2. **Verificación de la disciplina**:
   Se verifica si el campo `Discipline` de la solicitud es válido, es decir, si se encuentra en el mapa `servers`. Si el número de disciplina no está en el mapa, la función devuelve un error con un código de estado HTTP 400.

   ```go
   addr, ok := servers[body.Discipline]
   if !ok {
       return fiberCtx.Status(400).JSON(fiber.Map{
           "error": "discipline debe ser 1, 2 o 3 para enviar al servidor gRPC",
       })
   }
   ```

3. **Establecimiento de la conexión gRPC**:
   La función intenta conectar con el servidor gRPC que corresponde a la disciplina utilizando la dirección obtenida de `servers`. La conexión es establecida con `grpc.Dial` y se configura para que sea insegura (sin credenciales). Si no se puede conectar, el programa se detiene y registra un mensaje de error.

   ```go
   conn, err := grpc.Dial(addr, grpc.WithTransportCredentials(insecure.NewCredentials()))
   if err != nil {
       log.Fatalf("did not connect: %v", err)
   }
   defer conn.Close()
   ```

4. **Creación de cliente gRPC**:
   Se crea un cliente gRPC (`StudentClient`) a partir de la conexión establecida. Este cliente es usado para enviar la solicitud al servidor gRPC.

   ```go
   c := pb.NewStudentClient(conn)
   ```

5. **Manejo de la solicitud en un goroutine**:
   Se utiliza un goroutine para ejecutar la solicitud gRPC de forma asíncrona. La función configura un `context` con un tiempo límite de un segundo, luego utiliza el cliente `StudentClient` para llamar al método `GetStudent` y enviar una solicitud `StudentRequest` con los datos del estudiante.

   ```go
   go func() {
       ctx, cancel := context.WithTimeout(context.Background(), time.Second)
       defer cancel()
       
       r, err := c.GetStudent(ctx, &pb.StudentRequest{
           Student:       body.Student,
           Age:           int32(body.Age),
           Faculty:       body.Faculty,
           Discipline:    pb.Discipline(body.Discipline),
       })
       
       if err != nil {
           errorChan <- err
           return
       }
       
       responseChan <- r
   }()
   ```

   El resultado de la solicitud o cualquier error se envía al canal `responseChan` o `errorChan`, respectivamente.

6. **Recepción de la respuesta gRPC**:
   La función utiliza `select` para esperar la respuesta del canal `responseChan`, un error desde `errorChan`, o un tiempo de espera de 5 segundos. Dependiendo de cuál se complete primero:
   
   - Si se recibe una respuesta en `responseChan`, la función devuelve un mensaje JSON indicando éxito.
   - Si ocurre un error, la función devuelve un mensaje de error con código de estado HTTP 500.
   - Si se agota el tiempo de espera, también devuelve un mensaje de error de tiempo de espera.

   ```go
   select {
   case response := <-responseChan:
       return fiberCtx.JSON(fiber.Map{
           "message": response.GetSuccess(),
       })
   case err := <-errorChan:
       return fiberCtx.Status(500).JSON(fiber.Map{
           "error": err.Error(),
       })
   case <-time.After(5 * time.Second):
       return fiberCtx.Status(500).JSON(fiber.Map{
           "error": "timeout",
       })
   }
   ```

---

### Función `main`
Esta función es el punto de entrada de la aplicación y realiza las siguientes tareas:

1. **Inicialización del servidor Fiber**:
   Crea una nueva instancia de `Fiber`, un framework de manejo HTTP, para procesar solicitudes HTTP.

   ```go
   app := fiber.New()
   ```

2. **Definición de la ruta POST**:
   Define la ruta `/Agronomia` que maneja solicitudes POST y asigna la función `sendData` como manejador de las solicitudes en esta ruta.

   ```go
   app.Post("/Agronomia", sendData)
   ```

3. **Inicio del servidor HTTP**:
   Inicia el servidor en el puerto 8080. Si ocurre un error al iniciar el servidor, el error es registrado en la consola.

   ```go
   err := app.Listen(":8080")
   if err != nil {
       log.Println(err)
       return
   }
   ```

--- 

### Resumen
El servidor permite recibir datos de estudiantes a través de solicitudes HTTP POST, verificar y enviar estos datos a un servidor gRPC según la disciplina, y devolver el resultado al cliente HTTP. En caso de errores o tiempos de espera, se devuelven mensajes de error informativos.


## Deployment servidor de rust(Ingenieria)

Este código implementa un servidor HTTP en Rust usando Actix-web, que expone una ruta `/Ingenieria` para procesar solicitudes de estudiantes mediante un cliente gRPC para diferentes disciplinas deportivas. El servidor recibe un JSON con datos del estudiante y reenvía la solicitud al servidor gRPC correspondiente según la disciplina.

### Descripción detallada del código

1. **Importaciones de librerías:**
   ```rust
   use actix_web::{web, App, HttpServer, HttpResponse, Responder};
   use studentgrpc::student_client::StudentClient;
   use studentgrpc::StudentRequest;
   use serde::{Deserialize, Serialize};
   use tokio;
   ```
   - `actix_web`: Para configurar el servidor HTTP, manejar solicitudes y respuestas.
   - `studentgrpc`: Módulo gRPC que contiene el cliente `StudentClient` y el tipo de solicitud `StudentRequest`.
   - `serde`: Para serializar y deserializar datos JSON.
   - `tokio`: Usado para ejecutar tareas asíncronas.

2. **Definición del módulo gRPC:**
   ```rust
   pub mod studentgrpc {
       tonic::include_proto!("student");
   }
   ```
   - `tonic::include_proto!("student")`: Importa las definiciones gRPC generadas a partir de un archivo Protobuf llamado `student.proto`, que define las estructuras y métodos gRPC para interactuar con el servidor.

3. **Estructura `StudentData`:**
   ```rust
   #[derive(Deserialize, Serialize)]
   struct StudentData {
       #[serde(rename = "student")]
       name: String,
       age: i32,
       faculty: String,
       discipline: i32,
   }
   ```
   - Esta estructura representa los datos del estudiante que se esperan en la solicitud JSON.
   - `#[serde(rename = "student")]` indica que el campo `name` en Rust se mapeará al campo `student` en el JSON de entrada.

4. **Mapa de direcciones de servidores:**
   ```rust
   const SERVERS: [&str; 3] = [
       "http://go-server-natacion-service:50051",
       "http://go-server-atletismo-service:50052",
       "http://go-server-boxeo-service:50053",
   ];
   ```
   - `SERVERS`: Arreglo que contiene las direcciones de los servidores gRPC para cada disciplina. Cada índice en el arreglo representa una disciplina: 1 para Natación, 2 para Atletismo, y 3 para Boxeo.

5. **Función `handle_student`:**
   ```rust
   async fn handle_student(student: web::Json<StudentData>) -> impl Responder {
   ```
   - `student`: Parámetro de tipo `web::Json<StudentData>` que se deserializa automáticamente desde el cuerpo de la solicitud JSON.
   - La función realiza una verificación y, dependiendo de la disciplina del estudiante, reenvía la solicitud al servidor correspondiente.

   **Detalles del procesamiento:**
   ```rust
   if student.discipline < 1 || student.discipline > 3 {
       return HttpResponse::BadRequest().body("Discipline must be 1, 2, or 3");
   }
   ```
   - Verifica que la disciplina esté entre 1 y 3. Si no, responde con un error `BadRequest`.

   ```rust
   let server_addr = SERVERS[(student.discipline - 1) as usize];
   ```
   - Obtiene la dirección del servidor correspondiente en el arreglo `SERVERS`.

   **Ejecución de la conexión gRPC:**
   ```rust
   tokio::spawn(async move {
       let mut client = match StudentClient::connect(server_addr).await {
           Ok(client) => client,
           Err(e) => {
               eprintln!("Failed to connect to gRPC server: {}", e);
               return;
           }
       };
   ```
   - Usa `tokio::spawn` para crear una tarea asíncrona que conecta con el servidor gRPC.
   - `StudentClient::connect(server_addr).await` intenta establecer una conexión con el servidor.

   **Creación y envío de la solicitud gRPC:**
   ```rust
   let request = tonic::Request::new(StudentRequest {
       name: student_name,
       age: student_age,
       faculty: student_faculty,
       discipline: student_discipline,
   });
   
   match client.get_student(request).await {
       Ok(response) => {
           println!("RESPONSE={:?}", response);
       },
       Err(e) => eprintln!("gRPC call failed: {}", e),
   }
   ```
   - Crea un objeto `StudentRequest` con los datos del estudiante.
   - Llama al método `get_student` del cliente gRPC, que envía la solicitud al servidor remoto.

6. **Función principal `main`:**
   ```rust
   #[actix_web::main]
   async fn main() -> std::io::Result<()> {
   ```
   - Declara el punto de inicio del servidor HTTP utilizando el macro `#[actix_web::main]` para habilitar la ejecución asíncrona con Actix.

   **Configuración del servidor:**
   ```rust
   HttpServer::new(|| {
       App::new()
           .route("/Ingenieria", web::post().to(handle_student))
   })
   .bind("0.0.0.0:8081")?
   .run()
   .await
   ```
   - Configura el servidor en la dirección `0.0.0.0:8081`.
   - Define una ruta `POST` en `/Ingenieria` que llama a la función `handle_student`.

### Resumen de la funcionalidad
Este código establece un servidor HTTP que recibe solicitudes de datos de estudiantes. Según la disciplina, reenvía los datos a un servidor gRPC específico usando `tonic`, realiza la llamada gRPC en segundo plano con `tokio::spawn`, y responde inmediatamente al cliente HTTP que la solicitud está en proceso.

## Configuracion disciplinas

Aquí tienes una documentación detallada del código en Go que configura un servidor gRPC para cada disciplina (natación, atletismo y boxeo) y se comunica con Kafka para registrar el resultado del procesamiento:

---

### Descripción General

Este código es un servidor gRPC escrito en Go que utiliza un modelo cliente-servidor. El servidor escucha en puertos diferentes según la disciplina (natación en el puerto 50051, atletismo en el 50052, y boxeo en el 50053). Cuando recibe una solicitud, procesa los datos del estudiante, determina de manera aleatoria si el estudiante es un "ganador" o "perdedor" y envía este resultado a un tópico específico en Kafka.

### Detalle de la Implementación

#### 1. **Configuración de Dependencias e Importaciones**

   ```go
   import (
       "context"
       "flag"
       "fmt"
       "log"
       "math/rand"
       "net"
       "time"
       "github.com/segmentio/kafka-go"
       pb "go-server/proto"
       "google.golang.org/grpc"
   )
   ```
   - **context**: Maneja contextos para controlar tiempos de espera y cancelaciones en solicitudes gRPC y Kafka.
   - **flag**: Permite definir opciones configurables desde la línea de comandos (en este caso, el puerto del servidor).
   - **log**: Registra mensajes en la consola, principalmente para fines de depuración.
   - **math/rand y time**: Generan números aleatorios y manipulan tiempos (semilla para asegurar aleatoriedad en cada solicitud).
   - **net**: Permite configurar conexiones de red (aquí, para iniciar el listener en el puerto del servidor).
   - **github.com/segmentio/kafka-go**: Interactúa con Kafka para enviar mensajes al clúster Kafka.
   - **pb "go-server/proto"**: Importa el paquete auto-generado de gRPC con base en el archivo `.proto`.
   - **google.golang.org/grpc**: Implementa la comunicación gRPC.

#### 2. **Variables y Flags**

   ```go
   var (
       port        = flag.Int("port", 50051, "The server port")
       kafkaBroker = "my-cluster-kafka-bootstrap.kafka.svc.cluster.local:9092"
       topic       = "winners"
       kafkaWriter *kafka.Writer
   )
   ```
   - **port**: Define el puerto en el que el servidor escuchará (predeterminado 50051, puerto para natación).
   - **kafkaBroker**: Dirección del broker de Kafka, que permite al servidor comunicarse con el clúster de Kafka.
   - **topic**: Nombre inicial del tópico en Kafka. Se asignará dinámicamente según el resultado de cada solicitud (ganador o perdedor).
   - **kafkaWriter**: Es una instancia del escritor de Kafka, inicializado una vez y reutilizado en toda la vida del servidor.

#### 3. **Definición del Servidor gRPC**

   ```go
   type server struct {
       pb.UnimplementedStudentServer
   }
   ```
   Esta estructura define el servidor y hereda de `UnimplementedStudentServer`, lo cual es necesario para implementar métodos de servicio.

#### 4. **Método `GetStudent`**

   ```go
   func (s *server) GetStudent(_ context.Context, in *pb.StudentRequest) (*pb.StudentResponse, error) {
       log.Printf("Received: %v", in)
       log.Printf("Student: %s", in.GetStudent())
       log.Printf("Student faculty: %s", in.GetFaculty())
       log.Printf("Student age: %d", in.GetAge())
       log.Printf("Student discipline: %d", in.GetDiscipline())
   ```

   - **Propósito**: Implementa el método gRPC `GetStudent` que responde a las solicitudes `StudentRequest`.
   - **Logging**: Registra en consola los datos del estudiante para fines de auditoría y depuración.

   ##### Generación de "Ganador" o "Perdedor"

   ```go
       rand.Seed(time.Now().UnixNano())
       winner := rand.Intn(2)
   ```

   - `rand.Seed(time.Now().UnixNano())`: Asegura que el generador de números aleatorios cambie en cada solicitud.
   - `rand.Intn(2)`: Genera un número aleatorio (0 o 1), asignando 1 a "ganador" y 0 a "perdedor".

   ##### Selección de Tópico y Creación del Mensaje

   ```go
       if winner == 1 {
           topic = "winners"
       } else {
           topic = "losers"
       }
       result := "loser"
       if winner == 1 {
           result = "winner"
       }
       message := fmt.Sprintf("Student: %s, Age: %d, Faculty: %s, Discipline: %d, Result: %s",
           in.GetStudent(), in.GetAge(), in.GetFaculty(), in.GetDiscipline(), result)
   ```

   - **Asignación de Tópico**: Asigna `winners` para ganadores y `losers` para perdedores.
   - **Creación del Mensaje**: Genera un mensaje de texto con los datos del estudiante y el resultado, para enviarlo a Kafka.

   ##### Envío del Mensaje a Kafka

   ```go
       err := produceToKafka(topic, message)
       if err != nil {
           log.Printf("Failed to produce message to Kafka: %v", err)
           return nil, err
       }
   ```

   - **produceToKafka**: Llama a una función para enviar el mensaje a Kafka en el tópico determinado. Si falla, registra el error y lo devuelve.

   ##### Respuesta a gRPC

   ```go
       return &pb.StudentResponse{
           Success: true,
       }, nil
   ```

   - **Respuesta**: Retorna un objeto `StudentResponse` indicando éxito. 

#### 5. **Función `initKafkaWriter`**

   ```go
   func initKafkaWriter() {
       kafkaWriter = kafka.NewWriter(kafka.WriterConfig{
           Brokers:  []string{kafkaBroker},
           Balancer: &kafka.LeastBytes{},
       })
   }
   ```

   - **Propósito**: Inicializa una instancia de `kafka.Writer` configurada para el broker especificado, reutilizándose en todas las solicitudes.
   - **Balanceo de Carga**: Usa `LeastBytes` para enviar mensajes al broker menos cargado.

#### 6. **Función `produceToKafka`**

   ```go
   func produceToKafka(topic string, message string) error {
       if topic == "" {
           return fmt.Errorf("topic must be specified")
       }

       err := kafkaWriter.WriteMessages(context.Background(),
           kafka.Message{
               Topic: topic,
               Key:   []byte("key"),
               Value: []byte(message),
           },
       )
   ```

   - **Validación**: Asegura que se haya especificado un tópico; si no, retorna un error.
   - **Envía el Mensaje**: Llama a `WriteMessages` para producir el mensaje a Kafka, en el tópico y con el mensaje especificado.
   - **Log de Confirmación**: Registra el mensaje enviado y el tópico.

   ```go
       if err != nil {
           return fmt.Errorf("failed to produce message: %s", err)
       }

       log.Printf("Message sent to topic %s: %s", topic, message)
       return nil
   }
   ```

#### 7. **Función `main`**

   ```go
   func main() {
       flag.Parse()
       lis, err := net.Listen("tcp", fmt.Sprintf(":%d", *port))
       if err != nil {
           log.Fatalf("Failed to listen: %v", err)
       }

       initKafkaWriter()
       defer kafkaWriter.Close()

       s := grpc.NewServer()
       pb.RegisterStudentServer(s, &server{})
       log.Printf("Server started on port %d", *port)

       if err := s.Serve(lis); err != nil {
           log.Fatalf("Failed to serve: %v", err)
       }
   }
   ```

   - **Parseo de Flags**: Llama a `flag.Parse()` para obtener el puerto desde los argumentos de línea de comandos.
   - **Listener**: Abre un listener en el puerto especificado.
   - **Inicialización de Kafka**: Llama a `initKafkaWriter()` para crear una instancia del escritor de Kafka. El `defer` asegura que `kafkaWriter.Close()` se ejecute al finalizar el programa.
   - **Inicia el Servidor**: Configura el servidor gRPC y lo registra para que empiece a escuchar solicitudes `StudentServer`.
   - **Ejecuta el Servidor**: Inicia el servicio y escucha solicitudes en el puerto especificado.

### Ejecución por Disciplina
Cada instancia del servidor debe iniciarse en un puerto diferente:
   - **50051** para natación.
   - **50052** para atletismo.
   - **50053** para boxeo.

Esto permite que cada servidor maneje solicitudes específicas de una disciplina y envíe los resultados a Kafka según el tópico configurado.

## Configuracion para el ingress

1. **Crear el espacio de nombres y agregar el repositorio de NGINX Ingress en Helm**:

   ```bash
   kubectl create ns nginx-ingress
   helm repo add ingress-nginx https://kubernetes.github.io/ingress-nginx
   helm repo update
   ```

2. **Instalar el controlador de Ingress de NGINX**:

   ```bash
   helm install nginx-ingress ingress-nginx/ingress-nginx -n nginx-ingress
   ```

3. **Verificar los servicios y obtener la IP externa del Load Balancer**:

   Ejecuta este comando para ver los servicios en el espacio de nombres `nginx-ingress`:

   ```bash
   kubectl get services -n nginx-ingress
   ```

   Busca el servicio con el tipo `LoadBalancer`. La columna `EXTERNAL-IP` mostrará la IP pública asignada. Si ves `<pending>`, espera unos segundos y vuelve a ejecutar el comando hasta que aparezca la IP.

4. **Modificar tu archivo `ingress.yaml`**:
   
   Abre el archivo `ingress.yaml` y usa la IP que obtuviste en `spec.rules.host`. El bloque `ingress.yaml` debería verse algo así:

   ```yaml
   apiVersion: networking.k8s.io/v1
   kind: Ingress
   metadata:
     name: example-ingress
     namespace: default
   spec:
     rules:
       - host: "TU_IP_O_DOMINIO"
         http:
           paths:
             - path: /
               pathType: Prefix
               backend:
                 service:
                   name: nombre-del-servicio
                   port:
                     number: 80
   ```

5. **Aplicar el archivo `ingress.yaml`**:

   Finalmente, despliega el Ingress con:

   ```bash
   kubectl apply -f ingress.yaml
   ```


Aquí tienes la documentación detallada de cada uno de los comandos que proporcionaste:

---

### Comandos de Kubernetes (`kubectl` y `ka`)

#### 1. **Aplicar Configuraciones con `kubectl apply`**
   **Sintaxis**: `ka <archivo>.yaml [-n <namespace>]`
   
   En este contexto, `ka` es un alias de `kubectl apply -f`, que aplica la configuración definida en los archivos YAML a un clúster de Kubernetes. Estos archivos suelen contener la definición de recursos de Kubernetes, como `Deployments`, `Services`, `Ingress`, etc. Cada archivo YAML define los detalles de configuración de un recurso específico para el clúster.

   - **`ka namespace.yaml`**: Crea o actualiza el `namespace` definido en `namespace.yaml`. Los namespaces permiten separar recursos en un clúster para organizar y gestionar su ciclo de vida.
   
   - **`ka ingress.yaml`**: Aplica la configuración de `ingress`, que maneja las reglas de entrada al clúster (ej. rutas y puntos de entrada a servicios).
   
   - **`ka goclient.yaml -n sopes1`**: Aplica la configuración para el cliente Go en el `namespace sopes1`.
   
   - **`ka rustclient.yaml -n sopes1`**: Aplica la configuración para el cliente Rust en el `namespace sopes1`.
   
   - **`ka goserverAtletismo.yaml -n sopes1`**: Aplica la configuración del servidor de atletismo en el `namespace sopes1`.
   
   - **`ka goserverNatacion.yaml -n sopes1`**: Aplica la configuración del servidor de natación en el `namespace sopes1`.
   
   - **`ka goserverBoxeo.yaml -n sopes1`**: Aplica la configuración del servidor de boxeo en el `namespace sopes1`.

#### 2. **Crear Namespace de Kafka**
   ```bash
   kubectl create namespace kafka
   ```
   Crea un namespace llamado `kafka`. Este namespace se utilizará para agrupar todos los recursos relacionados con Kafka en el clúster.

#### 3. **Instalar Strimzi Operator para Kafka**
   ```bash
   kubectl apply -f https://strimzi.io/install/latest?namespace=kafka -n kafka
   ```
   Instala Strimzi, un operador de Kafka para Kubernetes, en el `namespace kafka`. Strimzi facilita la implementación y administración de Kafka en Kubernetes.

#### 4. **Configurar un Clúster de Kafka (Single Node)**
   ```bash
   kubectl apply -f https://strimzi.io/examples/latest/kafka/kraft/kafka-single-node.yaml -n kafka
   ```
   Aplica la configuración de un clúster de Kafka de un solo nodo en el `namespace kafka`. Esta configuración es ideal para entornos de desarrollo y pruebas.

#### 5. **Crear Tópicos de Kafka**
   ```bash
   kubectl apply -f kafka-topic.yaml -n kafka
   ```
   Crea los tópicos en Kafka usando el archivo `kafka-topic.yaml`, necesario para que los consumidores y productores puedan enviar o recibir mensajes de Kafka.

#### 6. **Consumidores de Kafka**
   - **`ka kafka-consumer-winners.yaml -n sopes1`**: Configura un consumidor para el tópico de `ganadores` en Kafka.
   - **`ka kafka-consumer-loser.yaml -n sopes1`**: Configura un consumidor para el tópico de `perdedores` en Kafka.

#### 7. **Instalar Dependencia de Redis en Go**
   ```bash
   go get github.com/go-redis/redis/v8
   ```
   Este comando descarga el paquete `redis/v8` para Go, lo cual permite al código Go interactuar con Redis, una base de datos en memoria usada comúnmente para almacenar datos en tiempo real.

#### 8. **Crear Namespace para Redis**
   ```bash
   kubectl create namespace redis
   ```
   Crea un namespace llamado `redis` para contener todos los recursos relacionados con Redis.

#### 9. **Aplicar Configuración de Redis**
   ```bash
   ka redis.yaml -n redis
   ```
   Aplica la configuración de Redis definida en `redis.yaml` en el `namespace redis`. Esto generalmente incluye la creación de un `Deployment` y un `Service` para Redis.

#### 10. **Verificar los Pods en el Namespace Redis**
   ```bash
   kubectl get pods -n redis
   ```
   Muestra el estado de los pods en el `namespace redis`. Útil para asegurarse de que los pods de Redis se han creado y están funcionando correctamente.

#### 11. **Crear Namespace para Grafana**
   ```bash
   kubectl create namespace grafana
   ```
   Crea un namespace llamado `grafana`, que se utilizará para contener los recursos de Grafana, una herramienta de visualización y monitoreo.

#### 12. **Aplicar Configuración de Grafana**
   ```bash
   kubectl apply -f grafana.yaml -n grafana
   ```
   Aplica la configuración definida en `grafana.yaml` en el `namespace grafana`. Esto instala Grafana y establece las configuraciones necesarias.

#### 13. **Reiniciar el Deployment de Redis**
   ```bash
   kubectl rollout restart deployment -n redis
   ```
   Reinicia los pods en el deployment de Redis para aplicar cualquier cambio en la configuración o imagen del contenedor sin eliminar manualmente los pods.

---

### Comandos de Ejecución y Verificación de Redis

#### **Ejecutar un Shell en el Pod de Redis**
   ```bash
   kubectl exec -it redis-57b4665c96-bh9jf -n redis -- /bin/sh
   ```
   Este comando abre un terminal dentro del contenedor Redis identificado como `redis-57b4665c96-bh9jf` en el namespace `redis`. Es útil para acceder al sistema de archivos del contenedor, verificar archivos, ejecutar comandos de Redis y hacer diagnósticos.