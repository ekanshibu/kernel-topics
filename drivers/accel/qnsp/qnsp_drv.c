// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.

#include <linux/module.h>
#include <linux/kernel.h>

static int __init qnsp_core_init(void)
{

	pr_info("Initializing core driver (manual CB device enumeration enabled)\n");

	return 0;
}

static void __exit qnsp_core_exit(void)
{
	pr_info("Exiting core driver\n");
}

module_init(qnsp_core_init);
module_exit(qnsp_core_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Qualcomm AI Infra Team");
MODULE_DESCRIPTION("QNSP Edge AI Accelerator Driver");
