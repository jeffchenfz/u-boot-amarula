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
#include <dt-bindings/clock/sun5i-ccu.h>

struct a10s_clk_priv {
	void *base;
};

static int a10s_clk_enable(struct clk *clk)
{
	struct a10s_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_AHB_OTG:
	case CLK_AHB_EHCI:
	case CLK_AHB_OHCI:
		setbits_le32(priv->base + 0x60, BIT(clk->id - CLK_AHB_OTG));
		return 0;
	case CLK_USB_OHCI:
		setbits_le32(priv->base + 0xcc, BIT(6));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
		setbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static int a10s_clk_disable(struct clk *clk)
{
	struct a10s_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_AHB_OTG:
	case CLK_AHB_EHCI:
	case CLK_AHB_OHCI:
		clrbits_le32(priv->base + 0x60, BIT(clk->id - CLK_AHB_OTG));
		return 0;
	case CLK_USB_OHCI:
		clrbits_le32(priv->base + 0xcc, BIT(6));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
		clrbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static struct clk_ops a10s_clk_ops = {
	.enable = a10s_clk_enable,
	.disable = a10s_clk_disable,
};

static int a10s_clk_probe(struct udevice *dev)
{
	return 0;
}

static int a10s_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct a10s_clk_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr_ptr(dev);

	return 0;
}

static const struct udevice_id a10s_clk_ids[] = {
	{ .compatible = "allwinner,sun5i-a10s-ccu" },
	{ .compatible = "allwinner,sun5i-a13-ccu" },
	{ }
};

U_BOOT_DRIVER(clk_sun5i_a10s) = {
	.name		= "sun5i_a10s_ccu",
	.id		= UCLASS_CLK,
	.of_match	= a10s_clk_ids,
	.priv_auto_alloc_size	= sizeof(struct a10s_clk_priv),
	.ofdata_to_platdata	= a10s_clk_ofdata_to_platdata,
	.ops		= &a10s_clk_ops,
	.probe		= a10s_clk_probe,
	.bind		= sunxi_clk_bind,
};
