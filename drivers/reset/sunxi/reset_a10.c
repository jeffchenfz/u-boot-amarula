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
#include <dt-bindings/reset/sun4i-a10-ccu.h>

struct a10_reset_priv {
	void *base;
};

static int a10_reset_request(struct reset_ctl *reset_ctl)
{
	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	/* check dt-bindings/reset/sun4i-a10-ccu.h for max id */
	if (reset_ctl->id > 22)
		return -EINVAL;

	return 0;
}

static int a10_reset_free(struct reset_ctl *reset_ctl)
{
	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	return 0;
}

static int a10_reset_assert(struct reset_ctl *reset_ctl)
{
	struct a10_reset_priv *priv = dev_get_priv(reset_ctl->dev);

	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	switch (reset_ctl->id) {
	case RST_USB_PHY0:
	case RST_USB_PHY1:
	case RST_USB_PHY2:
		clrbits_le32(priv->base + 0x0cc, BIT(reset_ctl->id));
		return 0;
	default:
		debug("%s (RST#%ld) unhandled\n", __func__, reset_ctl->id);
		return -ENODEV;
	}
}

static int a10_reset_deassert(struct reset_ctl *reset_ctl)
{
	struct a10_reset_priv *priv = dev_get_priv(reset_ctl->dev);

	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	switch (reset_ctl->id) {
	case RST_USB_PHY0:
	case RST_USB_PHY1:
	case RST_USB_PHY2:
		setbits_le32(priv->base + 0x0cc, BIT(reset_ctl->id));
		return 0;
	default:
		debug("%s (RST#%ld) unhandled\n", __func__, reset_ctl->id);
		return -ENODEV;
	}
}

struct reset_ops a10_reset_ops = {
	.request = a10_reset_request,
	.free = a10_reset_free,
	.rst_assert = a10_reset_assert,
	.rst_deassert = a10_reset_deassert,
};

static int a10_reset_probe(struct udevice *dev)
{
	struct a10_reset_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr_ptr(dev);

	return 0;
}

U_BOOT_DRIVER(reset_sun4i_a10) = {
	.name		= "sun4i_a10_reset",
	.id		= UCLASS_RESET,
	.ops		= &a10_reset_ops,
	.probe		= a10_reset_probe,
	.priv_auto_alloc_size = sizeof(struct a10_reset_priv),
};
