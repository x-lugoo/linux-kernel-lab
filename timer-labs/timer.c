#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/timer.h>


static struct timer_list my_timer;
static int status = 1; 

static struct my_dat {
	int num;
	struct timer_list tlist;
} my_data;


static void thr_fun(struct timer_list *timer)
{

	struct my_dat *data = container_of(timer, struct my_dat, tlist);
	while (status) {
		printk(KERN_INFO "%d\n", data->num);
		msleep(1);
	};
}


static int __init my_init(void)
{

	my_data.num = 99000;

	//init_timer(&my_data.tlist);
	my_timer.function = thr_fun;
	my_timer.expires = jiffies + 10000;
	add_timer(&my_timer);
	printk(KERN_INFO "add timer\n");
	return 0;
}

static void __exit my_exit(void)
{
	status = 0;
	del_timer(&my_timer);
	printk(KERN_INFO "del timer\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("JEFF");
MODULE_LICENSE("GPL");
