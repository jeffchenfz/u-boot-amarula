// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (C) 2018 Amarula Solutions B.V.
 * Author: Jagan Teki <jagan@amarulasolutions.com>
 */

#include <common.h>
#include <clk-uclass.h>
#include <dm.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <dt-bindings/clock/sun4i-a10-ccu.h>

struct a10_clk_priv {
	void *base;
};

static int a10_clk_enable(struct clk *clk)
{
	struct a10_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_AHB_OTG:
	case CLK_AHB_EHCI0:
	case CLK_AHB_OHCI0:
	case CLK_AHB_EHCI1:
	case CLK_AHB_OHCI1:
		setbits_le32(priv->base + 0x60, BIT(clk->id - CLK_AHB_OTG));
		return 0;
	case CLK_USB_OHCI0:
	case CLK_USB_OHCI1:
	case CLK_USB_PHY:
		setbits_le32(priv->base + 0xcc,
			     BIT(6 + (clk->id - CLK_USB_OHCI0)));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static int a10_clk_disable(struct clk *clk)
{
	struct a10_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_AHB_OTG:
	case CLK_AHB_EHCI0:
	case CLK_AHB_OHCI0:
	case CLK_AHB_EHCI1:
	case CLK_AHB_OHCI1:
		clrbits_le32(priv->base + 0x60, BIT(clk->id - CLK_AHB_OTG));
		return 0;
	case CLK_USB_OHCI0:
	case CLK_USB_OHCI1:
	case CLK_USB_PHY:
		clrbits_le32(priv->base + 0xcc,
			     BIT(6 + (clk->id - CLK_USB_OHCI0)));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static struct clk_ops a10_clk_ops = {
	.enable = a10_clk_enable,
	.disable = a10_clk_disable,
};

static int a10_clk_probe(struct udevice *dev)
{
	return 0;
}

static int a10_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct a10_clk_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr_ptr(dev);

	return 0;
}

static const struct udevice_id a10_clk_ids[] = {
	{ .compatible = "allwinner,sun4i-a10-ccu" },
	{ .compatible = "allwinner,sun7i-a20-ccu" },
	{ }
};

U_BOOT_DRIVER(clk_sun4i_a10) = {
	.name		= "sun4i_a10_ccu",
	.id		= UCLASS_CLK,
	.of_match	= a10_clk_ids,
	.priv_auto_alloc_size	= sizeof(struct a10_clk_priv),
	.ofdata_to_platdata	= a10_clk_ofdata_to_platdata,
	.ops		= &a10_clk_ops,
	.probe		= a10_clk_probe,
	.bind		= sunxi_clk_bind,
};
