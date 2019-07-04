#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/mutex.h>






#define PRINT_PERF "[SYNC_MUTEX]"
struct task_struct *read_thread, *write_thread;

unsigned int counter;
struct mutex *mut;

DEFINE_RWLOCK(count_lock);
static int write_function(void *data)
{
	while (!kthread_should_stop()) {
		mutex_lock(mut);
		counter++;;
		mutex_unlock(mut);
		msleep(50);
	}
	do_exit(0);
}

static int read_function(void *data)
{
	while (!kthread_should_stop()) {
		mutex_lock(mut);
		printk(PRINT_PERF "count:%d\n", counter);
		mutex_unlock(mut);
		msleep(50);
	}
	do_exit(0);
}

static int __init my_mod_init(void)
{
	printk(PRINT_PERF "Enteering module\n");
	counter = 0;
	mut = kmalloc(sizeof(struct mutex), GFP_KERNEL);
	if (!mut)
		return -1;
	mutex_init(mut);
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





