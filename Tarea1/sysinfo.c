/*
    cargar un módulo en kernel el cual cree un archivo en /proc/ 
    que imprima las metricas del sistema operativo.
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h> // trae las funciones para crear archivos en /proc
#include <linux/seq_file.h> // trae las funciones para escribir en archivos en /proc
#include <linux/mm.h> // trae las funciones para manejar la memoria
#include <linux/sched.h> // trae las funciones para manejar los procesos
#include <linux/timer.h> // trae las funciones para manejar los timers
#include <linux/jiffies.h> // trae las funciones para manejar los jiffies, que son los ticks del sistema

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JUFER");
MODULE_DESCRIPTION("Modulo para leer informacion de memoria y CPU");
MODULE_VERSION("1.0");


#define PROC_NAME "sysinfo" // nombre del archivo en /proc

static void show_process_tree(struct seq_file *m, struct task_struct *task, int level) {
    struct task_struct *child;
    struct list_head *list;
    int i;

    
    for (i = 0; i < level; i++) {
        seq_printf(m, "│   ");
    }
    
    
    if (level > 0) {
        seq_printf(m, "├── ");
    }
    seq_printf(m, "PID: %d, Nombre: %s\n", task->pid, task->comm);

    
    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);
        show_process_tree(m, child, level + 1); // Llamada recursiva para el hijo
    }
}

/*
    Explicacion de la función show_process_tree:
    - m es un puntero a la estructura seq_file, que es la estructura que se encarga de escribir en el archivo en /proc
    - task es un puntero a la estructura task_struct, que es la estructura que contiene la información de un proceso
    - level es un entero que indica el nivel del proceso en el árbol de procesos
    - child es un puntero a la estructura task_struct, que es la estructura que contiene la información de un proceso hijo
    - list es un puntero a la estructura list_head, que es la estructura que contiene la información de una lista
    - i es un entero que se utiliza para iterar sobre los niveles del árbol de procesos
    - list_for_each es una macro que se encarga de iterar sobre una lista
    - list es un puntero a la estructura list_head, que es la estructura que contiene la información de una lista
    - &task->children es un puntero a la lista de hijos de un proceso
    - children es un puntero a la lista de hijos de un proceso
    - child es un puntero a la estructura task_struct, que es la estructura que contiene la información de un proceso hijo
    - list_entry es una macro que se encarga de obtener un puntero a la estructura que contiene un elemento de una lista
    - child es un puntero a la estructura task_struct, que es la estructura que contiene la información de un proceso hijo
    - sibling es un puntero a la estructura list_head, que es la estructura que contiene la información de un hermano de un proceso
    - show_process_tree es una función que se encarga de mostrar el árbol de procesos, esta función 
    se llama recursivamente para mostrar los hijos de un proceso
    - level + 1 es el nivel del proceso hijo en el árbol de procesos
*/

static int sysinfo_show(struct seq_file *m, void *v) {
     struct sysinfo si; // estructura que contiene la informacion de la memoria

    si_meminfo(&si); // obtiene la informacion de la memoria

    /*  
        El seq_printf se encarga de escribir en el archivo en /proc
        - m: es el archivo en /pro
    */

    seq_printf(m, "Total RAM: %lu KB\n", si.totalram * 4);
    seq_printf(m, "Free RAM: %lu KB\n", si.freeram * 4);
    seq_printf(m, "Shared RAM: %lu KB\n", si.sharedram * 4);
    seq_printf(m, "Buffer RAM: %lu KB\n", si.bufferram * 4);
    seq_printf(m, "Total Swap: %lu KB\n", si.totalswap * 4);
    seq_printf(m, "Free Swap: %lu KB\n", si.freeswap * 4);

    seq_printf(m, "Number of processes: %d\n", num_online_cpus());

    show_process_tree(m, &init_task, 0); // Empezar con el proceso init (PID 1)

    return 0;
    
    /*
        m es un puntero a la estructura seq_file, 
        que es la estructura que se encarga de escribir en el archivo en /proc

        v es un puntero a void, que es un puntero a la estructura mm_struct,

        mm_struct es la estructura que contiene las metricas del sistema
        %lu: es el formato para imprimir un unsigned long,o sea un entero sin signo
        si.totalram: es la cantidad total de memoria RAM en el sistema y 
        se multiplica por 4 para convertirlo a KB, 
        si lo quiere en Gb se multiplica por 4 y se divide por 1024

    */
};


static int sysinfo_open(struct inode *inode, struct file *file) {
    return single_open(file, sysinfo_show, NULL);
}

/*
    Explicacion de la funcion sysinfo_open:
    - inode: es un puntero a la estructura inode, que 
    es la estructura que contiene la información del archivo en /proc

    - file: es un puntero a la estructura file, que es la estructura que
    contiene la información del archivo en /proc

    - single_open: es una función que se encarga de abrir el archivo en /proc
    - file: es el archivo en /proc
    - sysinfo_show: es la función que se encarga de escribir en el archivo en /proc
    - NULL: es un puntero a void, que es un puntero a la estructura mm_struct

*/


static const struct proc_ops sysinfo_ops = {
    .proc_open = sysinfo_open,
    .proc_read = seq_read,
};

/*
    Explicacion de la estructura sysinfo_fops:

    - proc_ops: es una estructura que contiene las funciones para 
    abrir y leer un archivo en /proc

    - sysinfo_open: es un puntero a la función sysinfo_open, que se
    encarga de abrir el archivo en /proc

    - proc_open: es un puntero a la función sysinfo_open, que se 
    encarga de abrir el archivo en /proc

    - proc_read: es un puntero a la función seq_read, 
    que se encarga de leer el archivo en /proc
*/


static int __init sysinfo_init(void) {
    proc_create(PROC_NAME, 0, NULL, &sysinfo_ops);
    printk(KERN_INFO "sysinfo module loaded\n");
    return 0;
}


/*
    Explicacion de la función sysinfo_init:
    - __init: es un macro que indica que la función se ejecuta al cargar el módulo
    - sysinfo_init: es la función que se ejecuta al cargar el módulo
    - proc_create: es una función que se encarga de crear un archivo en /proc
    - PROC_NAME: es el nombre del archivo en /proc
    - 0: es el modo de acceso al archivo en /proc
    - NULL: es un puntero a void, que es un puntero a la estructura mm_struct
    - &sysinfo_ops: es un puntero a la estructura sysinfo_ops, que es la estructura
    que contiene las funciones para abrir y leer un archivo en /proc
    - printk: es una función que se encarga de imprimir en el log del kernel
    - KERN_INFO: es un nivel de log del kernel
    - "sysinfo module loaded": es el mensaje que se imprime en el log del kernel
    - return 0: indica que la función se ejecutó correctamente

*/

static void __exit sysinfo_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "sysinfo module unloaded\n");
}

/*
    Explicacion de la función sysinfo_exit:
    - __exit: es un macro que indica que la función se ejecuta al descargar el módulo
    - sysinfo_exit: es la función que se ejecuta al descargar el módulo
    - remove_proc_entry: es una función que se encarga de eliminar un archivo en /proc
    - PROC_NAME: es el nombre del archivo en /proc
    - NULL: es un puntero a void, que es un puntero a la estructura mm_struct
    - printk: es una función que se encarga de imprimir en el log del kernel
    - KERN_INFO: es un nivel de log del kernel
    - "sysinfo module unloaded": es el mensaje que se imprime en el log del kernel
*/


module_init(sysinfo_init);
module_exit(sysinfo_exit);

/*
    Explicacion de las funciones module_init y module_exit:
    - module_init: es una macro que indica que la función se ejecuta al cargar el módulo
    - sysinfo_init: es la función que se ejecuta al cargar el módulo
    - module_exit: es una macro que indica que la función se ejecuta al descargar el módulo
    - sysinfo_exit: es la función que se ejecuta al descargar el módulo
*/


/*
    Flujo de como se ejecuta cada función:
        - al cargar el módulo se ejecuta la función sysinfo_init
        - al ejecutar la función sysinfo_init se crea un archivo en /proc
        - al crear el archivo en /proc se ejecuta la función sysinfo_open
        - al ejecutar la función sysinfo_open se ejecuta la función sysinfo_show
        - al ejecutar la función sysinfo_show se escriben las metricas del 
          sistema en el archivo en /proc
        - al descargar el módulo se ejecuta la función sysinfo_exit
        - al ejecutar la función sysinfo_exit se elimina el archivo en /proc
*/


/*
    Comandos para compilar y ejecutar el módulo:
        - make
        - sudo insmod sysinfo.ko para cargar el módulo
        - cat /proc/sysinfo para ver las metricas del sistema
        - sudo rmmod sysinfo para descargar el módulo
*/


