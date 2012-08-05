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

#include "intr_rwsem.h"
#include <linux/sched.h>

void intr_rwsem_init(struct intr_rwsem *r)
{
	atomic_set(&r->readers, 0);
	mutex_init(&r->mutex);
}

int intr_rwsem_down_read_interruptible(struct intr_rwsem *r)
{
	int ret = mutex_lock_interruptible(&r->mutex);
	if (ret)
		return ret;
	atomic_inc(&r->readers);
	mutex_unlock(&r->mutex);
	return 0;
}

void intr_rwsem_down_read(struct intr_rwsem *r)
{
	mutex_lock(&r->mutex);
	atomic_inc(&r->readers);
	mutex_unlock(&r->mutex);
}

void intr_rwsem_up_read(struct intr_rwsem *r)
{
	atomic_dec(&r->readers);
}

int intr_rwsem_down_write_interruptible(struct intr_rwsem *r)
{
	int ret = mutex_lock_interruptible(&r->mutex);
	if (ret)
		return ret;
	while (atomic_read(&r->readers)) {
		if (signal_pending(current)) {
			mutex_unlock(&r->mutex);
			return -EINTR;
		}
		cond_resched();
	}

	return 0;
}

void intr_rwsem_down_write(struct intr_rwsem *r)
{
	mutex_lock(&r->mutex);
	while (atomic_read(&r->readers))
		cond_resched();
}

void intr_rwsem_up_write(struct intr_rwsem *r)
{
	mutex_unlock(&r->mutex);
}
