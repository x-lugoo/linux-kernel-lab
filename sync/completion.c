#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/completion.h>






#define PRINT_PERF "[SYNC_COMP]"
struct task_struct *read_thread, *write_thread;

unsigned int counter;
struct completion *comp;

static int write_function(void *data)
{
	while (counter !=1234)
		counter++;;
	complete(comp);

	do_exit(0);
}

static int read_function(void *data)
{
	wait_for_completion(comp);
	printk(PRINT_PERF "count:%d\n", counter);
	do_exit(0);
}

static int __init my_mod_init(void)
{
	printk(PRINT_PERF "Enteering module\n");
	counter = 0;
	comp = kmalloc(sizeof(struct completion), GFP_KERNEL);
	if (!comp)
		return -1;
	init_completion(comp);
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





