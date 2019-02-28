#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
	int j = 0;
	struct list_head *modules;
	struct list_head *ptr;
	struct module *curr;
	
	struct module *m = THIS_MODULE;
	modules = &m->list;

	printk(KERN_INFO "%3d MOD:%20s, taints = %lu\n", j++, m->name,
			m->taints);
/*
 *	list_for_each(ptr, modules) {
 * 		curr = list_entry(ptr, struct module, list);
 *		printk(KERN_INFO "%3d MOD:%20s, taints = %lu\n", j++, curr->name,
 *			curr->taints);
 * 	}
 */

	list_for_each_entry(curr, modules, list) 
		printk(KERN_INFO "%3d MOD:%20s, taints = %lu\n", j++, curr->name,
			curr->taints);

	
	return 0;
}



static void __exit my_exit(void)
{
	printk(KERN_INFO "BYe: module unloaded from %0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("JEFF");
MODULE_LICENSE("GPL");

