
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0)
	#include <linux/sched/signal.h>
#else 
	#include <linux/signal.h>
#endif
#define MYDEV_NAME "mycdev"

static char *ramdisk;
static size_t ramdisk_size = (16 * PAGE_SIZE);
static atomic_t data_ready;
static DECLARE_WAIT_QUEUE_HEAD(wq);




static int mycdev_generic_open(struct inode *inode, struct file *file)
{
	static int counter = 0;
	printk(KERN_INFO "attempting to open device: %s \n", MYDEV_NAME);
	printk(KERN_INFO "MAJOR number = %d, MINOR number = %d\n",
			imajor(inode), iminor(inode));
	counter++;

	printk(KERN_INFO "successfully open device %s: \n\n", MYDEV_NAME);
	printk(KERN_INFO "ref=%d\n", module_refcount(THIS_MODULE));
	return 0;
}

static int mycdev_generic_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "closing character  device :%s\n \n", MYDEV_NAME);
	return 0;
}

static ssize_t mycdev_generic_read(struct file *file, char __user *buf, size_t lbuf,
		loff_t *ppos)
{
	int nbytes, maxbytes, bytes_to_do;

	printk(KERN_INFO "process %i (%s) going to sleep\n", current->pid,
	       current->comm);
	wait_event_interruptible(wq, atomic_read(&data_ready));
	if (signal_pending(current)) {
		printk(KERN_INFO "awaken by single, exit");
		return -ERESTARTSYS;
	}
	maxbytes = ramdisk_size - *ppos;
	printk(KERN_INFO "start to read\n");
	bytes_to_do = maxbytes > lbuf ? lbuf : maxbytes;
	if (bytes_to_do == 0)
		printk(KERN_WARNING "Readched end of the device on a read\n");
	nbytes = bytes_to_do - copy_to_user(buf, ramdisk + *ppos, bytes_to_do);
	*ppos += nbytes;
	printk(KERN_INFO "\n Leaving the write function bytes %d, pos=%d\n",
			nbytes, (int)*ppos);
	atomic_set(&data_ready, 0);
	return nbytes;
}

static ssize_t mycdev_generic_write(struct file *file, const char __user * buf, size_t lbuf,
		loff_t *ppos)
{
	int nbytes, maxbytes, bytes_to_do;

	printk(KERN_INFO "process %i (%s) awakening the readers...\n",
	       current->pid, current->comm);

	maxbytes = ramdisk_size - *ppos;
	bytes_to_do = maxbytes > lbuf ? lbuf : maxbytes;
	if (bytes_to_do == 0)
		printk(KERN_WARNING "Reached end of the device on a write\n");
	nbytes = bytes_to_do - copy_from_user(ramdisk + *ppos, buf, bytes_to_do);
	*ppos += nbytes;
	atomic_set(&data_ready, 1);
	wake_up_interruptible(&wq);
	printk(KERN_INFO "\n Leaving the write function bytes %d, pos=%d\n",
			nbytes, (int)*ppos);
	return nbytes;
}

static const struct file_operations mycdev_fops = {
	.owner = THIS_MODULE,
	.open  = mycdev_generic_open,
	.read  = mycdev_generic_read,
	.write = mycdev_generic_write,
	.release = mycdev_generic_release,
};
	

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = MYDEV_NAME,
	.fops  = &mycdev_fops,
};

static int __init my_generic_init(void)
{
	atomic_set(&data_ready, 0);
	ramdisk = kmalloc(ramdisk_size, GFP_KERNEL);
	if (!ramdisk)
		return -ENOMEM;

	if (misc_register(&my_misc_device)) {
		printk(KERN_WARNING "coldn't register device misc,"
				"%d.\n", my_misc_device.minor);
		return -EBUSY;
	}

	printk(KERN_INFO "\n jeff Successed in registering character device"
			"%s MINOR %d\n", MYDEV_NAME, my_misc_device.minor);
	return 0;
}

static void __exit my_generic_exit(void)
{
	misc_deregister(&my_misc_device);
	printk(KERN_INFO "\ndevice unregistered\n");
	kfree(ramdisk);
}

module_init(my_generic_init);
module_exit(my_generic_exit);

MODULE_AUTHOR("JEFF");
MODULE_LICENSE("GPL");


