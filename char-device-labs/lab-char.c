
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>

#define MYDEV_NAME "mycdev"
static unsigned int count = 1;
static struct file_operations mycdev_fops;

static dev_t first;
static char *ramdisk;
static size_t ramdisk_size = (16 * PAGE_SIZE);
static struct cdev *my_cdev;
static struct class *foo_class;

static int __init my_generic_init(void)
{
	if (alloc_chrdev_region(&first, 0, count, MYDEV_NAME) < 0) {
		printk(KERN_ERR "failed to allocate character device region\n");
		return -1;
	}
	if (!(my_cdev = cdev_alloc())) {
		printk(KERN_ERR "cdev_alloc() fail\n");
		unregister_chrdev_region(first, count);
		return -1;
	}
	cdev_init(my_cdev, &mycdev_fops);

	ramdisk = kmalloc(ramdisk_size, GFP_KERNEL);
	if (!ramdisk)
		return -ENOMEM;

	if (cdev_add(my_cdev, first, count) < 0) {
		printk(KERN_ERR "cdev_add() failed\n");
		cdev_del(my_cdev);
		unregister_chrdev_region(first,count);
		kfree(ramdisk);
		return -1;
	}
	foo_class = class_create(THIS_MODULE, "my_class");
	device_create(foo_class, NULL, first, NULL, "%s", "mycdev");
	printk(KERN_INFO "\nSuccessed in registering character device %s\n",
			MYDEV_NAME);
	printk(KERN_INFO "Major number = %d, Minor numver = %d\n", MAJOR(first),
				MINOR(first));
	return 0;
}

static void __exit my_generic_exit(void)
{
	device_destroy(foo_class, first);
	class_destroy(foo_class);

	if (my_cdev)
		cdev_del(my_cdev);
	unregister_chrdev_region(first, count);
	kfree(ramdisk);
	printk(KERN_INFO "\ndevice unregistered\n");
}

module_init(my_generic_init);
module_exit(my_generic_exit);
MODULE_AUTHOR("jeff");
MODULE_LICENSE("GPL");

