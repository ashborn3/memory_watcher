#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/pid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashborn3");
MODULE_DESCRIPTION("A module to monitor memory access of a specific process");

static int pid = 0;
module_param(pid, int, 0);
MODULE_PARM_DESC(pid, "The PID of the process to monitor");

static struct task_struct *task;

static int __init memory_watcher_init(void)
{
    printk(KERN_INFO "Memory watcher module loaded for PID %d\n", pid);

    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        printk(KERN_ERR "No such process with PID %d\n", pid);
        return -ESRCH;
    }

    return 0;
}

static void __exit memory_watcher_exit(void)
{
    printk(KERN_INFO "Memory watcher module unloaded\n");
}

module_init(memory_watcher_init);
module_exit(memory_watcher_exit);

