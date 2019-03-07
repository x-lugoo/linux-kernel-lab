#include <linux/module.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>





#define MS_TO_NS(x) (x*1E6L)

static struct hrtimer hr_timer;

static enum hrtimer_restart my_hrtimer_callback(struct hrtimer *timer)
{
        pr_info("my_hrtimer_callback called (%ld).\n", jiffies);
        return HRTIMER_NORESTART;
}

static int __init my_init(void)
{
        ktime_t ktime;
        unsigned long delay_in_ms = 200L;
        pr_info("HR Timer module installing\n");
        ktime = ktime_set(0, MS_TO_NS(delay_in_ms));
        hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        hr_timer.function = &my_hrtimer_callback;
        pr_info("Starting timer to fire in %ldms (%ld)\n",
                        delay_in_ms, jiffies);
        hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);
        return 0;
}

static void __exit my_exit(void)
{
        int ret;
        ret = hrtimer_cancel(&hr_timer);
        if (ret)
                pr_info("The timer was still in use...\n");
        pr_info("HR Timer module uninstalling\n");
}


module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("JEFF");
MODULE_LICENSE("GPL");
