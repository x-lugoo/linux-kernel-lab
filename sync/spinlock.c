#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/types.h>






#define PRINT_PERF "[SYNC_SPINLOCK]"
struct task_struct *read_thread, *write_thread;

unsigned int counter;

DEFINE_SPINLOCK(count_lock);
static int write_function(void *data)
{
	while (!kthread_should_stop()) {
		spin_lock(&count_lock);
		counter++;;
		spin_unlock(&count_lock);
		msleep(50);
	}
	do_exit(0);
}

static int read_function(void *data)
{
	while (!kthread_should_stop()) {
		spin_lock(&count_lock);
		printk(PRINT_PERF "count:%d\n", counter);
		spin_unlock(&count_lock);
		msleep(50);
	}
	do_exit(0);
}

static int __init my_mod_init(void)
{
	printk(PRINT_PERF "Enteering module\n");
	counter = 0;
	read_thread = kthread_run(read_function, NULL, "read_thread");
	write_thread = kthread_run(write_function, NULL, "write_thread");
	return 0;
}

static void __exit my_mod_exit(void)
{
	kthread_stop(read_thread);
	kthread_stop(write_thread);
	printk(KERN_INFO, "Exiting module\n");
}

module_init(my_mod_init);
module_exit(my_mod_exit);
MODULE_LICENSE("GPL");





