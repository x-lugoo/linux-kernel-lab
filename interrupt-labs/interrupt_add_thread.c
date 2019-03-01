
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/slab.h>

#define SHARE_IRQ 19
int irq = SHARE_IRQ;
module_param(irq, int, S_IRUGO);

static int delay = 0;
module_param(delay, int, S_IRUGO);

static atomic_t counter_bh, counter_th;

struct my_dat {
	unsigned long jiffies;
	struct tasklet_struct tsk;
	struct work_struct work;
};

static struct my_dat my_data;
static DECLARE_WAIT_QUEUE_HEAD(wq);
static atomic_t cond;
static struct task_struct *tsk;

static irqreturn_t my_interrupt(int irq, void *dev_id)
{
	struct my_dat *data = (struct my_dat *)dev_id;
	atomic_inc(&counter_th);
	data->jiffies = jiffies;
	atomic_set(&cond, 1);
	mdelay(delay);
	wake_up_interruptible(&wq);
	return IRQ_NONE;
}

static int thr_fun(void *thr_arg)
{
	struct my_dat *data = (struct my_dat *)thr_arg;

	do {
		atomic_set(&cond, 0);
		wait_event_interruptible(wq, kthread_should_stop()
				|| atomic_read(&cond));

		if (atomic_read(&cond))
			atomic_inc(&counter_bh);
		printk(KERN_INFO "In BH: counter_th = %d, counter_bh = %d, jiffies = %ld, %ld\n",
				atomic_read(&counter_th), atomic_read(&counter_bh),
				data->jiffies, jiffies);
	} while (!kthread_should_stop());
	return 0;
}

static int __init my_init(void)
{
	atomic_set(&cond, 1);
	atomic_set(&counter_bh, 0);
	atomic_set(&counter_th, 0);
	
	if (request_irq(irq, my_interrupt, IRQF_SHARED, "my_init", &my_data))
		return -1;
	
	if (!(tsk = kthread_run(thr_fun, (void*)&my_data, "thr_fun"))) {
		printk(KERN_INFO "Failed to generate a kernel thread\n");
		return -1;
	}

	printk(KERN_INFO "successfully loaded\n");
	return 0;
}


static void __exit my_exit(void)
{
	kthread_stop(tsk);
	synchronize_irq(irq);
	free_irq(irq, &my_data);
	printk(KERN_INFO " counter_th = %d, counter_bh = %d\n",
			atomic_read(&counter_th), atomic_read(&counter_bh));
	printk(KERN_INFO "successfully exited\n");
}


module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("JEFF");
MODULE_LICENSE("GPL");
