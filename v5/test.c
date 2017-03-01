#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

void handler(int sig) {
  if (sig == SIGALRM) {
    printf("SIGALRM\n");
  }
}
__attribute__((constructor))
void init() {
  printf("construct\n");
  if (signal(SIGALRM, handler) == SIG_ERR) {
    perror("signal");
  }
}


__attribute__((destructor))
void destory() {
  printf("destruct\n");
}


int main() {
  struct itimerval value;
  value.it_value.tv_sec = 0;
  value.it_value.tv_usec = 1000*20;
  value.it_interval.tv_sec = 0;
  value.it_interval.tv_usec = 1000*20;
  if (setitimer(ITIMER_REAL, &value, NULL) < 0) {
    perror("setitimer");
  }
  while(1) sleep(1);
  return 0;
}
