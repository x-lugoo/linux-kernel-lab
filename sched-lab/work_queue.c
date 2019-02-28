#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/workqueue.h>


static int status = 1; 
static struct my_dat {
	int num;
	struct work_struct work;
} my_data;

static void thr_fun(struct work_struct *w_arg)
{
	struct my_dat *data = container_of(w_arg, struct my_dat, work);

	while (status) {
		printk(KERN_INFO "%d\n", data->num);
		msleep(1);
	};
}


static int __init my_init(void)
{
	my_data.num = 90;

	INIT_WORK(&my_data.work, thr_fun);
	schedule_work(&my_data.work);
	printk(KERN_INFO "init schedule_work\n");
	return 0;
}

static void __exit my_exit(void)
{
	status = 0;
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("JEFF");
MODULE_LICENSE("GPL");
