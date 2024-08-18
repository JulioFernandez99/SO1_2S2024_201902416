#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JUFER");
MODULE_DESCRIPTION("Modulo para leer informacion de memoria, CPU y procesos de contenedores");
MODULE_VERSION("1.0");

#define PROC_NAME "moduloPY1"

static int sysinfo_show(struct seq_file *m, void *v) {
    struct sysinfo si;
    struct task_struct *task;
    unsigned long total_mem_kb, free_mem_kb, used_mem_kb;
    unsigned long vm_size, rss_size;
    long mem_usage;
    bool first_process = true;

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

    seq_printf(m, "    \"Processes\": [\n");
    for_each_process(task) {
        if (task->mm && (strstr(task->comm, "docker") || 
                 strstr(task->comm, "containerd") || 
                 strstr(task->comm, "runc") || 
                 strstr(task->comm, "dockerd") ||
                 strstr(task->comm, "containerd-shim"))) {

            vm_size = task->mm->total_vm * (PAGE_SIZE / 1024);
            rss_size = get_mm_rss(task->mm) * (PAGE_SIZE / 1024);
            mem_usage = (rss_size * 100) / total_mem_kb;

            if (!first_process) {
                seq_printf(m, ",\n");
            }
            first_process = false;

            seq_printf(m, "        {\n");
            seq_printf(m, "            \"PID\": %d,\n", task->pid);
            seq_printf(m, "            \"Name\": \"%s\",\n", task->comm);
            seq_printf(m, "            \"Vsz(KB)\": %lu,\n", vm_size);
            seq_printf(m, "            \"Rss(KB)\": %lu,\n", rss_size);
            seq_printf(m, "            \"MemoryUsagePercentage\": %ld\n", mem_usage);
            seq_printf(m, "        }");
        }
    }
    seq_printf(m, "\n    ]\n");
    seq_printf(m, "}\n");

    return 0;
}

static int sysinfo_open(struct inode *inode, struct file *file) {
    return single_open(file, sysinfo_show, NULL);
}

static const struct proc_ops sysinfo_ops = {
    .proc_open = sysinfo_open,
    .proc_read = seq_read,
};

static int __init sysinfo_init(void) {
    proc_create(PROC_NAME, 0, NULL, &sysinfo_ops);
    printk(KERN_INFO "sysinfo module loaded\n");
    return 0;
}

static void __exit sysinfo_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "sysinfo module unloaded\n");
}

module_init(sysinfo_init);
module_exit(sysinfo_exit);


/*

    Para compilar el codigo utilizar el siguiente comando: make
    Para instalar el modulo utilizar el siguiente comando: sudo insmod modulo.ko
    Para desinstalar el modulo utilizar el siguiente comando: sudo rmmod modulo

    Para ver la informacion del modulo utilizar el siguiente comando: cat /proc/moduloPY1

*/