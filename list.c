#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/slab.h>


static LIST_HEAD(my_list);

struct my_entry {
        struct list_head clist;
        int val;
};

static int __init my_init(void)
{
        struct my_entry *curr;
        struct list_head *ptr;
        struct my_entry *m1;
        struct my_entry *m2;

        m1 = kmalloc(sizeof(struct my_entry), GFP_KERNEL);
        if (!m1)
                return -ENOMEM;

        m2 = kmalloc(sizeof(struct my_entry), GFP_KERNEL);
        if (!m2)
                return -ENOMEM;

        INIT_LIST_HEAD(&m1->clist);
        INIT_LIST_HEAD(&m2->clist);
        m1->val = 20; 
        m2->val = 27; 
        list_add(&m1->clist, &my_list);
        list_add(&m2->clist, &my_list);
        list_for_each(ptr, &my_list) {
                curr = list_entry(ptr, struct my_entry, clist);
                printk(KERN_INFO "val = %d\n", curr->val);
        }   
        return 0;
}

static void __exit my_exit(void)
{
        struct list_head *pos;
        struct list_head *tmp;
        struct my_entry *curr;

        list_for_each_safe(pos, tmp, &my_list) {
                curr = list_entry(pos, struct my_entry, clist);
                list_del(&curr->clist);
                printk(KERN_INFO "(exit):val %d removed\n", curr->val);
                kfree(curr);
        }
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("jeff");
MODULE_LICENSE("GPL");


