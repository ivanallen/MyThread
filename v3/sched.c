#include "thread.h"
#include <stdlib.h>
#include <sys/time.h>

extern struct task_struct *current;
extern struct task_struct *task[NR_TASKS];

void switch_to(struct task_struct *next);

static struct task_struct *pick() {
  int current_id  = current->id;
  int i = current_id;

  struct task_struct *next = NULL;
  
  while(1) {
    i = (i + 1) % NR_TASKS;
    if (task[i]) {
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

