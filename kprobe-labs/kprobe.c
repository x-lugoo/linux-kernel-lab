#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

struct kprobe kp;

int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	printk(KERN_INFO "pid: %d |comm: %10s\n",
			current->pid, current->comm);
	return 0;
}

static __init int init_kprobe_sample(void)
{
	int retval;
	kprobe_opcode_t *addr;
	
	kp.symbol_name = "do_mmap";
	kp.pre_handler = handler_pre;
	addr = (kprobe_opcode_t *)kallsyms_lookup_name(kp.symbol_name);
	printk(KERN_INFO "addr = %p,op_code = 0x%lx \n", addr, (unsigned long)*addr);
	retval = register_kprobe(&kp);
	if (retval < 0)
		printk(KERN_INFO "retval = %d ---------kprobe for do_mmap error\n", retval);
	printk(KERN_INFO "addr = %p,op_code = 0x%lx \n", addr, (unsigned long)*addr);
	printk(KERN_INFO "------kprobe for do_mmap\n");
	return 0;
}

static void __exit cleanup_kprobe_sample(void)
{

	unregister_kprobe(&kp);
}
module_init(init_kprobe_sample);
module_exit(cleanup_kprobe_sample);

MODULE_AUTHOR("JEFF");
MODULE_LICENSE("GPL");
