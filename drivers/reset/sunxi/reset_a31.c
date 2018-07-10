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
#include <dt-bindings/reset/sun6i-a31-ccu.h>

struct a31_reset_priv {
	void *base;
};

static int a31_reset_request(struct reset_ctl *reset_ctl)
{
	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	/* check dt-bindings/reset/sun6i-a31-ccu.h for max id */
	if (reset_ctl->id > 56)
		return -EINVAL;

	return 0;
}

static int a31_reset_free(struct reset_ctl *reset_ctl)
{
	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	return 0;
}

static int a31_reset_assert(struct reset_ctl *reset_ctl)
{
	struct a31_reset_priv *priv = dev_get_priv(reset_ctl->dev);

	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	switch (reset_ctl->id) {
	case RST_USB_PHY0:
	case RST_USB_PHY1:
	case RST_USB_PHY2:
		clrbits_le32(priv->base + 0xcc, BIT(reset_ctl->id));
		return 0;
	case RST_AHB1_OTG:
		clrbits_le32(priv->base + 0x2c0, BIT(24));
		return 0;
	case RST_AHB1_EHCI0:
	case RST_AHB1_EHCI1:
		clrbits_le32(priv->base + 0x2c0,
			     BIT(26 + (reset_ctl->id - RST_AHB1_EHCI0)));
		return 0;
	case RST_AHB1_OHCI0:
	case RST_AHB1_OHCI1:
	case RST_AHB1_OHCI2:
		clrbits_le32(priv->base + 0x2c0,
			     BIT(29 + (reset_ctl->id - RST_AHB1_OHCI0)));
		return 0;
	default:
		debug("%s (RST#%ld) unhandled\n", __func__, reset_ctl->id);
		return -ENODEV;
	}
}

static int a31_reset_deassert(struct reset_ctl *reset_ctl)
{
	struct a31_reset_priv *priv = dev_get_priv(reset_ctl->dev);

	debug("%s(#%ld)\n", __func__, reset_ctl->id);

	switch (reset_ctl->id) {
	case RST_USB_PHY0:
	case RST_USB_PHY1:
	case RST_USB_PHY2:
		setbits_le32(priv->base + 0xcc, BIT(reset_ctl->id));
		return 0;
	case RST_AHB1_OTG:
		setbits_le32(priv->base + 0x2c0, BIT(24));
		return 0;
	case RST_AHB1_EHCI0:
	case RST_AHB1_EHCI1:
		setbits_le32(priv->base + 0x2c0,
			     BIT(26 + (reset_ctl->id - RST_AHB1_EHCI0)));
		return 0;
	case RST_AHB1_OHCI0:
	case RST_AHB1_OHCI1:
	case RST_AHB1_OHCI2:
		setbits_le32(priv->base + 0x2c0,
			     BIT(29 + (reset_ctl->id - RST_AHB1_OHCI0)));
		return 0;
	default:
		debug("%s (RST#%ld) unhandled\n", __func__, reset_ctl->id);
		return -ENODEV;
	}
}

struct reset_ops a31_reset_ops = {
	.request = a31_reset_request,
	.free = a31_reset_free,
	.rst_assert = a31_reset_assert,
	.rst_deassert = a31_reset_deassert,
};

static int a31_reset_probe(struct udevice *dev)
{
	struct a31_reset_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr_ptr(dev);

	return 0;
}

U_BOOT_DRIVER(reset_sun6i_a31) = {
	.name		= "sun6i_a31_reset",
	.id		= UCLASS_RESET,
	.ops		= &a31_reset_ops,
	.probe		= a31_reset_probe,
	.priv_auto_alloc_size = sizeof(struct a31_reset_priv),
};
