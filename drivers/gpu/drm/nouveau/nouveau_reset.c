/*
 * Copyright (C) 2012 Marcin Slusarz <marcin.slusarz@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <linux/debugfs.h>
#include "drmP.h"
#include "nouveau_drm.h"
#include <linux/module.h>

MODULE_PARM_DESC(lockup_recovery, "Reset GPU on lockup (default: 1)\n");
static int nouveau_lockup_recovery = 1;
module_param_named(lockup_recovery, nouveau_lockup_recovery, int, 0600);

static int off(struct nouveau_drm *drm)
{
	struct nouveau_device *device = nv_device(drm->device);
	struct drm_device *dev = drm->dev;
	struct pci_dev *pdev = dev->pdev;
	int ret;

	pm_message_t pmm = { .event = PM_EVENT_SUSPEND };
	atomic_inc(&device->gpureset_in_progress);
	ret = intr_rwsem_down_write_interruptible(&drm->ioctls_rwsem);
	if (ret)
		goto fail2;

	dev->switch_power_state = DRM_SWITCH_POWER_CHANGING;
	ret = nouveau_drm_suspend(pdev, pmm);
	if (ret)
		goto fail;

	dev->switch_power_state = DRM_SWITCH_POWER_OFF;
	return 0;

fail:
	dev->switch_power_state = DRM_SWITCH_POWER_ON;
	intr_rwsem_up_write(&drm->ioctls_rwsem);
fail2:
	atomic_dec(&device->gpureset_in_progress);
	return ret;
}

static void on(struct nouveau_drm *drm)
{
	struct nouveau_device *device = nv_device(drm->device);
	struct drm_device *dev = drm->dev;
	struct pci_dev *pdev = dev->pdev;

	dev->switch_power_state = DRM_SWITCH_POWER_CHANGING;
	atomic_dec(&device->gpureset_in_progress);
	nouveau_drm_resume(pdev);
	dev->switch_power_state = DRM_SWITCH_POWER_ON;

	device->last_gpu_reset = jiffies;
	intr_rwsem_up_write(&drm->ioctls_rwsem);
}

static int __nouveau_reset_device(struct nouveau_drm *drm, bool manual)
{
	struct nouveau_device *device = nv_device(drm->device);
	int ret = -EAGAIN;
	unsigned long start, end;
	int offret;

	if (mutex_trylock(&device->reset_lock) == 0)
		/* gpu reset in progress */
		return -EAGAIN;

	if (time_before(jiffies, device->last_gpu_reset + 10 * DRM_HZ))
		goto out;
	if (!(nouveau_lockup_recovery || manual))
		goto out;

	if (manual)
		NV_INFO(drm, "Manual GPU reset invoked...\n");
	else
		NV_INFO(drm, "GPU lockup detected, resetting... (process: %s[%d])\n",
				current->comm, task_pid_nr(current));

	start = jiffies;
	do {
		offret = off(drm);
	} while (offret != 0 && offret != -EINTR);

	if (offret == 0) {
		on(drm);
		end = jiffies;
		NV_INFO(drm, "GPU reset done, took %lus\n", (end - start) / DRM_HZ);
	} else {
		ret = offret;
		end = jiffies;
		NV_INFO(drm, "GPU reset interrupted after %lus\n", (end - start) / DRM_HZ);
	}

out:
	mutex_unlock(&device->reset_lock);
	return ret;
}

int nouveau_reset_device(struct nouveau_drm *drm)
{
	return __nouveau_reset_device(drm, false);
}
