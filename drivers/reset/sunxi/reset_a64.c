// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (C) 2018 Amarula Solutions B.V.
 * Author: Jagan Teki <jagan@amarulasolutions.com>
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <reset-uclass.h>
#include <asm/io.h>
#include <dm/lists.h>
#include <dt-bindings/reset/sun50i-a64-ccu.h>

struct a64_reset_priv {
	void *base;
};

static int a64_reset_request(struct reset_ctl *reset_ctl)
{
	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	return 0;
}

static int a64_reset_free(struct reset_ctl *reset_ctl)
{
	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	return 0;
}

static int a64_reset_assert(struct reset_ctl *reset_ctl)
{
	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	debug("  unhandled\n");
	return -EINVAL;
}

static int a64_reset_deassert(struct reset_ctl *reset_ctl)
{
	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	debug("  unhandled\n");
	return -EINVAL;
}

struct reset_ops a64_reset_ops = {
	.request = a64_reset_request,
	.free = a64_reset_free,
	.rst_assert = a64_reset_assert,
	.rst_deassert = a64_reset_deassert,
};

static int a64_reset_probe(struct udevice *dev)
{
	return 0;
}

U_BOOT_DRIVER(reset_sun50i_a64) = {
	.name		= "sun50i_a64_reset",
	.id		= UCLASS_RESET,
	.ops		= &a64_reset_ops,
	.probe		= a64_reset_probe,
	.priv_auto_alloc_size = sizeof(struct a64_reset_priv),
};
