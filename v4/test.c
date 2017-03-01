#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

unsigned int getmstime() {
  struct timeval tv;
  if (gettimeofday(&tv, NULL) < 0) {
    perror("gettimeofday");
    exit(-1);
  }
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int main() {
  unsigned int start, end;
  start = getmstime();
  printf("%u\n", start);
  sleep(1);
  end = getmstime();
  printf("%u\n", end);
  printf("diff = %u\n", end - start);
  return 0;
}
