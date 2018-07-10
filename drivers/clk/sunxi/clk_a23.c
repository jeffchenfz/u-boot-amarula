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
#include <dt-bindings/clock/sun8i-a23-a33-ccu.h>

struct a23_clk_priv {
	void *base;
};

static int a23_clk_enable(struct clk *clk)
{
	struct a23_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_BUS_OTG:
		setbits_le32(priv->base + 0x60, BIT(24));
		return 0;
	case CLK_BUS_EHCI:
		setbits_le32(priv->base + 0x60, BIT(26));
		return 0;
	case CLK_BUS_OHCI:
		setbits_le32(priv->base + 0x60, BIT(29));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
		setbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	case CLK_USB_OHCI:
		setbits_le32(priv->base + 0xcc, BIT(16));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static int a23_clk_disable(struct clk *clk)
{
	struct a23_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_BUS_OTG:
		clrbits_le32(priv->base + 0x60, BIT(24));
		return 0;
	case CLK_BUS_EHCI:
		clrbits_le32(priv->base + 0x60, BIT(26));
		return 0;
	case CLK_BUS_OHCI:
		clrbits_le32(priv->base + 0x60, BIT(29));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
		clrbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	case CLK_USB_OHCI:
		clrbits_le32(priv->base + 0xcc, BIT(16));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static struct clk_ops a23_clk_ops = {
	.enable = a23_clk_enable,
	.disable = a23_clk_disable,
};

static int a23_clk_probe(struct udevice *dev)
{
	return 0;
}

static int a23_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct a23_clk_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr_ptr(dev);

	return 0;
}

static const struct udevice_id a23_clk_ids[] = {
	{ .compatible = "allwinner,sun8i-a23-ccu" },
	{ }
};

U_BOOT_DRIVER(clk_sun8i_a23) = {
	.name		= "sun8i_a23_ccu",
	.id		= UCLASS_CLK,
	.of_match	= a23_clk_ids,
	.priv_auto_alloc_size	= sizeof(struct a23_clk_priv),
	.ofdata_to_platdata	= a23_clk_ofdata_to_platdata,
	.ops		= &a23_clk_ops,
	.probe		= a23_clk_probe,
	.bind		= sunxi_clk_bind,
};
