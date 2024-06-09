#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A module to monitor memory access of a specific process");

static int __init memory_watcher_init(void)
{
    printk(KERN_INFO "Memory watcher module loaded\n");
    return 0;
}

static void __exit memory_watcher_exit(void)
{
    printk(KERN_INFO "Memory watcher module unloaded\n");
}

module_init(memory_watcher_init);
module_exit(memory_watcher_exit);

