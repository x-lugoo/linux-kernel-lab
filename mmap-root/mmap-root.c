#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/gfp.h>
#include <linux/string.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/io.h>


static struct cdev *pcdev;
static dev_t ndev;



static int dev_open(struct inode *inodep ,struct file *filep)
{
	printk(KERN_INFO "Jeff: device has been opened.\n");

	return 0;
}

static int simple_mmap (struct file *filp, struct vm_area_struct *vma)
{
	printk(KERN_INFO "Jeff: device mmap.\n");

	if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
				vma->vm_end - vma->vm_start, vma->vm_page_prot))
	{
		printk(KERN_INFO "Jeff: device mmap failed.\n");
		return -EAGAIN;
	}
	printk(KERN_INFO "Jeff: device mmap ok. offset:%lu \n", vma->vm_pgoff);
	return 0;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "Jeff: device release.\n");
	kfree(filep->private_data);
	return 0;
}

static struct file_operations mmap_fops =
{
	.open = dev_open,
	.mmap = simple_mmap,
	.release = dev_release,
};


static int __init cve_mmap_init(void)
{
    int err = 0;

    pcdev = cdev_alloc();
    if(!pcdev){
        err = -ENOMEM;
        goto cdev_err;
    }
    cdev_init(pcdev, &mmap_fops);
    err = alloc_chrdev_region(&ndev, 0, 1, "mmap_dev");
    if(err)
        goto region_err;
    printk("major = %d, minor = %d\n", MAJOR(ndev), MINOR(ndev));
    pcdev->owner = THIS_MODULE;
    err = cdev_add(pcdev, ndev, 1);
    if(err)
        goto add_err;

    return 0;

add_err:
    unregister_chrdev_region(ndev, 1);
region_err:
    cdev_del(pcdev);
cdev_err:
	return err;
}

static void __exit cve_mmap_exit(void)
{
    cdev_del(pcdev);
    unregister_chrdev_region(ndev, 1);
}


MODULE_AUTHOR("Jeff Xie");
MODULE_LICENSE("GPL");
module_init(cve_mmap_init);
module_exit(cve_mmap_exit);

