# Comandos para compilar el modulo

- Compilar modulo -> ```make```

- Subir modulo al kernel -> ```sudo insmod modulo.ko```

- Ver los logs del kernel -> ```cat /proc/sysinfo_201902416```

# Si se desean hacer cambios en el modulo hacer lo siguiente

- Bajar el modulo del kernel -> ```sudo rmmod modulo```

- Para compilar nuevamente el modulo se recomienda hacer ```make clean``` pero el script ```Makefile```borra ya tiene integrado el ```make clean``` antes de compilar

- Luego volver a utilizar los [comandos para compilar el modulo](#comandos-para-compilar-el-modulo)

# Explicacion del codigo 

### Inclusión de encabezados
```c
#include <linux/module.h>
```
- **`#include <linux/module.h>`**: Incluye el encabezado que proporciona las macros y funciones necesarias para crear un módulo de kernel. Esto permite al código ser cargado y descargado dinámicamente en el kernel.

```c
#include <linux/kernel.h>
```
- **`#include <linux/kernel.h>`**: Incluye el encabezado que proporciona varias funciones útiles, como la impresión de mensajes al log del kernel con `printk`, y varias macros estándar.

```c
#include <linux/init.h>
```
- **`#include <linux/init.h>`**: Proporciona macros para la inicialización (`module_init`) y la limpieza (`module_exit`) del módulo del kernel.

```c
#include <linux/proc_fs.h>
```
- **`#include <linux/proc_fs.h>`**: Incluye funciones y estructuras necesarias para interactuar con el sistema de archivos proc, como `proc_create`, que permite crear entradas en `/proc`.

```c
#include <linux/seq_file.h>
```
- **`#include <linux/seq_file.h>`**: Proporciona una API simple para manejar la secuencialidad en la lectura y escritura de archivos del sistema de archivos proc, mediante el uso de la estructura `seq_file`.

```c
#include <linux/mm.h>
```
- **`#include <linux/mm.h>`**: Define estructuras y funciones relacionadas con la gestión de la memoria, como `struct mm_struct`, que describe el estado de la memoria de un proceso.

```c
#include <linux/sched.h>
```
- **`#include <linux/sched.h>`**: Incluye funciones y estructuras relacionadas con la planificación de procesos (`struct task_struct`), permitiendo la interacción con procesos en ejecución.

```c
#include <linux/slab.h>
```
- **`#include <linux/slab.h>`**: Proporciona funciones para la asignación y liberación de memoria en el espacio del kernel, como `kmalloc` y `kfree`.

```c
#include <linux/fs.h>
```
- **`#include <linux/fs.h>`**: Define estructuras y funciones necesarias para interactuar con el sistema de archivos, incluyendo las operaciones de archivo (`struct file_operations`).

```c
#include <linux/sched/signal.h>
```
- **`#include <linux/sched/signal.h>`**: Proporciona acceso a las funciones y estructuras relacionadas con la manipulación de señales de procesos, útiles para gestionar los estados de los procesos.

```c
#include <linux/uaccess.h>
```
- **`#include <linux/uaccess.h>`**: Contiene funciones para copiar datos entre el espacio de usuario y el espacio del kernel de manera segura, como `copy_to_user` y `copy_from_user`.

```c
#include <linux/cgroup.h>
```
- **`#include <linux/cgroup.h>`**: Proporciona funciones y estructuras relacionadas con los cgroups, que permiten agrupar y gestionar los recursos utilizados por un conjunto de procesos.

```c
#include <linux/nsproxy.h>
```
- **`#include <linux/nsproxy.h>`**: Incluye funciones para gestionar los espacios de nombres (`namespaces`), que son una característica de aislamiento utilizada por contenedores como Docker.

```c
#include <linux/jiffies.h>
```
- **`#include <linux/jiffies.h>`**: Proporciona acceso a `jiffies`, que es un contador global que se incrementa a cada tick del reloj del sistema. Se utiliza para medir el tiempo transcurrido en el kernel.

### Definición de información del módulo
```c
MODULE_LICENSE("GPL");
```
- **`MODULE_LICENSE("GPL")`**: Define la licencia bajo la cual se distribuye el módulo. "GPL" significa que el módulo está bajo la Licencia Pública General de GNU, lo que permite su distribución bajo las mismas condiciones.

```c
MODULE_AUTHOR("JUFER");
```
- **`MODULE_AUTHOR("JUFER")`**: Define el nombre del autor del módulo. Es solo para información y no afecta el funcionamiento del módulo.

```c
MODULE_DESCRIPTION("Modulo para leer informacion de memoria, CPU y procesos de contenedores");
```
- **`MODULE_DESCRIPTION("Modulo para leer informacion de memoria, CPU y procesos de contenedores")`**: Proporciona una breve descripción del propósito del módulo. Esta descripción se muestra cuando se consulta información sobre el módulo.

```c
MODULE_VERSION("1.5");
```
- **`MODULE_VERSION("1.5")`**: Define la versión del módulo. Es útil para llevar un control de las actualizaciones y cambios en el módulo.

### Definición de constantes
```c
#define PROC_NAME "sysinfo_201902416"
```
- **`#define PROC_NAME "sysinfo_201902416"`**: Define una constante `PROC_NAME` con el valor `"sysinfo_201902416"`, que será el nombre del archivo que se creará en `/proc`.

```c
#define MAX_CMDLINE_LENGTH 256
```
- **`#define MAX_CMDLINE_LENGTH 256`**: Define una constante `MAX_CMDLINE_LENGTH` con el valor `256`, que representa la longitud máxima de la línea de comandos que se puede capturar para un proceso.

### Función para obtener el uso de CPU de un proceso
```c
static u64 get_cpu_usage_percent(struct task_struct *task) {
```
- **`static u64 get_cpu_usage_percent(struct task_struct *task)`**: Define una función estática que calcula el porcentaje de CPU utilizado por un proceso. La función recibe un puntero `task` a una estructura `task_struct`, que representa el proceso.

```c
    u64 cpu_time_used, total_time;
    u64 cpu_usage = 0;
```
- **`u64 cpu_time_used, total_time; u64 cpu_usage = 0;`**: Declara las variables `cpu_time_used` y `total_time` para almacenar los tiempos de CPU utilizados y el tiempo total transcurrido, respectivamente. `cpu_usage` almacena el uso de CPU calculado.

```c
    cpu_time_used = task->utime + task->stime;
```
- **`cpu_time_used = task->utime + task->stime;`**: Calcula el tiempo total de CPU usado por el proceso sumando `utime` (tiempo de usuario) y `stime` (tiempo de sistema) del proceso.

```c
    total_time = jiffies_to_usecs(get_jiffies_64());
```
- **`total_time = jiffies_to_usecs(get_jiffies_64());`**: Convierte el número de `jiffies` transcurridos desde el arranque en microsegundos, que se usa como el tiempo total.

```c
    if (total_time > 0) {
        cpu_usage = (cpu_time_used * 10000) / total_time;
    }
```
- **`if (total_time > 0) { cpu_usage = (cpu_time_used * 10000) / total_time; }`**: Si `total_time` es mayor que cero, calcula el porcentaje de uso de CPU multiplicando el tiempo de CPU usado por 10000 (para mayor precisión) y dividiéndolo entre el tiempo total.

```c
    return cpu_usage;
}
```
- **`return cpu_usage;`**: Devuelve el valor calculado de `cpu_usage`.

### Función para obtener la línea de comandos de un proceso
```c
static char *get_process_cmdline(struct task_struct *task) {
```
- **`static char *get_process_cmdline(struct task_struct *task)`**: Define una función estática que obtiene la línea de comandos utilizada por un proceso. La función devuelve un puntero a una cadena que contiene la línea de comandos.

```c
    struct mm_struct *mm;
    char *cmdline, *p;
    unsigned long arg_start, arg_end, env_start;
    int i, len;
```
- **`struct mm_struct *mm; char *cmdline, *p; unsigned long arg_start, arg_end, env_start; int i, len;`**: Declara las variables necesarias:
  - `mm`: Puntero a la estructura `mm_struct` del proceso.
  - `cmdline`: Puntero a la cadena que almacenará la línea de comandos.
  - `p`: Puntero utilizado para recorrer `cmdline`.
  - `arg_start`, `arg_end`, `env_start`: Variables para almacenar las direcciones de inicio y fin de los argumentos y el entorno.
  - `i`, `len`: Variables enteras para bucles y longitud.

```c
    cmdline = kmalloc(MAX_CMDLINE_LENGTH, GFP_KERNEL);
    if (!cmdline)
        return NULL;
```
- **`cmdline = kmalloc(MAX_CMDLINE_LENGTH, GFP_KERNEL); if (!cmdline) return NULL;`**: Asigna memoria para `cmdline` usando `kmalloc`, y si la asignación falla, devuelve `NULL`.

```c
    mm = get_task_mm(task);
    if (!mm) {
        kfree(cmdline);
        return NULL;
    }
```
- **`mm = get_task_mm(task); if (!mm) { kfree(cmdline); return NULL; }`**: Obtiene el puntero a `mm_struct` del proceso usando `get_task_mm`. Si falla

, libera la memoria asignada a `cmdline` y devuelve `NULL`.

```c
    arg_start = mm->arg_start;
    arg_end = mm->arg_end;
    env_start = mm->env_start;
```
- **`arg_start = mm->arg_start; arg_end = mm->arg_end; env_start = mm->env_start;`**: Asigna los valores de `arg_start`, `arg_end`, y `env_start` a partir de la estructura `mm_struct`.

```c
    len = min((int)(arg_end - arg_start), MAX_CMDLINE_LENGTH - 1);
```
- **`len = min((int)(arg_end - arg_start), MAX_CMDLINE_LENGTH - 1);`**: Calcula la longitud de la línea de comandos (`len`) como el mínimo entre la longitud real y el tamaño máximo permitido (`MAX_CMDLINE_LENGTH - 1`).

```c
    if (len > 0) {
        if (copy_from_user(cmdline, (char __user *)arg_start, len)) {
            kfree(cmdline);
            return NULL;
        }
    }
```
- **`if (len > 0) { if (copy_from_user(cmdline, (char __user *)arg_start, len)) { kfree(cmdline); return NULL; } }`**: Si `len` es mayor que cero, copia la línea de comandos desde el espacio de usuario al espacio del kernel. Si falla, libera `cmdline` y devuelve `NULL`.

```c
    cmdline[len] = '\0';
```
- **`cmdline[len] = '\0';`**: Asegura que la cadena `cmdline` esté terminada en `\0` (nulo) para indicar el final de la cadena.

```c
    for (i = 0, p = cmdline; i < len; i++, p++) {
        if (*p == '\0')
            *p = ' ';
    }
```
- **`for (i = 0, p = cmdline; i < len; i++, p++) { if (*p == '\0') *p = ' '; }`**: Recorre `cmdline` y reemplaza los caracteres `\0` por espacios (`' '`), haciendo que la línea de comandos sea legible como una sola cadena.

```c
    mmput(mm);
```
- **`mmput(mm);`**: Libera la estructura `mm_struct` después de usarla, devolviendo la referencia a su estado original.

```c
    return cmdline;
}
```
- **`return cmdline;`**: Devuelve el puntero `cmdline` con la línea de comandos procesada.







Vamos a continuar con el análisis detallado del código.

### Estructura de datos para almacenar la información del proceso
```c
struct container_task_info {
    pid_t pid;
    char *comm;
    char *cmdline;
    u64 vsize;
    u64 rss;
    u64 cpu_usage;
    struct container_task_info *next;
};
```
- **`struct container_task_info`**: Define una estructura para almacenar información sobre los procesos dentro de un contenedor.
  - **`pid_t pid;`**: Almacena el ID del proceso.
  - **`char *comm;`**: Almacena el nombre del proceso (`comm`).
  - **`char *cmdline;`**: Almacena la línea de comandos completa que inició el proceso.
  - **`u64 vsize;`**: Almacena el tamaño de la memoria virtual utilizada por el proceso.
  - **`u64 rss;`**: Almacena el tamaño de la memoria física (RSS, Resident Set Size) utilizada por el proceso.
  - **`u64 cpu_usage;`**: Almacena el porcentaje de uso de CPU del proceso.
  - **`struct container_task_info *next;`**: Puntero a la siguiente estructura `container_task_info` en la lista enlazada, permitiendo mantener una lista de procesos.

### Función para liberar la memoria utilizada por la lista de procesos
```c
static void free_container_task_info(struct container_task_info *head) {
    struct container_task_info *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        kfree(tmp->comm);
        kfree(tmp->cmdline);
        kfree(tmp);
    }
}
```
- **`static void free_container_task_info(struct container_task_info *head)`**: Define una función que libera la memoria de todas las estructuras `container_task_info` en la lista enlazada.
  - **`struct container_task_info *tmp;`**: Variable temporal para almacenar el nodo actual.
  - **`while (head)`**: Itera a través de la lista enlazada mientras `head` no sea `NULL`.
  - **`tmp = head; head = head->next;`**: Avanza en la lista almacenando el nodo actual en `tmp` y moviendo `head` al siguiente nodo.
  - **`kfree(tmp->comm); kfree(tmp->cmdline); kfree(tmp);`**: Libera la memoria asociada al nombre del proceso, la línea de comandos y la estructura misma.

### Función para obtener la información de los procesos dentro de un contenedor
```c
static struct container_task_info *get_container_task_info(struct task_struct *task) {
    struct container_task_info *head = NULL, *tail = NULL, *info;
    struct task_struct *child;
    struct list_head *list;
```
- **`static struct container_task_info *get_container_task_info(struct task_struct *task)`**: Define una función que recopila información sobre los procesos que pertenecen a un contenedor.
  - **`struct container_task_info *head = NULL, *tail = NULL, *info;`**: Define punteros para gestionar la lista enlazada de procesos (`head` y `tail`) y un puntero `info` para almacenar la información de cada proceso.
  - **`struct task_struct *child; struct list_head *list;`**: Declara punteros `child` y `list` para iterar a través de los procesos hijos del proceso padre (`task`).

```c
    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);
        info = kmalloc(sizeof(*info), GFP_KERNEL);
        if (!info) {
            free_container_task_info(head);
            return NULL;
        }
```
- **`list_for_each(list, &task->children)`**: Itera sobre la lista de procesos hijos de `task`.
- **`child = list_entry(list, struct task_struct, sibling)`**: Obtiene el proceso hijo (`child`) correspondiente a la entrada de la lista `list`.
- **`info = kmalloc(sizeof(*info), GFP_KERNEL)`**: Asigna memoria para almacenar la información del proceso hijo.
- **`if (!info) { free_container_task_info(head); return NULL; }`**: Si la asignación falla, libera toda la memoria utilizada hasta el momento y devuelve `NULL`.

```c
        info->pid = child->pid;
        info->comm = kstrdup(child->comm, GFP_KERNEL);
        info->cmdline = get_process_cmdline(child);
        info->vsize = child->mm ? child->mm->total_vm << PAGE_SHIFT : 0;
        info->rss = child->mm ? get_mm_rss(child->mm) << PAGE_SHIFT : 0;
        info->cpu_usage = get_cpu_usage_percent(child);
        info->next = NULL;
```
- **`info->pid = child->pid;`**: Almacena el ID del proceso hijo en `info`.
- **`info->comm = kstrdup(child->comm, GFP_KERNEL);`**: Duplica el nombre del proceso (`comm`) y lo almacena en `info`.
- **`info->cmdline = get_process_cmdline(child);`**: Obtiene la línea de comandos del proceso hijo y la almacena en `info`.
- **`info->vsize = child->mm ? child->mm->total_vm << PAGE_SHIFT : 0;`**: Calcula y almacena el tamaño de la memoria virtual utilizada por el proceso.
- **`info->rss = child->mm ? get_mm_rss(child->mm) << PAGE_SHIFT : 0;`**: Calcula y almacena el tamaño de la memoria física (RSS) utilizada por el proceso.
- **`info->cpu_usage = get_cpu_usage_percent(child);`**: Calcula y almacena el uso de CPU del proceso.
- **`info->next = NULL;`**: Inicializa `next` como `NULL` ya que es el último nodo de la lista en este momento.

```c
        if (!head)
            head = tail = info;
        else {
            tail->next = info;
            tail = info;
        }
    }
```
- **`if (!head) head = tail = info; else { tail->next = info; tail = info; }`**: Si `head` es `NULL`, significa que `info` es el primer nodo de la lista, por lo que `head` y `tail` se apuntan a `info`. Si no, se agrega `info` al final de la lista y se actualiza `tail` para que apunte al último nodo.

```c
    return head;
}
```
- **`return head;`**: Devuelve el puntero `head`, que apunta al primer nodo de la lista enlazada que contiene la información de todos los procesos.

### Función para leer y mostrar la información del sistema en `/proc`
```c
static int sysinfo_proc_show(struct seq_file *m, void *v) {
    struct task_struct *task;
    struct container_task_info *info, *head;
    struct sysinfo sys_info;
```
- **`static int sysinfo_proc_show(struct seq_file *m, void *v)`**: Define la función que se ejecutará cuando se lea el archivo `/proc/sysinfo_201902416`. Esta función se encarga de recopilar y mostrar la información del sistema.
  - **`struct task_struct *task;`**: Declara un puntero a `task_struct` para iterar a través de los procesos.
  - **`struct container_task_info *info, *head;`**: Declara punteros `info` y `head` para gestionar la lista de información de procesos.
  - **`struct sysinfo sys_info;`**: Declara una estructura `sysinfo` para almacenar la información general del sistema.

```c
    si_meminfo(&sys_info);
    seq_printf(m, "{\n\"MemTotal\": %lu,\n\"MemFree\": %lu,\n",
               sys_info.totalram << (PAGE_SHIFT - 10),
               sys_info.freeram << (PAGE_SHIFT - 10));
```
- **`si_meminfo(&sys_info);`**: Obtiene información sobre la memoria del sistema y la almacena en `sys_info`.
- **`seq_printf(m, "{\n\"MemTotal\": %lu,\n\"MemFree\": %lu,\n", sys_info.totalram << (PAGE_SHIFT - 10), sys_info.freeram << (PAGE_SHIFT - 10));`**: Imprime el total de memoria (`MemTotal`) y la memoria libre (`MemFree`) en el archivo `/proc/sysinfo_201902416`. Los valores de `sys_info.totalram` y `sys_info.freeram` se desplazan a la derecha (`PAGE_SHIFT - 10`) para convertir de páginas a kilobytes.

```c
    rcu_read_lock();
    for_each_process(task) {
        if (task->flags & PF_KTHREAD)
            continue;
```
- **`rcu_read_lock();`**: Bloquea la lectura de la lista de procesos para evitar que otros hilos modifiquen la lista mientras se itera.
- **`for_each_process(task)`**: Itera a través de todos los procesos en el sistema.
- **`if (task->flags & PF_KTHREAD) continue;`**: Si el proceso es un hilo del kernel (`PF_KTHREAD`), lo omite,

 ya que solo se interesa en los procesos de usuario.

```c
        head = get_container_task_info(task);
        if (!head)
            continue;
```
- **`head = get_container_task_info(task);`**: Llama a la función `get_container_task_info()` para obtener la información de los procesos en un contenedor.
- **`if (!head) continue;`**: Si `head` es `NULL`, continúa con el siguiente proceso, ya que no hay información que mostrar.

```c
        seq_printf(m, "\"PID\": %d,\n\"Comm\": \"%s\",\n\"Cmdline\": \"%s\",\n",
                   task->pid, task->comm, head->cmdline);
        seq_printf(m, "\"Vsize\": %llu,\n\"RSS\": %llu,\n\"CPUUsage\": %llu\n",
                   head->vsize, head->rss, head->cpu_usage);
        seq_printf(m, "},\n");
        free_container_task_info(head);
    }
    rcu_read_unlock();
    return 0;
}
```
- **`seq_printf(m, "\"PID\": %d,\n\"Comm\": \"%s\",\n\"Cmdline\": \"%s\",\n", task->pid, task->comm, head->cmdline);`**: Imprime el PID, el nombre del proceso (`Comm`) y la línea de comandos (`Cmdline`) en el archivo `/proc/sysinfo_201902416`.
- **`seq_printf(m, "\"Vsize\": %llu,\n\"RSS\": %llu,\n\"CPUUsage\": %llu\n", head->vsize, head->rss, head->cpu_usage);`**: Imprime el tamaño de la memoria virtual (`Vsize`), el tamaño de la memoria física (`RSS`), y el uso de CPU (`CPUUsage`) del proceso en el archivo.
- **`seq_printf(m, "},\n");`**: Cierra el bloque JSON de información del proceso.
- **`free_container_task_info(head);`**: Libera la memoria asociada con la lista enlazada de `container_task_info`.
- **`rcu_read_unlock();`**: Desbloquea la lectura de la lista de procesos.
- **`return 0;`**: Devuelve `0` para indicar que la operación fue exitosa.

### Registro del archivo `/proc` y finalización del módulo
```c
static int sysinfo_proc_open(struct inode *inode, struct file *file) {
    return single_open(file, sysinfo_proc_show, NULL);
}
```
- **`static int sysinfo_proc_open(struct inode *inode, struct file *file)`**: Define la función de apertura del archivo `/proc/sysinfo_201902416`. Llama a `single_open()` para iniciar la secuencia de impresión de la información del sistema.

```c
static const struct file_operations sysinfo_proc_fops = {
    .owner = THIS_MODULE,
    .open = sysinfo_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};
```
- **`static const struct file_operations sysinfo_proc_fops`**: Define las operaciones permitidas en el archivo `/proc/sysinfo_201902416`, que incluyen abrir, leer, desplazar el puntero del archivo (`llseek`) y liberar recursos al cerrar.

```c
static int __init sysinfo_init(void) {
    proc_create("sysinfo_201902416", 0, NULL, &sysinfo_proc_fops);
    printk(KERN_INFO "Sysinfo module loaded.\n");
    return 0;
}
```
- **`static int __init sysinfo_init(void)`**: Define la función de inicialización del módulo.
  - **`proc_create("sysinfo_201902416", 0, NULL, &sysinfo_proc_fops);`**: Crea el archivo `/proc/sysinfo_201902416` con las operaciones definidas en `sysinfo_proc_fops`.
  - **`printk(KERN_INFO "Sysinfo module loaded.\n");`**: Imprime un mensaje en el log del kernel indicando que el módulo se ha cargado correctamente.
  - **`return 0;`**: Devuelve `0` para indicar que la inicialización fue exitosa.

```c
static void __exit sysinfo_exit(void) {
    remove_proc_entry("sysinfo_201902416", NULL);
    printk(KERN_INFO "Sysinfo module unloaded.\n");
}
```
- **`static void __exit sysinfo_exit(void)`**: Define la función de limpieza del módulo.
  - **`remove_proc_entry("sysinfo_201902416", NULL);`**: Elimina el archivo `/proc/sysinfo_201902416`.
  - **`printk(KERN_INFO "Sysinfo module unloaded.\n");`**: Imprime un mensaje en el log del kernel indicando que el módulo se ha descargado correctamente.

```c
module_init(sysinfo_init);
module_exit(sysinfo_exit);
```
- **`module_init(sysinfo_init);`**: Especifica que la función `sysinfo_init` se ejecutará al cargar el módulo.
- **`module_exit(sysinfo_exit);`**: Especifica que la función `sysinfo_exit` se ejecutará al descargar el módulo.

