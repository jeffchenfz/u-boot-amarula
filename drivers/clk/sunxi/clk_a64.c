// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * (C) Copyright 2018 Arm Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0
 */

#include <common.h>
#include <clk-uclass.h>
#include <dm.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <dt-bindings/clock/sun50i-a64-ccu.h>

struct a64_clk_priv {
	void *base;
};

static ulong a64_clk_get_rate(struct clk *clk)
{
	debug("%s(#%ld)\n", __func__, clk->id);

	debug("  unhandled\n");
	return -EINVAL;
}

static ulong a64_clk_set_rate(struct clk *clk, ulong rate)
{
	debug("%s(#%ld, rate: %lu)\n", __func__, clk->id, rate);

	debug("  unhandled\n");
	return -EINVAL;
}

static int a64_clk_enable(struct clk *clk)
{
	struct a64_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_BUS_OTG:
	case CLK_BUS_EHCI0:
	case CLK_BUS_EHCI1:
		setbits_le32(priv->base + 0x60,
			     BIT(23 + (clk->id - CLK_BUS_OTG)));
		return 0;
	case CLK_BUS_OHCI0:
	case CLK_BUS_OHCI1:
		setbits_le32(priv->base + 0x60,
			     BIT(28 + (clk->id - CLK_BUS_OHCI0)));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
		setbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	case CLK_USB_OHCI0:
		setbits_le32(priv->base + 0xcc, BIT(16));
		return 0;
	case CLK_USB_OHCI1:
		setbits_le32(priv->base + 0xcc, BIT(17));
		return 0;
	default:
		debug("  unhandled\n");
		return -ENODEV;
	}
}

static int a64_clk_disable(struct clk *clk)
{
	struct a64_clk_priv *priv = dev_get_priv(clk->dev);

	debug("%s(#%ld)\n", __func__, clk->id);

	switch (clk->id) {
	case CLK_BUS_OTG:
	case CLK_BUS_EHCI0:
	case CLK_BUS_EHCI1:
		clrbits_le32(priv->base + 0x60,
			     BIT(23 + (clk->id - CLK_BUS_OTG)));
		return 0;
	case CLK_BUS_OHCI0:
	case CLK_BUS_OHCI1:
		clrbits_le32(priv->base + 0x60,
			     BIT(28 + (clk->id - CLK_BUS_OHCI0)));
		return 0;
	case CLK_USB_PHY0:
	case CLK_USB_PHY1:
		clrbits_le32(priv->base + 0xcc,
			     BIT(8 + (clk->id - CLK_USB_PHY0)));
		return 0;
	case CLK_USB_OHCI0:
		clrbits_le32(priv->base + 0xcc, BIT(16));
		return 0;
	case CLK_USB_OHCI1:
		clrbits_le32(priv->base + 0xcc, BIT(17));
		return 0;
	default:
		debug("  unhandled\n");
		return -ENODEV;
	}
}

static struct clk_ops a64_clk_ops = {
	.get_rate = a64_clk_get_rate,
	.set_rate = a64_clk_set_rate,
	.enable = a64_clk_enable,
	.disable = a64_clk_disable,
};

static int a64_clk_probe(struct udevice *dev)
{
	return 0;
}

static int a64_clk_ofdata_to_platdata(struct udevice *dev)
{
	struct a64_clk_priv *priv = dev_get_priv(dev);

	priv->base = dev_read_addr_ptr(dev);

	return 0;
}

static const struct udevice_id a64_clk_ids[] = {
	{ .compatible = "allwinner,sun50i-a64-ccu" },
	{ }
};

U_BOOT_DRIVER(clk_sun50i_a64) = {
	.name		= "sun50i_a64_ccu",
	.id		= UCLASS_CLK,
	.of_match	= a64_clk_ids,
	.priv_auto_alloc_size	= sizeof(struct a64_clk_priv),
	.ofdata_to_platdata	= a64_clk_ofdata_to_platdata,
	.ops		= &a64_clk_ops,
	.probe		= a64_clk_probe,
};
