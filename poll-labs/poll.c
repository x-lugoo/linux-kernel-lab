#include <linux/module.h>
#include <linux/fs.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>

#define MYDEV_NAME "mycdev"
static struct devive *my_device;

struct polldev {
	wait_queue_head_t wq;
	unsigned long state;
};


static int kern_open(struct inode *inode, struct  file *filp)
{
	struct polldev *pd;

	pr_info("start");
	pd = kmalloc(sizeof(struct polldev), GFP_KERNEL);
	if (IS_ERR(pd))
		return PTR_ERR(pd);
	init_waitqueue_head(&pd->wq);
	pd->state = 0;
	filp->private_data = pd;
}

static int kern_release(struct inode *inode, struct file *filp)
{
	struct polldev *pd;

	pr_info("release\n");
	pd = (struct polldev *)(filp->private_data);
	kfree(pd);
	return 0;
}

static unsigned int kern_poll(struct file *fp, poll_table *wait)
{
	struct polldev *pd;
	unsigned int mask;

	pd = (struct polldev *)(fp->private_data);

	pr_info("start %p\n", pd);
	poll_wait(fp, &pd->wq, wait);
	pr_info("return with %u", mask);
	return mask;
}

static long kern_unlocked_ioctl(struct file *fp, unsigned int cmd,
                unsigned long arg) {
        struct polldev *pd;
        pd = (struct polldev *)fp->private_data;
        pr_info("start %p", pd);
        switch (cmd) {
        case 1:
                pr_info("in WAKE");
                pd->state = POLLIN;
                wmb();
                wake_up_all(&pd->wq);
                return 0;
        case 2:
                pr_info("in RESET");
                pd->state = 0;
                wmb();
                wake_up_all(&pd->wq);
                return 0;
        }
        return -ENOTTY;
}

static const struct file_operations my_fops = { 
        .owner = THIS_MODULE,
        .open = kern_open,
        .release = kern_release,
        .unlocked_ioctl = kern_unlocked_ioctl,
        .poll = kern_poll,
};

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = MYDEV_NAME,
	.fops  = &my_fops,
};

static int __init my_init(void)
{
	if (misc_register(&my_misc_device)) {
		printk(KERN_WARNING "coldn't register device misc,"
				"%d.\n", my_misc_device.minor);
		return -EBUSY;
	}

	printk(KERN_INFO "\n jeff Successed in registering character device"
			"%s MINOR %d\n", MYDEV_NAME, my_misc_device.minor);
	return 0;
}

static void __exit my_exit(void)
{
	misc_deregister(&my_misc_device);
	printk(KERN_INFO "\ndevice unregistered\n");
}


module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JEFF");

