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
  int i;

  struct task_struct *next = NULL;

repeat:
  for (i = 0; i < NR_TASKS; ++i) {
    if (task[i] && task[i]->status == THREAD_SLEEP) {
      if (getmstime() > task[i]->wakeuptime)
        task[i]->status = THREAD_RUNNING;
    }
  }

  i = current_id;
  
  while(1) {
    i = (i + 1) % NR_TASKS;
    if (i == current_id) {
      // 循环了一圈说明没找到可被调度的线程
      goto repeat;
    }
    if (task[i] && task[i]->status == THREAD_RUNNING) {
      next = task[i];
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
