#ifndef __THREAD_H__
#define __THREAD_H__

#define NR_TASKS 16
#define STACK_SIZE 1024 // 1024*4 B

#define THREAD_READY 0
#define THREAD_RUNNING 1
#define THREAD_SLEEP 2
#define THREAD_EXIT 3

struct task_struct {
  /******** 这三行不能动 ***********/
  int id;
  void (*th_fn)();
  int esp; // 保存 esp
  /*********************************/

  unsigned int wakeuptime; // 线程唤醒时间
  int status; // 线程状态
  int counter; // 时间片
  int priority; // 线程优先级
  int stack[STACK_SIZE];
};


int thread_create(int *tid, void (*start_routine)());
int thread_join(int tid);
void mysleep(int seconds);

#endif //__THREAD_H__
