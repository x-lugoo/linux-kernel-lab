
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

static struct my_dat my_data;
static atomic_t counter_bh, counter_th;
static atomic_t nevents;

struct my_dat {
	unsigned long jiffies;
	struct tasklet_struct tsk;
	struct work_struct work;
};


static irqreturn_t my_interrupt(int irq, void *dev_id)
{
	atomic_inc(&counter_th);
	atomic_inc(&nevents);
	mdelay(delay);
	return IRQ_WAKE_THREAD;
 	/* return IRQ_NONE; */
}

static irqreturn_t thread_run(int irq, void *thr_arg)
{
	do {
		atomic_inc(&counter_bh);

	} while (!atomic_dec_and_test(&nevents)); 
	printk(KERN_INFO "In Bh: counter_th= %d,counter_bh=%d, nevents=%d\n",
			atomic_read(&counter_th), atomic_read(&counter_bh),
			atomic_read(&nevents));

	return IRQ_NONE;
}

static int __init my_init(void)
{
	atomic_set(&nevents, 0);
	atomic_set(&counter_bh, 0);
	atomic_set(&counter_th, 0);
	
	if (request_threaded_irq(irq, my_interrupt, thread_run, IRQF_SHARED, 
				"my_init", &my_data))
		return -1;

	printk(KERN_INFO "successfully loaded\n");
	return 0;
}


static void __exit my_exit(void)
{
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
