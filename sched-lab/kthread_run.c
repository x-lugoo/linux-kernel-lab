
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct task_struct *tsk;

static int thr_fun(void *thr_arg)
{
	while (!kthread_should_stop()) {
		printk(KERN_INFO "running thr fun\n");
		msleep(1);
	};
	return 0;
}

static int __init my_init(void)
{

	if (!(tsk = kthread_run(thr_fun, (void*)NULL, "thr_fun"))) {
			printk(KERN_INFO "Failed to gene a kernel thread\n");
			return -1;
	}
	return 0;
}

static void __exit my_exit(void)
{
	kthread_stop(tsk);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("JEFF");
MODULE_LICENSE("GPL");
