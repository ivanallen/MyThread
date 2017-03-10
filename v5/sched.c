#include "thread.h"
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>

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
  int i, next, c;


  for (i = 0; i < NR_TASKS; ++i) {
    if (task[i] && task[i]->status != THREAD_EXIT
        && getmstime() > task[i]->wakeuptime) {
      task[i]->status = THREAD_RUNNING;
    }
  }

  while(1) {
    c = -1;
    next = 0;
    for (i = 0; i < NR_TASKS; ++i) {
      if (!task[i]) continue;
      if (task[i]->status == THREAD_RUNNING && task[i]->counter > c) {
        c = task[i]->counter;
        next = i;
      }
    }
    if (c) break;

    // 如果所有任务时间片都是 0，重新调整时间片的值
    if (c == 0) {
      for (i = 0; i < NR_TASKS; ++i) {
        if(task[i]) {
          task[i]->counter = task[i]->priority + (task[i]->counter >> 1);
        }
      }
    }
  }
  
  return task[next];
}

void closealarm() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
      perror("sigprocmask BLOCK");
    }
}

void openalarm() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0) {
      perror("sigprocmask BLOCK");
    }
}


void schedule() {
    struct task_struct *next = pick();
    if (next) {
      switch_to(next);
    }
    // schedule 永远不会返回
}

void mysleep(int seconds) {
  current->wakeuptime = getmstime() + 1000*seconds;
  current->status = THREAD_SLEEP;
  schedule();
}

static void do_timer() {
  if (--current->counter > 0) return;
  current->counter = 0;
  schedule();
  //printf("do_timer\n");
}

__attribute__((constructor))
static void init() {
  struct itimerval value;
  value.it_value.tv_sec = 0;
  value.it_value.tv_usec = 1000;
  value.it_interval.tv_sec = 0;
  value.it_interval.tv_usec = 1000*10; // 10 ms
  if (setitimer(ITIMER_REAL, &value, NULL) < 0) {
    perror("setitimer");
  }
  signal(SIGALRM, do_timer);
}
