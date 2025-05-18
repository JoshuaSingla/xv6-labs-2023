//
// File-system system calls.
// Mostly argument checking, since we don't trust
// user code, and calls into file.c and fs.c.
//

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h"
#include "sysinfo.h"

uint64
sys_sysinfo(void)
{
  struct sysinfo si = {0, 0};
  struct proc *p = myproc();
  uint64 si_uaddr = 0;

  argaddr(0, &si_uaddr);

  si.freemem = kfreemem();
  si.nproc = nprocs();

  if(copyout(p->pagetable, si_uaddr, (char *)&si, sizeof(si)) < 0)
      return -1;
  return 0;
}
