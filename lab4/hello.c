// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Laboratory work 4: Kernel Module with parameters and linked list");
MODULE_VERSION("0.1");

static unsigned int print_count = 1;
module_param(print_count, uint, 0644);
MODULE_PARM_DESC(print_count,
		 "Number of times 'Hello, world!' string will be printed");

struct hello_event {
	struct list_head list;
	ktime_t event_time;
};

static LIST_HEAD(hello_event_list);

static int __init hello_init(void)
{
	unsigned int i;
	struct hello_event *element;

	if (print_count == 0 || (print_count >= 5 && print_count <= 10)) {
		pr_warn("Hello Module: Warning! print_count is %u\n",
			print_count);
	} else if (print_count > 10) {
		pr_err("Hello Module: Error! print_count (%u) > 10\n",
		       print_count);
		return -EINVAL;
	}

	for (i = 0; i < print_count; i++) {
		element = kmalloc(sizeof(*element), GFP_KERNEL);
		if (!element)
			return -ENOMEM;

		element->event_time = ktime_get();
		list_add_tail(&element->list, &hello_event_list);

		pr_info("Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_event *element, *tmp;

	list_for_each_entry_safe(element, tmp, &hello_event_list, list) {
		pr_info("Event time: %lld ns\n",
			ktime_to_ns(element->event_time));
		list_del(&element->list);
		kfree(element);
	}

	pr_info("Hello Module unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);
