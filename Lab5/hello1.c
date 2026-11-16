// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksandr Yakubin");
MODULE_DESCRIPTION("Laboratory work 5: Module hello1 (list management & timing)");
MODULE_VERSION("0.1");

struct hello_event {
	struct list_head list;
	ktime_t time_before;
	ktime_t time_after;
};

static LIST_HEAD(hello_event_list);

int print_hello(void)
{
	struct hello_event *element;

	element = kmalloc(sizeof(*element), GFP_KERNEL);
	if (!element)
		return -ENOMEM;

	element->time_before = ktime_get();
	pr_info("Hello, world!\n");
	element->time_after = ktime_get();

	list_add_tail(&element->list, &hello_event_list);

	return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1 module loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_event *element, *tmp;
	s64 delta_ns;

	list_for_each_entry_safe(element, tmp, &hello_event_list, list) {
		delta_ns = ktime_to_ns(ktime_sub(element->time_after,
						 element->time_before));
		pr_info("Print duration: %lld ns\n", delta_ns);
		list_del(&element->list);
		kfree(element);
	}

	pr_info("hello1 module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
