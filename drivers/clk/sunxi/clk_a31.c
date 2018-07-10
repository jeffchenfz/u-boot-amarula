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
#include <dt-bindings/clock/sun6i-a31-ccu.h>

struct a31_clk_priv {
	void *base;
};

static int a31_clk_enable(struct clk *clk)
{
	struct a31_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_AHB1_OTG:
		setbits_le32(priv->base + 0x60, BIT(24));
		return 0;
	case CLK_AHB1_EHCI0:
	case CLK_AHB1_EHCI1:
		setbits_le32(priv->base + 0x60,
			     BIT(26 + (clk->id - CLK_AHB1_EHCI0)));
		return 0;
	case CLK_AHB1_OHCI0:
	case CLK_AHB1_OHCI1:
	case CLK_AHB1_OHCI2:
		setbits_le32(priv->base + 0x60,
			     BIT(29 + (clk->id - CLK_AHB1_OHCI0)));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
	case CLK_USB_PHY2:
		setbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	case CLK_USB_OHCI0:
	case CLK_USB_OHCI1:
	case CLK_USB_OHCI2:
		setbits_le32(priv->base + 0xcc,
			     BIT(16 + (clk->id - CLK_USB_OHCI0)));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static int a31_clk_disable(struct clk *clk)
{
	struct a31_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_AHB1_OTG:
		clrbits_le32(priv->base + 0x60, BIT(24));
		return 0;
	case CLK_AHB1_EHCI0:
	case CLK_AHB1_EHCI1:
		clrbits_le32(priv->base + 0x60,
			     BIT(26 + (clk->id - CLK_AHB1_EHCI0)));
		return 0;
	case CLK_AHB1_OHCI0:
	case CLK_AHB1_OHCI1:
	case CLK_AHB1_OHCI2:
		clrbits_le32(priv->base + 0x60,
			     BIT(29 + (clk->id - CLK_AHB1_OHCI0)));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
	case CLK_USB_PHY2:
		clrbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	case CLK_USB_OHCI0:
	case CLK_USB_OHCI1:
	case CLK_USB_OHCI2:
		clrbits_le32(priv->base + 0xcc,
			     BIT(16 + (clk->id - CLK_USB_OHCI0)));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static struct clk_ops a31_clk_ops = {
	.enable = a31_clk_enable,
	.disable = a31_clk_disable,
};

static int a31_clk_probe(struct udevice *dev)
{
	return 0;
}

static int a31_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct a31_clk_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr_ptr(dev);

	return 0;
}

static const struct udevice_id a31_clk_ids[] = {
	{ .compatible = "allwinner,sun6i-a31-ccu" },
	{ }
};

U_BOOT_DRIVER(clk_sun6i_a31) = {
	.name		= "sun6i_a31_ccu",
	.id		= UCLASS_CLK,
	.of_match	= a31_clk_ids,
	.priv_auto_alloc_size	= sizeof(struct a31_clk_priv),
	.ofdata_to_platdata	= a31_clk_ofdata_to_platdata,
	.ops		= &a31_clk_ops,
	.probe		= a31_clk_probe,
	.bind		= sunxi_clk_bind,
};
