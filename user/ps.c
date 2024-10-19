#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

struct pstat {
  char name[64][16];          // name of the process
  enum procstate state[64];   // state of the process   
  int inuse[64];              // whether this slot of the process table is in use (1 or 0)
  int effective_priority[64]; // the effective priority of the process
  int real_priority[64];      // the real priority of the process
  int pid[64];                // the PID of each process
  int ticks[64];              // the number of ticks each process has accumulated 
};

int main(int argc, char *argv[])
{
    struct pstat my_pstat;

    setPriority(1, 1);
    setPriority(2, 1);
    setPriority(3, 20);
    getpinfo(&my_pstat);
 

    printf("NAME\tPID\tSTATE\tPRIORITY\n");
    for (int i = 0; i < 64; i++) 
    {
        if (my_pstat.inuse[i] == 1) 
        { 
            printf("%s\t%d\t%d\t%d\n", my_pstat.name[i], my_pstat.pid[i], my_pstat.state[i], my_pstat.real_priority[i]);
        }
    }

    exit(0);
}