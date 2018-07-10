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
#include <dt-bindings/clock/sun8i-h3-ccu.h>

struct h3_clk_priv {
	void *base;
};

static int h3_clk_enable(struct clk *clk)
{
	struct h3_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_BUS_OTG:
	case CLK_BUS_EHCI0:
	case CLK_BUS_EHCI1:
	case CLK_BUS_EHCI2:
	case CLK_BUS_EHCI3:
	case CLK_BUS_OHCI0:
	case CLK_BUS_OHCI1:
	case CLK_BUS_OHCI2:
	case CLK_BUS_OHCI3:
		setbits_le32(priv->base + 0x60,
			     BIT(23 + (clk->id - CLK_BUS_OTG)));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
	case CLK_USB_PHY2:
	case CLK_USB_PHY3:
		setbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	case CLK_USB_OHCI0:
	case CLK_USB_OHCI1:
	case CLK_USB_OHCI2:
	case CLK_USB_OHCI3:
		setbits_le32(priv->base + 0xcc,
			     BIT(16 + (clk->id - CLK_USB_OHCI0)));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static int h3_clk_disable(struct clk *clk)
{
	struct h3_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_BUS_OTG:
	case CLK_BUS_EHCI0:
	case CLK_BUS_EHCI1:
	case CLK_BUS_EHCI2:
	case CLK_BUS_EHCI3:
	case CLK_BUS_OHCI0:
	case CLK_BUS_OHCI1:
	case CLK_BUS_OHCI2:
	case CLK_BUS_OHCI3:
		clrbits_le32(priv->base + 0x60,
			     BIT(23 + (clk->id - CLK_BUS_OTG)));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
	case CLK_USB_PHY2:
	case CLK_USB_PHY3:
		clrbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	case CLK_USB_OHCI0:
	case CLK_USB_OHCI1:
	case CLK_USB_OHCI2:
	case CLK_USB_OHCI3:
		clrbits_le32(priv->base + 0xcc,
			     BIT(16 + (clk->id - CLK_USB_OHCI0)));
		return 0;
	default:
		debug("%s (CLK#%ld) unhandled\n", __func__, clk->id);
		return -ENODEV;
	}
}

static struct clk_ops h3_clk_ops = {
	.enable = h3_clk_enable,
	.disable = h3_clk_disable,
};

static int h3_clk_probe(struct udevice *dev)
{
	return 0;
}

static int h3_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct h3_clk_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr_ptr(dev);

	return 0;
}

static const struct udevice_id h3_clk_ids[] = {
	{ .compatible = "allwinner,sun8i-h3-ccu" },
	{ .compatible = "allwinner,sun50i-h5-ccu" },
	{ }
};

U_BOOT_DRIVER(clk_sun8i_h3) = {
	.name		= "sun8i_h3_ccu",
	.id		= UCLASS_CLK,
	.of_match	= h3_clk_ids,
	.priv_auto_alloc_size	= sizeof(struct h3_clk_priv),
	.ofdata_to_platdata	= h3_clk_ofdata_to_platdata,
	.ops		= &h3_clk_ops,
	.probe		= h3_clk_probe,
	.bind		= sunxi_clk_bind,
};
