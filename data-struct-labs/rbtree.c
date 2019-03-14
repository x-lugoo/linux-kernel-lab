#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/rbtree.h>



struct product_t {
	struct rb_node tnode;
	char *name; 
	unsigned int price;
};


struct rb_root product_tree = RB_ROOT;

static int rbtree_search_product_by_name(const char *product_name,
		struct product_t **product_data_buf)
{
	struct rb_node *each_tnode;
	struct product_t *each_product;
	int ret;

	each_tnode = product_tree.rb_node;

	while (each_tnode != NULL) {
		each_product = rb_entry(each_tnode, struct product_t, tnode);

		ret = strcmp(product_name, each_product->name);
		if (ret < 0)
			each_tnode = each_tnode->rb_left;
		else if (ret > 0)
			each_tnode = each_tnode->rb_right;
		else {
			*product_data_buf = each_product;
			return 0;
		}

	}
		return -1;
}

static int rbtree_del(const char *name)
{
	struct product_t *tmp;
	if (rbtree_search_product_by_name(name, &tmp) < 0) {
		printk(KERN_INFO "can not  find the right product\n");
		return -1;
	}

	rb_erase(&tmp->tnode, &product_tree);
	return 0;
}

static int rbtree_clear(void)
{
	struct product_t *each_product, *tmp_product;

	rbtree_postorder_for_each_entry_safe(each_product, tmp_product, &product_tree, tnode) {
		rb_erase(&each_product->tnode, &product_tree);
		kfree(each_product);
	}
	return 0;
}


static int rb_tree_add(struct product_t *product_data)
{
	struct rb_node **each_tnode, *parent_tnode = NULL;
	struct product_t *each_product;
	int ret;

	if (!product_data) {
		printk(KERN_INFO "invaild param\n");
		return -EINVAL;
	}

	each_tnode = &(product_tree.rb_node);

	/* if *each_tnode equal empty, find the inset point */

	while (*each_tnode) {
		/* save parent node, need it when insert the new node */
		parent_tnode = *each_tnode;
		each_product = rb_entry(*each_tnode, struct product_t, tnode);
		ret = strcmp(product_data->name, each_product->name);
		if (ret < 0)
			each_tnode = &((*each_tnode)->rb_left);
		else if (ret > 0)
			each_tnode = &((*each_tnode)->rb_right);
		else 
			printk(KERN_INFO "product already exist, %s %u", each_product->name, 
				each_product->price);
	}


	/* insert into the rbtree */

	rb_link_node(&product_data->tnode, parent_tnode, each_tnode);
	rb_insert_color(&product_data->tnode, &product_tree);
	printk(KERN_INFO "insert the inode, %s %u", product_data->name, 
		product_data->price);
	return 0;
}

static struct product_t *p1;
static struct product_t *p2;

static int __init rbtree_init(void)
{
	int retval;
	struct product_t *tmp;

	p1 = kmalloc(sizeof(*p1), GFP_KERNEL);
	if (!p1) {
		printk(KERN_INFO "alloc memory faile\n");
		return -ENOMEM;
	}
	p1->name = "jeff";
	p1->price = 23;

	p2 = kmalloc(sizeof(*p2), GFP_KERNEL);
	if (!p2) {
		kfree(p1);
		printk(KERN_INFO "alloc memory faile\n");
		return -ENOMEM;
	}
	p2->name = "one";
	p2->price = 24;

	rb_tree_add(p1);
	rb_tree_add(p2);

	rbtree_del("one");
	retval = rbtree_search_product_by_name("one", &tmp);
	if (retval < 0) {
		printk(KERN_INFO "can not find the product\n");
		return 0; 
	}
	printk(KERN_INFO "find the product\n");
	return 0;
}

static void __exit rbtree_exit(void)
{
	rbtree_clear();
	printk(KERN_INFO "exit rbtree module\n");
}

module_init(rbtree_init);
module_exit(rbtree_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("JEFF");

