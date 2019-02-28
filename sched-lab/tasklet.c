
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/init.h>

typedef struct simp_t {
	int i;
	int j;
}simp;

static simp t_data;

static void t_fun(unsigned long t_arg) {
	simp *datum = (simp *)t_arg;

	printk(KERN_INFO "entering t_fun ,datum->i = %d, jiffies = %ld\n",
			datum->i, jiffies);
	printk(KERN_INFO "entering t_fun,datum->j = %d ,jiffies = %ld\n",
			datum->j, jiffies);
}
DECLARE_TASKLET(t_name, t_fun, &t_data);

static int __init my_init(void) {
	t_data.i = 100;
	t_data.j = 200;
	printk(KERN_INFO "schedling my tasklet ,jiffies = %ld\n", jiffies);
	tasklet_schedule(&t_name);
	return 0;
}

static void __exit my_exit(void)
{
	printk(KERN_INFO "exit");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("JEFF");
MODULE_LICENSE("GPL");
