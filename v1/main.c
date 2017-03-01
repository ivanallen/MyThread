#include <stdio.h>

int task[3] = {0, 0, 0};
int cur = 0;
void switch_to(int n);

void fun1() {
  while(1) {
    printf("hello, I'm fun1\n");
    sleep(1);
    // 强制切换到线程 2
    switch_to(2);
  }
}

void fun2() {
  while(1) {
    printf("hello, I'm fun2\n");
    sleep(1);
    // 强制切换到线程 1
    switch_to(1);
  }
}

// 线程启动函数
void start(int n) {
  if (n == 1) fun1();
  else if(n == 2) fun2();
}

int main() {
  int stack1[1024] = {0};
  int stack2[1024] = {0};
  task[1] = (int)(stack1+1013);
  task[2] = (int)(stack2+1013);

  // 创建 fun1 线程
  // 初始 switch_to 函数栈帧
  stack1[1013] = 7; // eflags
  stack1[1014] = 6; // eax
  stack1[1015] = 5; // edx
  stack1[1016] = 4; // ecx
  stack1[1017] = 3; // ebx
  stack1[1018] = 2; // esi
  stack1[1019] = 1; // edi
  stack1[1020] = 0; // old ebp
  stack1[1021] = (int)start; // ret to start
  // start 函数栈帧，刚进入 start 函数的样子 
  stack1[1022] = 100;// ret to unknown，如果 start 执行结束，表明线程结束
  stack1[1023] = 1; // start 的参数
  
  // 创建 fun2 线程
  // 初始 switch_to 函数栈帧
  stack2[1013] = 7; // eflags
  stack2[1014] = 6; // eax
  stack2[1015] = 5; // edx
  stack2[1016] = 4; // ecx
  stack2[1017] = 3; // ebx
  stack2[1018] = 2; // esi
  stack2[1019] = 1; // edi
  stack2[1020] = 0; // old ebp
  stack2[1021] = (int)start; // ret to start
  // start 函数栈帧，刚进入 start 函数的样子 
  stack2[1022] = 100;// ret to unknown，如果 start 执行结束，表明线程结束
  stack2[1023] = 2; // start 的参数
  
  switch_to(1);  
}


