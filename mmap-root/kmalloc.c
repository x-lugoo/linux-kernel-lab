#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>

static char *p;

static __init int init_kmalloc(void)
{

	p = kmalloc(20, GFP_KERNEL);
	if (!p)
		return -1;
	strcpy(p, "jeffnice+20");
	printk("p %s\n",p);
	printk(KERN_INFO "phy:0x%llx\n", virt_to_phys(p));
	return 0;
}

static void __exit exit_kmalloc(void)
{
	printk(KERN_INFO "p string:%s\n", p);
	kfree(p);
}
module_init(init_kmalloc);
module_exit(exit_kmalloc);

MODULE_AUTHOR("Jeff Xie");
MODULE_LICENSE("GPL");
