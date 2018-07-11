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
#include <dt-bindings/reset/sun8i-h3-ccu.h>

struct h3_reset_priv {
	void *base;
};

static int h3_reset_request(struct reset_ctl *reset_ctl)
{
	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	/* check dt-bindings/reset/sun8i-h3-ccu.h for max id */
	if (reset_ctl->id > 54)
		return -EINVAL;

	return 0;
}

static int h3_reset_free(struct reset_ctl *reset_ctl)
{
	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	return 0;
}

static int h3_reset_assert(struct reset_ctl *reset_ctl)
{
	struct h3_reset_priv *priv = dev_get_priv(reset_ctl->dev);

	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	switch (reset_ctl->id) {
	case RST_USB_PHY0:
	case RST_USB_PHY1:
	case RST_USB_PHY2:
	case RST_USB_PHY3:
		clrbits_le32(priv->base + 0x0cc, BIT(reset_ctl->id));
		return 0;
	case RST_BUS_OTG:
	case RST_BUS_EHCI0:
	case RST_BUS_EHCI1:
	case RST_BUS_EHCI2:
	case RST_BUS_EHCI3:
	case RST_BUS_OHCI0:
	case RST_BUS_OHCI1:
	case RST_BUS_OHCI2:
	case RST_BUS_OHCI3:
		clrbits_le32(priv->base + 0x2c0,
			     BIT(23 + (reset_ctl->id - RST_BUS_OTG)));
		return 0;
	default:
		debug("%s (RST#%ld) unhandled\n", __func__, reset_ctl->id);
		return -ENODEV;
	}
}

static int h3_reset_deassert(struct reset_ctl *reset_ctl)
{
	struct h3_reset_priv *priv = dev_get_priv(reset_ctl->dev);

	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	switch (reset_ctl->id) {
	case RST_USB_PHY0:
	case RST_USB_PHY1:
	case RST_USB_PHY2:
	case RST_USB_PHY3:
		setbits_le32(priv->base + 0x0cc, BIT(reset_ctl->id));
		return 0;
	case RST_BUS_OTG:
	case RST_BUS_EHCI0:
	case RST_BUS_EHCI1:
	case RST_BUS_EHCI2:
	case RST_BUS_EHCI3:
	case RST_BUS_OHCI0:
	case RST_BUS_OHCI1:
	case RST_BUS_OHCI2:
	case RST_BUS_OHCI3:
		setbits_le32(priv->base + 0x2c0,
			     BIT(23 + (reset_ctl->id - RST_BUS_OTG)));
		return 0;
	default:
		debug("%s (RST#%ld) unhandled\n", __func__, reset_ctl->id);
		return -ENODEV;
	}
}

struct reset_ops h3_reset_ops = {
	.request = h3_reset_request,
	.free = h3_reset_free,
	.rst_assert = h3_reset_assert,
	.rst_deassert = h3_reset_deassert,
};

static int h3_reset_probe(struct udevice *dev)
{
	struct h3_reset_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr_ptr(dev);

	return 0;
}

U_BOOT_DRIVER(reset_sun8i_h3) = {
	.name		= "sun8i_h3_reset",
	.id		= UCLASS_RESET,
	.ops		= &h3_reset_ops,
	.probe		= h3_reset_probe,
	.priv_auto_alloc_size = sizeof(struct h3_reset_priv),
};
