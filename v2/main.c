#include <stdio.h>
#include <stdlib.h>

#define NR_TASKS 16
#define STACK_SIZE 1024

struct task_struct {
  int id;
  void (*th_fn)();
  int esp; // 保存 esp
  int stack[STACK_SIZE];
};

static struct task_struct init_task = {0, NULL, 0, {0}};

struct task_struct *current = &init_task;

struct task_struct *task[NR_TASKS] = {&init_task,};

void switch_to(struct task_struct *next);

struct task_struct *pick() {
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

void fun1() {
  while(1) {
    printf("hello, I'm fun1\n");
    sleep(1);
    struct task_struct *next = pick();
    if (next) {
      switch_to(next);
    }
  }
}

void fun2() {
  while(1) {
    printf("hello, I'm fun2\n");
    sleep(1);
    struct task_struct *next = pick();
    if (next) {
      switch_to(next);
    }
  }
}

void fun3() {
  int i = 2;
  while(i--) {
    printf("hello, I'm fun3\n");
    sleep(1);
    struct task_struct *next = pick();
    if (next) {
      switch_to(next);
    }
  }
}

// 线程启动函数
void start(struct task_struct *tsk) {
  tsk->th_fn();
  task[tsk->id] = NULL;
  struct task_struct *next = pick();
  if (next) {
    switch_to(next);
  }
}

int thread_create(int *tid, void (*start_routine)()) {
  int id = -1;
  struct task_struct *tsk = (struct task_struct*)malloc(sizeof(struct task_struct));
  
  while(++id < NR_TASKS && task[id]);
  
  if (id == NR_TASKS) return -1;

  task[id] = tsk;

  if (tid) *tid = id;

  tsk->id = id;
  tsk->th_fn = start_routine;
  int *stack = tsk->stack; // 栈顶界限
  tsk->esp = (int)(stack+STACK_SIZE-11);
   
  // 初始 switch_to 函数栈帧
  stack[STACK_SIZE-11] = 7; // eflags
  stack[STACK_SIZE-10] = 6; // eax
  stack[STACK_SIZE-9] = 5; // edx
  stack[STACK_SIZE-8] = 4; // ecx
  stack[STACK_SIZE-7] = 3; // ebx
  stack[STACK_SIZE-6] = 2; // esi
  stack[STACK_SIZE-5] = 1; // edi
  stack[STACK_SIZE-4] = 0; // old ebp
  stack[STACK_SIZE-3] = (int)start; // ret to start
  // start 函数栈帧，刚进入 start 函数的样子 
  stack[STACK_SIZE-2] = 100;// ret to unknown，如果 start 执行结束，表明线程结束
  stack[STACK_SIZE-1] = (int)tsk; // start 的参数

  return 0;
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
    struct task_struct *next = pick();
    if (next) {
      switch_to(next);
    }
  }
}


