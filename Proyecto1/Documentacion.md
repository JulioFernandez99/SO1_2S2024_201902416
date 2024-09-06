# <div align="center"><img src="https://user-images.githubusercontent.com/74038190/235223599-0eadbd7c-c916-4f24-af9d-9242730e6172.gif" width="30px" /> &nbsp; Instalacion &nbsp; <img src="https://user-images.githubusercontent.com/74038190/235223599-0eadbd7c-c916-4f24-af9d-9242730e6172.gif" width="30px" />



## Script creador de contenedores
Este script debe de agregarse al cronjob para que pueda ejecutarse cada 2 minutos, para ello seguir los siguientes comandos:
- Abrir la terminar y ejecutar ```crontab -e```.
- Seguido a ello agregar lo siguiente al final del archivo,para guardar los cambios presiocona ```Ctrl + o``` y ```Ctrl + x``` para salir.

  <details>
        <summary>Codigo cronjob</summary>

        #*/2 * * * * /home/julio-fernandez/Escritorio/Proyectos_2S/SOPES/Proyecto1-Oficial/Contenedores/script_contenedores.sh
        #* * * * * sleep 30; /home/julio-fernandez/Escritorio/Proyectos_2S/SOPES/Proyecto1-Oficial/Contenedores/script_contenedores.sh
    </details>

## Modulo de kernel
Este modulo se encarga de generar el archivo ```sysinfo_201902416```, que muestra la informacion del sistema y de los contenedores, para ello seguir los siguentes pasos:

- Compilar el codigo de c con el comando ```Makefile```
- Luego de compilar el codigo ejecutar el comando ```sudo insmod modulo.ko```


## Servicio de rust
Este servicio se encarga de analizar el archivo ```sysinfo_201902416``` y ordenar los contenedores y eliminarlos.

- Para compilar el servicio ejecutar el comando ```cargo build```.
- Para correr el servicio ejecutar el comando ```cargo run```.
- De ser necesario recompilar el codigo primero deben eliminarse todos los archivos con el comando ```cargo clean```.

## Servidor de logs
Este servidor se encarga recibir los logs del servicio de rust y procesarlos para hacer las graficas y guardar los logs. Para poder compilar el codigo utilizar el comando ```docker compose build``` y seguido a ella levantar el servidor con el comando ```docker compose up -d```.

![](https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif)
<br>
<br>

# <div align="center"><img src="https://user-images.githubusercontent.com/74038190/212284087-bbe7e430-757e-4901-90bf-4cd2ce3e1852.gif" width="30px" /> &nbsp; Explicacion &nbsp; <img src="https://user-images.githubusercontent.com/74038190/212284087-bbe7e430-757e-4901-90bf-4cd2ce3e1852.gif" width="30px" />

## Script creador de contenedores


```bash
#!/bin/bash
```
Esta línea indica que el script debe ser interpretado por el intérprete de comandos Bash.


```bash
# Array de imágenes disponibles
images=("high-image-1" "high-image-2" "low-image-1" "low-image-2")
```
Aquí se define un array llamado `images` que contiene los nombres de las imágenes Docker disponibles que se utilizarán para crear los contenedores.

```bash
# Función para generar un nombre aleatorio
random_name() {
    echo "container_$(cat /dev/urandom | tr -dc 'a-z0-9' | fold -w 8 | head -n 1)"
}
```
Esta función, `random_name`, genera un nombre aleatorio para cada contenedor. Utiliza `/dev/urandom` para obtener una secuencia de caracteres aleatorios, filtra solo los caracteres alfanuméricos (`a-z0-9`), y luego toma los primeros 8 caracteres. Este nombre se precede con `container_` para formar el nombre del contenedor.

```bash
# Crear 10 contenedores
for i in {1..10}
do
    # Seleccionar una imagen aleatoria
    random_image=${images[$RANDOM % ${#images[@]}]}
    
    # Generar un nombre aleatorio
    name=$(random_name)
    
    # Intentar crear el contenedor
    if docker run -d --name $name $random_image; then
        echo "Created container $name from image $random_image"
    else
        echo "Failed to create container $name from image $random_image"
    fi
    
    # Opcional: Agregar un retraso para evitar problemas de rapidez
    sleep 1
done
```
Este bloque de código crea 10 contenedores Docker en un bucle:

1. `for i in {1..10}`: Un bucle que se ejecuta 10 veces.
2. `random_image=${images[$RANDOM % ${#images[@]}]}`: Selecciona una imagen aleatoria del array `images`. `RANDOM` es una variable interna que genera un número aleatorio, y `% ${#images[@]}` garantiza que el índice esté dentro del rango del array.
3. `name=$(random_name)`: Genera un nombre aleatorio para el contenedor utilizando la función `random_name`.
4. `if docker run -d --name $name $random_image; then`: Intenta crear un contenedor en segundo plano (`-d`) con el nombre y la imagen especificados. Si el comando tiene éxito, imprime un mensaje de éxito; si falla, imprime un mensaje de error.
5. `sleep 1`: Introduce un retraso de 1 segundo entre la creación de cada contenedor para evitar problemas que puedan surgir de intentar crear contenedores demasiado rápido.


## Modulo de kernel

Este módulo crea un archivo en el sistema de archivos `/proc` que proporciona información sobre la memoria y el uso de la CPU de los procesos en formato JSON.

### Inclusiones de Archivos de Cabecera

```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h> 
#include <linux/init.h>
#include <linux/proc_fs.h> 
#include <linux/seq_file.h> 
#include <linux/mm.h> 
#include <linux/sched.h> 
#include <linux/timer.h> 
#include <linux/jiffies.h> 
#include <linux/uaccess.h>
#include <linux/tty.h>
#include <linux/sched/signal.h>
#include <linux/fs.h>        
#include <linux/slab.h>      
#include <linux/sched/mm.h>
#include <linux/binfmts.h>
#include <linux/timekeeping.h>
```

Estas cabeceras proporcionan funciones y estructuras necesarias para interactuar con el sistema operativo, como el manejo de archivos en `/proc`, acceso a la memoria, y operaciones con procesos.

### Metadatos del Módulo

```c
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tu Nombre");
MODULE_DESCRIPTION("Modulo para leer informacion de memoria y CPU en JSON");
MODULE_VERSION("1.0");
```

Define los metadatos del módulo, incluyendo la licencia, el autor, la descripción y la versión del módulo.

### Definiciones de Constantes

```c
#define PROC_NAME "sysinfo_201902416"
#define MAX_CMDLINE_LENGTH 256
#define CONTAINER_ID_LENGTH 64
```

Define constantes para el nombre del archivo `/proc`, la longitud máxima de la línea de comandos y la longitud del ID del contenedor.

### Función para Obtener la Línea de Comandos de un Proceso

```c
static char *get_process_cmdline(struct task_struct *task) {
    struct mm_struct *mm;
    char *cmdline, *p;
    unsigned long arg_start, arg_end, env_start;
    int i, len;

    cmdline = kmalloc(MAX_CMDLINE_LENGTH, GFP_KERNEL);
    if (!cmdline)
        return NULL;

    mm = get_task_mm(task);
    if (!mm) {
        kfree(cmdline);
        return NULL;
    }

    down_read(&mm->mmap_lock);
    arg_start = mm->arg_start;
    arg_end = mm->arg_end;
    env_start = mm->env_start;
    up_read(&mm->mmap_lock);

    len = arg_end - arg_start;
    if (len > MAX_CMDLINE_LENGTH - 1)
        len = MAX_CMDLINE_LENGTH - 1;

    if (access_process_vm(task, arg_start, cmdline, len, 0) != len) {
        mmput(mm);
        kfree(cmdline);
        return NULL;
    }

    cmdline[len] = '\0';

    p = cmdline;
    for (i = 0; i < len; i++)
        if (p[i] == '\0')
            p[i] = ' ';

    mmput(mm);
    return cmdline;
}
```

Esta función obtiene la línea de comandos de un proceso dado (`task`). Utiliza la estructura `mm_struct` para acceder a la memoria del proceso y lee la línea de comandos de la memoria virtual del proceso.

### Función para Mostrar la Información en el Archivo `/proc`

```c
static int sysinfo_show(struct seq_file *m, void *v) {
    struct sysinfo si;
    struct task_struct *task;
    unsigned long total_jiffies = jiffies;
    int first_process = 1;

    unsigned long total_mem_kb, free_mem_kb, used_mem_kb;

    si_meminfo(&si);
    total_mem_kb = si.totalram * (PAGE_SIZE / 1024);
    free_mem_kb = si.freeram * (PAGE_SIZE / 1024);
    used_mem_kb = total_mem_kb - free_mem_kb;

    seq_printf(m, "{\n");
    seq_printf(m, "    \"Memory\": {\n");
    seq_printf(m, "    \"Total RAM(KB)\": %lu,\n", total_mem_kb);
    seq_printf(m, "    \"Free RAM(KB)\": %lu,\n", free_mem_kb);
    seq_printf(m, "    \"In Use RAM(KB)\": %lu\n", used_mem_kb);
    seq_printf(m, "    },\n");
    seq_printf(m, "\"Processes\": [\n");

    for_each_process(task) {
        if (strcmp(task->comm, "containerd-shim") == 0) {
            unsigned long vsz = 0;
            unsigned long rss = 0;
            unsigned long totalram = si.totalram * 4;
            unsigned long mem_usage = 0;
            unsigned long cpu_usage = 0;
            char *cmdline = NULL;

            if (task->mm) {
                vsz = task->mm->total_vm << (PAGE_SHIFT - 10);
                rss = get_mm_rss(task->mm) << (PAGE_SHIFT - 10);
                mem_usage = (rss * 10000) / totalram;
            }

            unsigned long total_time = task->utime + task->stime;
            cpu_usage = (total_time * 10000) / total_jiffies;
            cmdline = get_process_cmdline(task);

            if (!first_process) {
                seq_printf(m, ",\n");
            } else {
                first_process = 0;
            }

            seq_printf(m, "  {\n");
            seq_printf(m, "    \"PID\": %d,\n", task->pid);
            seq_printf(m, "    \"Name\": \"%s\",\n", task->comm);
            seq_printf(m, "    \"Cmdline\": \"%s\",\n", cmdline ? cmdline : "N/A");
            seq_printf(m, "    \"MemoryUsage\": %lu.%02lu,\n", mem_usage / 100, mem_usage % 100);
            seq_printf(m, "    \"CPUUsage\": %lu.%02lu\n", cpu_usage / 100, cpu_usage % 100);
            seq_printf(m, "  }");

            if (cmdline) {
                kfree(cmdline);
            }
        }
    }

    seq_printf(m, "\n]\n}\n");
    return 0;
}
```

Esta función muestra la información de memoria y de procesos en formato JSON. Primero, obtiene la información de memoria total y utilizada. Luego, itera sobre los procesos del sistema, filtrando solo aquellos cuyo nombre es `"containerd-shim"`. Para cada proceso, obtiene la información de uso de memoria y CPU y la línea de comandos, y formatea esta información en JSON.

### Función para Abrir el Archivo `/proc`

```c
static int sysinfo_open(struct inode *inode, struct file *file) {
    return single_open(file, sysinfo_show, NULL);
}
```

Esta función se llama cuando se abre el archivo `/proc`. Utiliza la función `single_open` para abrir el archivo y asociarlo con la función `sysinfo_show` que muestra la información.

### Estructura de Operaciones del Archivo `/proc`

```c
static const struct proc_ops sysinfo_ops = {
    .proc_open = sysinfo_open,
    .proc_read = seq_read,
};
```

Define las operaciones para el archivo `/proc`, incluyendo `proc_open` y `proc_read`. `proc_read` se utiliza para leer el contenido del archivo.

### Función de Inicialización del Módulo

```c
static int __init sysinfo_init(void) {
    proc_create(PROC_NAME, 0, NULL, &sysinfo_ops);
    printk(KERN_INFO "sysinfo_json modulo cargado\n");
    return 0;
}
```

Esta función se llama al cargar el módulo. Crea el archivo `/proc` con el nombre definido en `PROC_NAME` y asocia las operaciones definidas en `sysinfo_ops`.

### Función de Limpieza del Módulo

```c
static void __exit sysinfo_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "sysinfo_json modulo desinstalado\n");
}
```

Esta función se llama al descargar el módulo. Elimina el archivo `/proc` creado.

### Inicialización y Salida del Módulo

```c
module_init(sysinfo_init);
module_exit(sysinfo_exit);
```

Estas macros definen las funciones de inicialización y salida del módulo.


## Servicio de rust

### Modelos de estructuras

### Importaciones

```python
from pydantic import BaseModel # type: ignore
from typing import List, Optional
```

- **`BaseModel`**: La clase base de Pydantic para definir modelos de datos con validación automática.
- **`List`**: Un tipo genérico para definir listas de elementos de un tipo específico.
- **`Optional`**: Un tipo de unión que indica que un campo puede ser de un tipo especificado o `None`.

### Clases de Modelos
Estos modelos definen las estructuras que debe de leer del JSON.
#### 1. `ContainerInfo`

```python
class ContainerInfo(BaseModel):
    pid: int
    container_id: Optional[str]  # Optional en caso de que algunos campos sean opcionales
    name: str
    memory_usage: float
    cpu_usage: float
```

- **`pid`**: El identificador del proceso, obligatorio y de tipo entero.
- **`container_id`**: El identificador del contenedor, es opcional (`Optional[str]`), es decir, puede ser un `str` o `None`.
- **`name`**: El nombre del contenedor, obligatorio y de tipo cadena (`str`).
- **`memory_usage`**: El uso de memoria del contenedor en unidades de punto flotante (`float`), obligatorio.
- **`cpu_usage`**: El uso de CPU del contenedor en unidades de punto flotante (`float`), obligatorio.

#### 2. `ContainerHigh`

```python
class ContainerHigh(BaseModel):
    PID: int
    Name: str
    Cmdline: str
    MemoryUsage: float
    CPUUsage: float
```

- **`PID`**: El identificador del proceso, obligatorio y de tipo entero.
- **`Name`**: El nombre del contenedor, obligatorio y de tipo cadena (`str`).
- **`Cmdline`**: La línea de comandos que se utilizó para iniciar el contenedor, obligatorio y de tipo cadena (`str`).
- **`MemoryUsage`**: El uso de memoria del contenedor en unidades de punto flotante (`float`), obligatorio.
- **`CPUUsage`**: El uso de CPU del contenedor en unidades de punto flotante (`float`), obligatorio.

#### 3. `MemoryInfo`

```python
class MemoryInfo(BaseModel):
    total_ram_kb: int
    free_ram_kb: int
    in_use_ram_kb: int
```

- **`total_ram_kb`**: La cantidad total de RAM en kilobytes (`int`), obligatorio.
- **`free_ram_kb`**: La cantidad de RAM libre en kilobytes (`int`), obligatorio.
- **`in_use_ram_kb`**: La cantidad de RAM en uso en kilobytes (`int`), obligatorio.

#### 4. `SystemLog`

```python
class SystemLog(BaseModel):
    current_time: str
    killed_containers: List[ContainerInfo]
    high_consumption_containers: List[ContainerHigh]
    low_consumption_containers: List[ContainerHigh]
    memory_info: MemoryInfo
```

- **`current_time`**: La hora actual cuando se genera el log, en formato de cadena (`str`), obligatorio.
- **`killed_containers`**: Una lista de objetos `ContainerInfo` que representa contenedores que han sido eliminados (`List[ContainerInfo]`), obligatorio.
- **`high_consumption_containers`**: Una lista de objetos `ContainerHigh` que representa contenedores con alto consumo de recursos (`List[ContainerHigh]`), obligatorio.
- **`low_consumption_containers`**: Una lista de objetos `ContainerHigh` que representa contenedores con bajo consumo de recursos (`List[ContainerHigh]`), obligatorio.
- **`memory_info`**: Un objeto `MemoryInfo` que contiene información sobre el uso de memoria del sistema (`MemoryInfo`), obligatorio.

### Resumen

Estos modelos de datos definen la estructura esperada para los logs del sistema y proporcionan validación automática para los datos entrantes. Cada campo en los modelos tiene un tipo de dato específico y algunos campos son opcionales, lo que permite manejar la variabilidad en los datos de entrada.

Si necesitas más detalles o tienes alguna pregunta adicional, ¡no dudes en preguntar!


### Importaciones

```rust
use std::fs::File;
use std::io::{self, Read};
use std::path::Path;
use chrono::{DateTime, Local};
use serde::{Deserialize, Serialize};
```
Estas importaciones traen en el ámbito el manejo de archivos y entradas/salidas, el módulo `chrono` para trabajar con fechas y horas, y `serde` para la serialización y deserialización de JSON.

### Estructuras (Structs)

```rust
#[derive(Debug, Serialize, Deserialize)]
struct SystemInfo {
    #[serde(rename = "Processes")]
    processes: Vec<Process>
}

#[derive(Debug, Serialize, Deserialize, Clone, PartialEq)]
struct Process {
    #[serde(rename = "PID")]
    pid: u32,
    #[serde(rename = "Name")]
    name: String,
    #[serde(rename = "Cmdline")]
    cmd_line: String,
    #[serde(rename = "MemoryUsage")]
    memory_usage: f64,
    #[serde(rename = "CPUUsage")]
    cpu_usage: f64,
}

#[derive(Debug, Serialize, Clone)]
struct LogProcess {
    pid: u32,
    container_id: String,
    name: String,
    memory_usage: f64,
    cpu_usage: f64,
}

#[derive(Debug, Serialize)]
struct AnalyzerOutput {
    current_time: String,
    killed_containers: Vec<LogProcess>,
    high_consumption_containers: Vec<Process>,
    low_consumption_containers: Vec<Process>,
    memory_info: MemoryInfo,
}

#[derive(Debug, Serialize, Deserialize)]
struct MemoryInfo {
    total_ram_kb: u64,
    free_ram_kb: u64,
    in_use_ram_kb: u64,
}
```

Estas estructuras definen los datos que se manejarán en el programa:

- `SystemInfo` contiene una lista de `Process`.
- `Process` representa un proceso del sistema.
- `LogProcess` es similar a `Process` pero se usa para registrar información sobre los contenedores que se matan.
- `AnalyzerOutput` es la salida del análisis, que incluye la hora actual, los contenedores eliminados, contenedores de alto y bajo consumo y la información de memoria.
- `MemoryInfo` contiene información sobre el uso de la memoria.

### Implementaciones para `Process`

```rust
impl Process {
    fn get_container_id(&self) -> &str {
        let parts: Vec<&str> = self.cmd_line.split_whitespace().collect();
        for (i, part) in parts.iter().enumerate() {
            if *part == "-id" {
                if let Some(id) = parts.get(i + 1) {
                    return id;
                }
            }
        }
        "N/A"
    }

    fn get_image_name(&self) -> &str {
        let parts: Vec<&str> = self.cmd_line.split_whitespace().collect();
        for (i, part) in parts.iter().enumerate() {
            if *part == "-image" {
                if let Some(image) = parts.get(i + 1) {
                    return image;
                }
            }
        }
        "N/A"
    }
}
```

Estos métodos de instancia para `Process` extraen el ID del contenedor y el nombre de la imagen desde la línea de comandos del proceso.

```rust
impl Eq for Process {}

impl Ord for Process {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.cpu_usage.partial_cmp(&other.cpu_usage).unwrap_or(std::cmp::Ordering::Equal)
            .then_with(|| self.memory_usage.partial_cmp(&other.memory_usage).unwrap_or(std::cmp::Ordering::Equal))
    }
}

impl PartialOrd for Process {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}
```

Estas implementaciones permiten comparar y ordenar `Process` por uso de CPU y memoria. `PartialOrd` y `Ord` permiten ordenar los procesos por consumo de recursos.

### Funciones

```rust
fn kill_container(id: &str) -> Result<std::process::Output, io::Error> {
    println!("Matando contenedor con id: {}", id);
    let output = std::process::Command::new("sudo")
        .arg("docker")
        .arg("stop")
        .arg(id)
        .output()?;

    Ok(output)
}
```
Esta función detiene un contenedor Docker usando `sudo docker stop`.

```rust
fn print_current_time() -> String {
    let now: DateTime<Local> = Local::now();
    now.to_rfc3339()
}
```
Devuelve la fecha y hora actual en formato RFC 3339.

```rust
fn analyzer(system_info: SystemInfo) -> AnalyzerOutput {
    let mut log_proc_list: Vec<LogProcess> = Vec::new();
    let mut processes_list: Vec<Process> = system_info.processes;

    // Filtrar el contenedor 'servidor-log_registry'
    let special_container_id = "servidor-log_registry";
    processes_list.retain(|process| process.get_image_name() != special_container_id);

    // Verificar si el número total de contenedores (excluyendo el especial) es exactamente 5
    if processes_list.len() == 6 {
        // No eliminar ningún contenedor si hay exactamente 5 (excluyendo el especial)
        let output = AnalyzerOutput {
            current_time: print_current_time(),
            killed_containers: Vec::new(), // No se eliminaron contenedores
            high_consumption_containers: processes_list.clone(),
            low_consumption_containers: Vec::new(),
            memory_info: read_memory_info().unwrap_or_else(|_| MemoryInfo {
                total_ram_kb: 0,
                free_ram_kb: 0,
                in_use_ram_kb: 0,
            }),
        };
        return output;
    }

    // Ordenar la lista de procesos por consumo (primero por CPU y luego por memoria)
    processes_list.sort_by(|a, b| {
        b.cpu_usage
            .partial_cmp(&a.cpu_usage)
            .unwrap_or(std::cmp::Ordering::Equal)
            .then_with(|| b.memory_usage.partial_cmp(&a.memory_usage).unwrap_or(std::cmp::Ordering::Equal))
    });

    // Dividir en alto consumo y bajo consumo
    let total_processes = processes_list.len();
    let half_point = total_processes / 2;

    let high_consumption_containers: Vec<Process> = processes_list.iter().take(half_point).cloned().collect();
    let low_consumption_containers: Vec<Process> = processes_list.iter().skip(half_point).cloned().collect();

    // Seleccionamos los 2 de alto consumo y 3 de bajo consumo
    let mut final_high_consumption: Vec<Process> = high_consumption_containers.iter().take(2).cloned().collect();
    let mut final_low_consumption: Vec<Process> = low_consumption_containers.iter().take(3).cloned().collect();

    // Si hay menos de 2 en alto o 3 en bajo, ajustar para completar los 5
    while final_high_consumption.len() < 2 && !low_consumption_containers.is_empty() {
        if let Some(p) = low_consumption_containers.get(final_high_consumption.len()) {
            final_high_consumption.push(p.clone());
        }
    }
    
    while final_low_consumption.len() < 3 && !high_consumption_containers.is_empty() {
        if let Some(p) = high_consumption_containers.get(final_low_consumption.len()) {
            final_low_consumption.push(p.clone());
        }
    }

    // Evitar eliminar contenedores cuyo ID coincide con los primeros 12 caracteres de "e1dab6247ccb"
    let skip_id_prefix: &str = "e1dab6247ccb";
    let mut containers_to_kill: Vec<Process> = Vec::new();

    for process in processes_list.iter() {
        let container_id = process.get_container_id().to_string();
        
        // Saltar si está en las listas de bajo o alto consumo
        if final_low_consumption.contains(process) || final_high_consumption.contains(process) {
            continue;
        }

        // Saltar si el ID del contenedor coincide con el prefijo que queremos evitar
        if container_id.starts_with(skip_id_prefix) {
            continue;
        }

        containers_to_kill.push(process.clone());
    }

    // Asegúrate de que no se eliminen más de 5 contenedores
    let max_to_kill = 5;
    if containers_to_kill.len() > max_to_kill {
        containers_to_kill.truncate(max_to_kill);
    }

    // Ahora matamos los contenedores
    for process in containers_to_kill.iter() {
        let container_id = process.get_container_id().to_string();
        let log_process = LogProcess {
            pid: process.pid,
            container_id: container_id.clone(),
            name: process.name.clone(),
            memory_usage: process.memory_usage,
            cpu_usage: process.cpu_usage,
        };
        log_proc_list.push(log_process.clone());

        if let Err(e) = kill_container(&container_id) {
            eprintln!("Error al matar el contenedor: {}", e);
        }
    }

    // Leer la información de memoria
    let memory_info = read_memory_info().unwrap_or_else(|_| MemoryInfo {
        total_ram_kb: 0,
        free_ram_kb: 0,
        in_use_ram_kb: 0,


    });

    AnalyzerOutput {
        current_time: print_current_time(),
        killed_containers: log_proc_list,
        high_consumption_containers: final_high_consumption,
        low_consumption_containers: final_low_consumption,
        memory_info,
    }
}
```

La función `analyzer` realiza los siguientes pasos:
1. Filtra los contenedores para excluir el especial (`servidor-log_registry`).
2. Verifica si el número total de contenedores excluyendo el especial es exactamente 5; en caso afirmativo, devuelve esa información sin eliminar contenedores.
3. Ordena los procesos por uso de CPU y memoria.
4. Divide los procesos en alto y bajo consumo.
5. Selecciona los 2 de alto consumo y 3 de bajo consumo, ajustando si es necesario.
6. Evita la eliminación de contenedores con un ID específico y limita la eliminación a 5 contenedores.
7. Mata los contenedores seleccionados y registra esta información.
8. Lee la información de memoria y devuelve un `AnalyzerOutput` con todos los datos relevantes.

### Función para leer información de memoria

```rust
fn read_memory_info() -> io::Result<MemoryInfo> {
    let path = Path::new("/proc/meminfo");
    let mut file = File::open(&path)?;
    let mut contents = String::new();
    file.read_to_string(&mut contents)?;

    let mut total_ram_kb = 0;
    let mut free_ram_kb = 0;
    let mut in_use_ram_kb = 0;

    for line in contents.lines() {
        if let Some(pos) = line.find(":") {
            let (key, value) = line.split_at(pos);
            let value = value.trim_start_matches(':').trim().to_string();
            let value_kb = value.parse::<u64>().unwrap_or(0);
            
            match key {
                "MemTotal" => total_ram_kb = value_kb,
                "MemFree" => free_ram_kb = value_kb,
                "MemAvailable" => in_use_ram_kb = total_ram_kb - value_kb,
                _ => (),
            }
        }
    }

    Ok(MemoryInfo {
        total_ram_kb,
        free_ram_kb,
        in_use_ram_kb,
    })
}
```

Esta función lee el archivo `/proc/meminfo`, analiza su contenido y devuelve la información de memoria total, libre y en uso.


## Servidor de logs

El código en FastAPI está diseñado para recibir logs a través de una solicitud POST, almacenarlos en un archivo JSON y generar un gráfico basado en el uso de CPU de los contenedores.

### Importaciones

```python
from fastapi import FastAPI, HTTPException  # type: ignore
import os
import json
from models.models import SystemLog
import matplotlib.pyplot as plt
```

Estas importaciones incluyen FastAPI para construir la API, `os` y `json` para manejar archivos, `SystemLog` de tu modelo para validar los datos de entrada, y `matplotlib` para generar gráficos.

### Inicialización de la aplicación FastAPI

```python
app = FastAPI()
```
Aquí se crea una instancia de la aplicación FastAPI.

### Ruta raíz

```python
@app.get("/")
def read_root():
    return {"Hello": "World!"}
```
Esta ruta simple responde con un saludo al acceder a la raíz del API.

### Ruta para recibir logs

```python
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
```

- **Lectura de Logs**: Verifica si el archivo `logs.json` existe y, si es así, carga los logs existentes. Si no, inicializa una lista vacía.
- **Agregar Nuevo Log**: Convierte el nuevo log (recibido como un objeto `SystemLog`) en un diccionario y lo agrega a la lista de logs.
- **Guardar Logs**: Guarda la lista actualizada de logs en el archivo `logs.json`.
- **Generar Gráfico**: Llama a `generate_plot` para crear un gráfico basado en los logs.
- **Manejo de Errores**: Si ocurre un error, se registra en `error.log` y se devuelve una respuesta de error 500.

### Función para generar gráficos

```python
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
```

- **Inicialización**: Prepara listas para almacenar el uso de CPU y etiquetas para los contenedores.
- **Procesamiento de Logs**: Recorre cada tipo de contenedor en los logs (`killed_containers`, `high_consumption_containers`, `low_consumption_containers`) y agrega los datos de uso de CPU a las listas.
- **Generar Gráfico**: Crea un gráfico de barras que muestra el uso de CPU para cada contenedor.
- **Guardar Gráfico**: Guarda el gráfico como una imagen PNG en la ruta especificada.
- **Manejo de Errores**: Si ocurre un error durante la generación del gráfico, se registra en `error.log`.

### Consideraciones

1. **Ruta de Archivo**: Asegúrate de que la ruta para guardar el gráfico (`/code/logs/plot.png`) existe en tu entorno de ejecución o cámbiala a una ruta válida.
2. **Estructura de `SystemLog`**: Asegúrate de que la estructura del modelo `SystemLog` en `models/models.py` coincida con la que estás esperando recibir en la solicitud.
3. **Errores de Generación de Gráficos**: Se manejan en `error.log`, pero considera también añadir manejo de errores más específicos o pruebas para asegurar la integridad de los datos.










