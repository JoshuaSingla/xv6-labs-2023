#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define ERROR_MSG "Usage: sleep <time (deciseconds)>\n"

int main(int argc, char *argv[]) {
  int time = 0;
  int ret = -1;

  if (argc != 2) {
    write(1, ERROR_MSG, strlen(ERROR_MSG));
    exit(-1);
  }

  time = atoi(argv[1]);

  ret = sleep(time);
  if (ret != 0) {
    return ret;
  }

  exit(0);
}
