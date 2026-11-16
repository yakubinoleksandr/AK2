// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleksandr Yakubin");
MODULE_DESCRIPTION("Laboratory work 5: Module hello2 (parameter handling & caller)");
MODULE_VERSION("0.1");

static unsigned int print_count = 1;
module_param(print_count, uint, 0644);
MODULE_PARM_DESC(print_count,
		 "Number of times print_hello() will be called");

static int __init hello2_init(void)
{
	unsigned int i;
	int ret;

	if (print_count == 0 || (print_count >= 5 && print_count <= 10)) {
		pr_warn("hello2: Warning! print_count is %u\n", print_count);
	} else if (print_count > 10) {
		pr_err("hello2: Error! print_count (%u) > 10\n", print_count);
		return -EINVAL;
	}

	for (i = 0; i < print_count; i++) {
		ret = print_hello();
		if (ret) {
			pr_err("hello2: print_hello() failed with %d\n", ret);
			return ret;
		}
	}

	pr_info("hello2 module loaded\n");
	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2 module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
