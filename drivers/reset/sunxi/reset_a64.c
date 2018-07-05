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

	/* check dt-bindings/reset/sun50i-a64-ccu.h for max id */
	if (reset_ctl->id > 50)
		return -EINVAL;

	return 0;
}

static int a64_reset_free(struct reset_ctl *reset_ctl)
{
	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	return 0;
}

static int a64_reset_assert(struct reset_ctl *reset_ctl)
{
	struct a64_reset_priv *priv = dev_get_priv(reset_ctl->dev);

	debug("%s(#%ld)\n", __func__, reset_ctl->id);
	switch(reset_ctl->id) {
	case RST_BUS_OTG:
	case RST_BUS_EHCI0:
	case RST_BUS_EHCI1:
		clrbits_le32(priv->base + 0x2c0,
			     BIT(23 + (reset_ctl->id - RST_BUS_OTG)));
		return 0;
	case RST_BUS_OHCI0:
	case RST_BUS_OHCI1:
		clrbits_le32(priv->base + 0x2c0,
			     BIT(28 + (reset_ctl->id - RST_BUS_OHCI0)));
		return 0;
	case RST_USB_PHY0:
	case RST_USB_PHY1:
		clrbits_le32(priv->base + 0x0cc,
			     BIT(reset_ctl->id - RST_USB_PHY0));
		return 0;
	default:
		debug("%s (RST#%ld) unhandled\n", __func__, reset_ctl->id);
		return -ENODEV;
	}
}

static int a64_reset_deassert(struct reset_ctl *reset_ctl)
{
	struct a64_reset_priv *priv = dev_get_priv(reset_ctl->dev);

	debug("%s(#%ld)\n", __func__, reset_ctl->id);
	switch(reset_ctl->id) {
	case RST_BUS_OTG:
	case RST_BUS_EHCI0:
	case RST_BUS_EHCI1:
		setbits_le32(priv->base + 0x2c0,
			     BIT(23 + (reset_ctl->id - RST_BUS_OTG)));
		return 0;
	case RST_BUS_OHCI0:
	case RST_BUS_OHCI1:
		setbits_le32(priv->base + 0x2c0,
			     BIT(28 + (reset_ctl->id - RST_BUS_OHCI0)));
		return 0;
	case RST_USB_PHY0:
	case RST_USB_PHY1:
		setbits_le32(priv->base + 0x0cc,
			     BIT(reset_ctl->id - RST_USB_PHY0));
		return 0;
	default:
		debug("%s (RST#%ld) unhandled\n", __func__, reset_ctl->id);
		return -ENODEV;
	}
}

struct reset_ops a64_reset_ops = {
	.request = a64_reset_request,
	.free = a64_reset_free,
	.rst_assert = a64_reset_assert,
	.rst_deassert = a64_reset_deassert,
};

static int a64_reset_probe(struct udevice *dev)
{
	struct a64_reset_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr_ptr(dev);

	return 0;
}

U_BOOT_DRIVER(reset_sun50i_a64) = {
	.name		= "sun50i_a64_reset",
	.id		= UCLASS_RESET,
	.ops		= &a64_reset_ops,
	.probe		= a64_reset_probe,
	.priv_auto_alloc_size = sizeof(struct a64_reset_priv),
};
