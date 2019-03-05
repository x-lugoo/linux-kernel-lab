#include <linux/module.h>
#include <linux/init.h>
#include <linux/sysfs.h>
#include <linux/slab.h>

static struct kobject *parent;
static struct kobject *child;
static struct kset *c_kset;

static unsigned long flag = 1;

static ssize_t att_show(struct kobject *obj, struct attribute *attr, char *buf)
{
	size_t  count = 0;

	count +=sprintf(&buf[count], "%lu\n", flag);

	return count;
}

static ssize_t att_store(struct kobject *kobj, struct attribute *attr,
				const char *buf, size_t count)
{
	flag = buf[0] - '0';

	switch(flag) {
		printk(KERN_INFO "flag = %d\n", flag);
	case 0:
		kobject_uevent(kobj, KOBJ_ADD);
		printk(KERN_INFO "kobject add event\n");
		break;
	case 1:
		kobject_uevent(kobj, KOBJ_REMOVE);
		printk(KERN_INFO "kobject add event\n");
		break;
	case 2:
		kobject_uevent(kobj, KOBJ_CHANGE);
		printk(KERN_INFO "kobject change event\n");
		break;
	case 3:
		kobject_uevent(kobj, KOBJ_MOVE);
		printk(KERN_INFO "kobject move event\n");
		break;
	case 4:
		printk(KERN_INFO "kobject online event\n");
		kobject_uevent(kobj, KOBJ_ONLINE);
		break;
	case 5:
		printk(KERN_INFO "kobject offline event\n");
		kobject_uevent(kobj, KOBJ_OFFLINE);
		break;
	case 6:
		kobj->uevent_suppress = 1;
		printk(KERN_INFO "uevent_suppress = 1\n");
		break;
	case 7:
		kobj->uevent_suppress = 0;
		printk(KERN_INFO "uevent_suppress = 0\n");
		break;
	}
	return count;
}

	static struct attribute cld_att = {
		.name = "jeff",
		.mode = S_IRUGO | S_IWUSR,
	};
	static const struct sysfs_ops att_ops = {
		.show = att_show,
		.store = att_store,
	};

	static struct kobj_type cld_type = {
		.sysfs_ops = &att_ops,
	};

static int __init kobj_init(void)
{

	int err;

	parent = kobject_create_and_add("dir_obj", NULL);
	child = kzalloc(sizeof(*child), GFP_KERNEL);
	if (!child)
		return PTR_ERR(child);

	c_kset = kset_create_and_add("c_kset", NULL, parent);
	if (!c_kset)
		return -1;
	err = kobject_init_and_add(child, &cld_type, parent, "c_obj");
	if (err)
		return err;
	child->kset = c_kset;

	err = sysfs_create_file(child, &cld_att);
	return err;
}

static void __exit kobj_exit(void)
{
	sysfs_remove_file(child, &cld_att);
	kset_unregister(c_kset);
	kobject_del(child);
	kobject_del(parent);
}

module_init(kobj_init);
module_exit(kobj_exit)
MODULE_AUTHOR("JEFF");
MODULE_LICENSE("GPL");

