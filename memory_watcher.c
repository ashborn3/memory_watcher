#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/kprobes.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ashborn3");
MODULE_DESCRIPTION("A module to monitor memory access of a specific process");

static int pid = 0;
module_param(pid, int, 0);
MODULE_PARM_DESC(pid, "The PID of the process to monitor");

static struct task_struct *task;

static int pre_handle_mm_fault(struct kprobe *p, struct pt_regs *regs)
{
    struct task_struct *cur = current;
    unsigned long addr = regs->si; // si register holds the fault address
    if (task && task != cur) {
        printk(KERN_INFO "Process %d accessed memory address %lx of process %d\n",
               cur->pid, addr, pid);
    }
    return 0;
}

static struct kprobe kp = {
    .symbol_name = "handle_mm_fault",
    .pre_handler = pre_handle_mm_fault,
};

static int __init memory_watcher_init(void)
{
    int ret;

    printk(KERN_INFO "Memory watcher module loaded for PID %d\n", pid);

    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        printk(KERN_ERR "No such process with PID %d\n", pid);
        return -ESRCH;
    }

    ret = register_kprobe(&kp);
    if (ret < 0) {
        printk(KERN_ERR "register_kprobe failed, returned %d\n", ret);
        return ret;
    }

    return 0;
}

static void __exit memory_watcher_exit(void)
{
    unregister_kprobe(&kp);
    printk(KERN_INFO "Memory watcher module unloaded\n");
}

module_init(memory_watcher_init);
module_exit(memory_watcher_exit);

