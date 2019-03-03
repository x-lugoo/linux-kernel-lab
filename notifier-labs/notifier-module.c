#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

static struct notifier_block *pnb = NULL;

static char *master[] = {"LIVE", "COMING", "GOING"};

int get_notify(struct notifier_block *p, unsigned long v, void *m)
{
	printk(KERN_INFO "module<%s> is %s, p->priority=%d\n", ((struct module*)m)->name, master[v],
			p->priority);
	return 0;
}

static int hello_init(void)
{
	pnb = kzalloc(sizeof(struct notifier_block), GFP_KERNEL);
	if (!pnb)
		return -ENOMEM;

	pnb->notifier_call = get_notify;
	pnb->priority = 12;
	register_module_notifier(pnb);
	printk(KERN_INFO "A listening module is coming...\n");
	return 0;
}

static void hello_exit(void)
{
	unregister_module_notifier(pnb);
	kfree(pnb);
	printk(KERN_INFO "A listening module is going...\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("JEFF");
