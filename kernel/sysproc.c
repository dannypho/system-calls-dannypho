#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "process.h"
#include "pstat.h"

extern struct process_control_block process_table[NPROC];

uint64 sys_setPriority(void)
{
  int pid;
  int real_priority;

  argint(0, &pid);
  argint(1, &real_priority);

  if(real_priority < -20 || real_priority > 20)
  {
    return -1;
  }

  struct process_control_block *p;
  for(p = process_table; p < &process_table[NPROC]; p++) 
  {
    acquire(&p->lock);

    if(p->pid == pid)
    {
      p->real_priority = real_priority;
      release(&p->lock);
      return 0;
    }
    release(&p->lock);
  }

  return -1;
}

uint64 sys_setEffectivePriority(void)
{
  int pid;
  int effective_priority;

  argint(0, &pid);
  argint(1, &effective_priority);

  if(effective_priority < -20 || effective_priority > 20)
  {
    return -1;
  }

  struct process_control_block *p;
  for(p = process_table; p < &process_table[NPROC]; p++) 
  {
    acquire(&p->lock);

    if(p->pid == pid)
    {
      p->effective_priority = effective_priority;
      release(&p->lock);
      return 0;
    }
    release(&p->lock);
  }

  return -1;
}

uint64 sys_getpinfo(void)
{
  struct pstat my_pstat;
  uint64 dst;
  argaddr(0, &dst);
  int i = 0;

  struct process_control_block *p;
  for(p = process_table; p < &process_table[NPROC]; p++) 
  { 
    acquire(&p->lock);

    if(p->state != UNUSED)
    {
      strncpy(my_pstat.name[i], p->name, 16);
      my_pstat.pid[i] = p->pid;
      my_pstat.state[i] = p->state;
      my_pstat.real_priority[i] = p->real_priority;
      my_pstat.effective_priority[i] = p->effective_priority;
      my_pstat.inuse[i] = 1;
    }
    release(&p->lock);

    i++;
  }

  if(either_copyout(1, dst, &my_pstat, sizeof(struct pstat)) == -1)
  {
    return -1;
  }

  return 0;
}

uint64 sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0; // not reached
}

uint64 sys_getpid(void)
{
  return myproc()->pid;
}

uint64 sys_fork(void)
{
  return fork();
}

uint64 sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64 sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64 sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64 sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64 sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}