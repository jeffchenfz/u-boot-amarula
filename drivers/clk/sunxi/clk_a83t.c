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
#include <dt-bindings/clock/sun8i-a83t-ccu.h>

struct a83t_clk_priv {
	void *base;
};

static int a83t_clk_enable(struct clk *clk)
{
	struct a83t_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_BUS_OTG:
		setbits_le32(priv->base + 0x60, BIT(24));
		return 0;
	case CLK_BUS_EHCI0:
	case CLK_BUS_EHCI1:
		setbits_le32(priv->base + 0x60,
			     BIT(26 + (clk->id - CLK_BUS_EHCI0)));
		return 0;
	case CLK_BUS_OHCI0:
		setbits_le32(priv->base + 0x60, BIT(29));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
	case CLK_USB_HSIC:
	case CLK_USB_HSIC_12M:
		setbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	case CLK_USB_OHCI0:
		setbits_le32(priv->base + 0xcc, BIT(16));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static int a83t_clk_disable(struct clk *clk)
{
	struct a83t_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_BUS_OTG:
		clrbits_le32(priv->base + 0x60, BIT(24));
		return 0;
	case CLK_BUS_EHCI0:
	case CLK_BUS_EHCI1:
		clrbits_le32(priv->base + 0x60,
			     BIT(26 + (clk->id - CLK_BUS_EHCI0)));
		return 0;
	case CLK_BUS_OHCI0:
		clrbits_le32(priv->base + 0x60, BIT(29));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
	case CLK_USB_HSIC:
	case CLK_USB_HSIC_12M:
		clrbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	case CLK_USB_OHCI0:
		clrbits_le32(priv->base + 0xcc, BIT(16));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static struct clk_ops a83t_clk_ops = {
	.enable = a83t_clk_enable,
	.disable = a83t_clk_disable,
};

static int a83t_clk_probe(struct udevice *dev)
{
	return 0;
}

static int a83t_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct a83t_clk_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr_ptr(dev);

	return 0;
}

static const struct udevice_id a83t_clk_ids[] = {
	{ .compatible = "allwinner,sun8i-a83t-ccu" },
	{ }
};

U_BOOT_DRIVER(clk_sun8i_a83t) = {
	.name		= "sun8i_a83t_ccu",
	.id		= UCLASS_CLK,
	.of_match	= a83t_clk_ids,
	.priv_auto_alloc_size	= sizeof(struct a83t_clk_priv),
	.ofdata_to_platdata	= a83t_clk_ofdata_to_platdata,
	.ops		= &a83t_clk_ops,
	.probe		= a83t_clk_probe,
	.bind		= sunxi_clk_bind,
};
