#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "processInfo.h"

extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;
int

sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_getNumProc(void)
{
  int count = 0;

  acquire(&ptable.lock);
  for (int i = 0; i < NPROC; i++){
    if(ptable.proc[i].state != UNUSED) count++;
  }
  release(&ptable.lock);
  return count;
}

int
sys_getMaxPid(void)
{
  int max = 0;

  acquire(&ptable.lock);
  for (int i = 0; i < NPROC; i++){
    if(ptable.proc[i].state != UNUSED) max = (ptable.proc[i].pid > max) ? ptable.proc[i].pid : max;
  }
  release(&ptable.lock);
  return max;
}

int 
sys_getProcInfo(void)
{
  int pid;
  struct processInfo * process_info;

  // x86 arg retrieval: returns -1 on error
  if(argint(0, &pid) < 0 || argptr(1, (char **)&process_info, sizeof(* process_info)) < 0)
    return -1;

  acquire(&ptable.lock);
  for(int i = 0; i < NPROC; i++){
    if(ptable.proc[i].state != UNUSED && ptable.proc[i].pid == pid){
      process_info->state = ptable.proc[i].state;
      process_info->ppid = (ptable.proc[i].parent) ? ptable.proc[i].parent->pid : 0;
      process_info->sz = ptable.proc[i].sz;
      process_info->nrswitch = ptable.proc[i].nrswitch;
      
      int count = 0;
      for(int i = 0; i < NOFILE; i++){
        if(ptable.proc[i].ofile[i]) count++;
      }
      process_info->nfd = count;

      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}