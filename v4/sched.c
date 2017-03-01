#include "thread.h"
#include <stdlib.h>
#include <sys/time.h>

extern struct task_struct *current;
extern struct task_struct *task[NR_TASKS];

void switch_to(struct task_struct *next);

static unsigned int getmstime() {
  struct timeval tv;
  if (gettimeofday(&tv, NULL) < 0) {
    perror("gettimeofday");
    exit(-1);
  }
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static struct task_struct *pick() {
  int current_id  = current->id;
  int i = current_id;

  struct task_struct *next = NULL;
  
  while(1) {
    i = (i + 1) % NR_TASKS;
    if (task[i] && task[i]->status != THREAD_EXIT
        && getmstime() > task[i]->wakeuptime) {
      next = task[i];
      next->status = THREAD_RUNNING;
      break;
    }
  } 
  
  return next;
}



void schedule() {
    struct task_struct *next = pick();
    if (next) {
      switch_to(next);
    }
}

void mysleep(int seconds) {
  current->wakeuptime = getmstime() + 1000*seconds;
  current->status = THREAD_SLEEP;
  schedule();
}
