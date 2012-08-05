#ifndef INTR_RWSEM_H
#define INTR_RWSEM_H

#include <linux/mutex.h>
#include <linux/types.h>

struct intr_rwsem {
       struct mutex mutex;
       atomic_t readers;
};

extern void intr_rwsem_init(struct intr_rwsem *r);
extern void intr_rwsem_down_read(struct intr_rwsem *r);
extern int  intr_rwsem_down_read_interruptible(struct intr_rwsem *r);
extern void intr_rwsem_up_read(struct intr_rwsem *r);
extern void intr_rwsem_down_write(struct intr_rwsem *r);
extern int  intr_rwsem_down_write_interruptible(struct intr_rwsem *r);
extern void intr_rwsem_up_write(struct intr_rwsem *r);

#endif
