#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

void schedule();

void fun1() {
  while(1) {
    printf("hello, I'm fun1\n");
    sleep(1);
    schedule();
  }
}

void fun2() {
  while(1) {
    printf("hello, I'm fun2\n");
    sleep(1);
    schedule();
  }
}

void fun3() {
  int i = 2;
  while(i--) {
    printf("hello, I'm fun3\n");
    sleep(1);
    schedule();
  }
}

int main() {
  
  int tid1, tid2, tid3;
  thread_create(&tid1, fun1);
  printf("create thread %d\n", tid1);
  thread_create(&tid2, fun2);
  printf("create thread %d\n", tid2);
  thread_create(&tid3, fun3);
  printf("create thread %d\n", tid3);
  
  int i = 5;
  while(i--) {
    printf("hello, I'm main\n");
    sleep(1);
    schedule();
  }
}


