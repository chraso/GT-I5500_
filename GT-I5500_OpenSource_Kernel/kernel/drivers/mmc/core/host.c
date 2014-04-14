/*
 *  linux/drivers/mmc/core/host.c
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
 *  Copyright (C) 2007-2008 Pierre Ossman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  MMC host class device management
 */

#include <linux/device.h>
#include <linux/err.h>
#include <linux/idr.h>
#include <linux/pagemap.h>
#include <linux/leds.h>
#include <linux/moduleparam.h>

#include <linux/mmc/host.h>

#include "core.h"
#include "host.h"

#ifdef	CONFIG_MMC_AUTO_SUSPEND
/* Default idle timeout in seconds */
static int mmc_idle_timeout = 120;
module_param_named(idle_timeout, mmc_idle_timeout, int, 0644);
MODULE_PARM_DESC(idle_timeout, "default idle timeout in secs");
#endif

#define cls_dev_to_mmc_host(d)	container_of(d, struct mmc_host, class_dev)

static void mmc_host_classdev_release(struct device *dev)
{
	struct mmc_host *host = cls_dev_to_mmc_host(dev);
	kfree(host);
}

static struct class mmc_host_class = {
	.name		= "mmc_host",
	.dev_release	= mmc_host_classdev_release,
};

int mmc_register_host_class(void)
{
	return class_register(&mmc_host_class);
}

void mmc_unregister_host_class(void)
{
	class_unregister(&mmc_host_class);
}

static DEFINE_IDR(mmc_host_idr);
static DEFINE_SPINLOCK(mmc_host_lock);

/**
 *	mmc_alloc_host - initialise the per-host structure.
 *	@extra: sizeof private data structure
 *	@dev: pointer to host device model structure
 *
 *	Initialise the per-host structure.
 */
struct mmc_host *mmc_alloc_host(int extra, struct device *dev)
{
	int err;
	struct mmc_host *host;

	if (!idr_pre_get(&mmc_host_idr, GFP_KERNEL))
		return NULL;

	host = kzalloc(sizeof(struct mmc_host) + extra, GFP_KERNEL);
	if (!host)
		return NULL;

	spin_lock(&mmc_host_lock);
	err = idr_get_new(&mmc_host_idr, host, &host->index);
	spin_unlock(&mmc_host_lock);
	if (err)
		goto free;

	dev_set_name(&host->class_dev, "mmc%d", host->index);

	host->parent = dev;
	host->class_dev.parent = dev;
	host->class_dev.class = &mmc_host_class;
	device_initialize(&host->class_dev);

	spin_lock_init(&host->lock);
	init_waitqueue_head(&host->wq);
	INIT_DELAYED_WORK(&host->detect, mmc_rescan);

#ifdef CONFIG_MMC_AUTO_SUSPEND
	mutex_init(&host->auto_suspend_mutex);
	INIT_DELAYED_WORK(&host->auto_suspend, mmc_auto_suspend_work);
	host->idle_timeout = mmc_idle_timeout * HZ;
#endif
	/*
	 * By default, hosts do not support SGIO or large requests.
	 * They have to set these according to their abilities.
	 */
	host->max_hw_segs = 1;
	host->max_phys_segs = 1;
	host->max_seg_size = PAGE_CACHE_SIZE;

	host->max_req_size = PAGE_CACHE_SIZE;
	host->max_blk_size = 512;
	host->max_blk_count = PAGE_CACHE_SIZE / 512;

	return host;

free:
	kfree(host);
	return NULL;
}

EXPORT_SYMBOL(mmc_alloc_host);

#ifdef CONFIG_MMC_AUTO_SUSPEND
static ssize_t
show_idle_timeout(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct mmc_host *host = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d sec\n", host->idle_timeout/HZ);
}

static ssize_t
set_idle_timeout(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	struct mmc_host *host = dev_get_drvdata(dev);
	int value;

	if (sscanf(buf, "%d", &value) != 1 || value >= INT_MAX/HZ ||
			value <= -INT_MAX/HZ)
		return -EINVAL;

	host->idle_timeout = value * HZ;
	if (value < 0)
		mmc_auto_suspend(host, 0); /* Resume the host */
	else
		mmc_auto_suspend(host, 1); /* Try suspending the host */

	return count;
}

static DEVICE_ATTR(idle_timeout, S_IRUGO | S_IWUSR,
		show_idle_timeout, set_idle_timeout);
static struct attribute *dev_attrs[] = {
	&dev_attr_idle_timeout.attr,
	NULL,
};
static struct attribute_group dev_attr_grp = {
	.attrs = dev_attrs,
};
#endif

/**
 *	mmc_add_host - initialise host hardware
 *	@host: mmc host
 *
 *	Register the host with the driver model. The host must be
 *	prepared to start servicing requests before this function
 *	completes.
 */
int mmc_add_host(struct mmc_host *host)
{
	int err;

	WARN_ON((host->caps & MMC_CAP_SDIO_IRQ) &&
		!host->ops->enable_sdio_irq);

	led_trigger_register_simple(dev_name(&host->class_dev), &host->led);

	err = device_add(&host->class_dev);
	if (err)
		return err;

#ifdef CONFIG_DEBUG_FS
	mmc_add_host_debugfs(host);
#endif

#ifdef CONFIG_MMC_AUTO_SUSPEND
	err = sysfs_create_group(&host->parent->kobj, &dev_attr_grp);
	if (err)
		return err;
#endif
	mmc_start_host(host);

	return 0;
}

EXPORT_SYMBOL(mmc_add_host);

/**
 *	mmc_remove_host - remove host hardware
 *	@host: mmc host
 *
 *	Unregister and remove all cards associated with this host,
 *	and power down the MMC bus. No new requests will be issued
 *	after this function has returned.
 */
void mmc_remove_host(struct mmc_host *host)
{
	mmc_stop_host(host);

#ifdef CONFIG_DEBUG_FS
	mmc_remove_host_debugfs(host);
#endif

#ifdef CONFIG_MMC_AUTO_SUSPEND
	sysfs_remove_group(&host->parent->kobj, &dev_attr_grp);
#endif
	device_del(&host->class_dev);

	led_trigger_unregister_simple(host->led);
}

EXPORT_SYMBOL(mmc_remove_host);

/**
 *	mmc_free_host - free the host structure
 *	@host: mmc host
 *
 *	Free the host once all references to it have been dropped.
 */
void mmc_free_host(struct mmc_host *host)
{
	spin_lock(&mmc_host_lock);
	idr_remove(&mmc_host_idr, host->index);
	spin_unlock(&mmc_host_lock);

	put_device(&host->class_dev);
}

EXPORT_SYMBOL(mmc_free_host);

