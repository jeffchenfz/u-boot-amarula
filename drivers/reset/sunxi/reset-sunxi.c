// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (C) 2018 Amarula Solutions B.V.
 * Author: Jagan Teki <jagan@amarulasolutions.com>
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <dm/lists.h>

int sunxi_clk_bind(struct udevice *cdev)
{
	struct udevice *rst_dev;
	char *dev_name = "reset";
	char drv_name[30];
	int ret;

	/**
	 * reset driver doesn't have separate dt binding, so probe them
	 * using driver name.
	 * example: sun50i_a64_ccu is clock driver,
	 *          sun50i_a64_reset is reset driver.
	 */
	strcpy(drv_name, cdev->driver->name);
	memcpy(&drv_name[strlen(drv_name) - 3], dev_name, strlen(dev_name));

	ret = device_bind_driver_to_node(cdev, drv_name, dev_name,
					 dev_ofnode(cdev), &rst_dev);
	if (ret) {
		debug("Warning: failed to bind (%s) reset driver: ret=%d\n",
		      drv_name, ret);
		return ret;
	}

	return 0;
}
