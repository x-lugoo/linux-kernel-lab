#include<linux/init.h>
#include<linux/module.h>
#include<linux/slab.h>

static struct kmem_cache *jcache;
static void *buf;

static int __init jslab_init(void)
{
    printk(KERN_INFO "start %s\n",__func__);
    jcache = kmem_cache_create("jcache_object",
                20, 
                0,  
                SLAB_RECLAIM_ACCOUNT | SLAB_MEM_SPREAD,
                NULL);
    if (!jcache) {
        kmem_cache_destroy(jcache);
        printk(KERN_DEBUG "alloc cache fail\n");
        return -ENOMEM;
    }   
    buf= kmem_cache_alloc(jcache,GFP_KERNEL);
    return 0;
}

static void __exit jslab_exit(void)
{
    kmem_cache_free(jcache,buf);
    kmem_cache_destroy(jcache);
    printk(KERN_INFO "%s\n",__func__);
}

module_init(jslab_init);
module_exit(jslab_exit);
MODULE_LICENSE("GPL");

