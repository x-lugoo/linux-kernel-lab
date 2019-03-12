#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/sched.h>

/* get symbol table megs from /proc/kallsyms */
struct vm_area_struct *(*p)(struct mm_struct *mm, unsigned long, struct vm_area_struct **) = 0xffffffffb3a161c0;

static int __init vma_init(void)
{
	struct mm_struct *cm;
	struct vm_area_struct *vma, *start_vma, *prev;
	struct task_struct *task;
	unsigned long start;

	task = get_current();
	if (!task) {
		printk(KERN_ALERT "NULL task\n");
		return -1;
	}
	printk(KERN_ALERT "task:%p", task);
	printk(KERN_ALERT "%s\n",task->comm);
	cm = task->mm;
	if (!cm) {
		printk(KERN_ALERT "NULL cm\n");
		return -1;
	}
	printk(KERN_ALERT "cm:%p", cm);
	start_vma = vma = cm->mmap;
	if (!vma) {
		printk(KERN_ALERT "NULL vma\n");
		return -1;
	}
	printk(KERN_ALERT "vma:%p", vma);
	while (vma) {
		printk(KERN_ALERT "start:0x%08lx, end:0x%08lx\n",
				vma->vm_start, vma->vm_end);
		vma = vma->vm_next;
	}

	printk(KERN_ALERT "find_vma-----------------\n");
	start = start_vma->vm_start - 0x4000;	
	vma = find_vma(cm, start - 0x4000);
	if (vma) {
		printk(KERN_ALERT"we find vma start from 0x%08lx\n", start);
		printk(KERN_ALERT"we  get vma start from 0x%08lx to 0x%08lx\n", 
				vma->vm_start, vma->vm_end);
	}

	printk(KERN_ALERT "find_vma_prev-----------------\n");
	start = (start_vma->vm_next )->vm_start - 0x4000;	
//	vma = find_vma_prev(cm, start - 0x4000, &prev);
	vma = p(cm, start - 0x4000, &prev);
	if (vma) {
		printk(KERN_ALERT"we find vma start from 0x%08lx\n", start);
		printk(KERN_ALERT"we  get vma start from 0x%08lx to 0x%08lx\n", 
				vma->vm_start, vma->vm_end);
		if (prev)
			printk(KERN_ALERT"prev vma start from 0x%08lx"\
				"to 0x%08lx\n", prev->vm_start, prev->vm_end);
		else
			printk(KERN_ALERT"Not find prev vma\n");
	}

	printk(KERN_ALERT "init ok\n");
	return 0;
}

static void __exit vma_exit(void)
{
	printk(KERN_ALERT "Goodbye kernel\n");
}

module_init(vma_init);
module_exit(vma_exit);
MODULE_LICENSE("GPL");
MODULE_LICENSE("JEFF");
