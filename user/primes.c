#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define puts(string) printf("%s\n", string)
#define NUM_PRIMES 35

int child_rec(int* pipefd);
int main(void) {

  int ret = 0;
  int cpid = -1;
  int pipefd[] = {-1, -1};


  ret = pipe(pipefd);

  if (ret == -1) {
    puts("pipe failed 1\n");
    exit(-1);
  }

  cpid = fork();

  if (cpid == -1) {
      puts("fork failed\n");
      exit(-1);
  }

  if (cpid == 0) { // child
    ret = child_rec(pipefd);
    exit(ret);
  } else { // parent

    ret = close(pipefd[0]); // close the unused read end
    if (ret == -1) {
      puts("closed failed\n");
      exit(-1); 
    }

    for (int i = 2; i <= NUM_PRIMES; i++) {
      ret = write(pipefd[1], &i, sizeof(int));
      if (ret == -1) {
          puts("write failed\n");
          exit(-1);
      }
    }
    ret = close(pipefd[1]); // close the unused read end
    if (ret == -1) {
      puts("closed failed\n");
      exit(-1); 
    }
    while (wait((int*) 0) > 0);
  }
  exit(0);
}

int child_rec(int* pipefd) {
    int first_num = -1;
    int first_read = 0;
    int cpid = 0;
    int read_buf = 0;
    int ret = 0;
    int child_pipefd[] = {-1, -1};
    int read_count = 0;

    ret = pipe(child_pipefd);
    if (ret == -1) {
      puts("pipe failed 2\n");
      exit(-1);
    }

    ret = close(pipefd[1]); // close the unused write end
    if (ret == -1) {
      puts("closed failed\n");
      exit(-1);
    }

    while (read(pipefd[0], &read_buf, sizeof(int)) != 0) {
      read_count += 1;
      if (first_read == 0) {
        first_num = read_buf;
        first_read = 1;
        printf("prime %d\n", first_num);
        continue;
      }
      if (read_buf % first_num != 0) {
        write(child_pipefd[1], &read_buf, sizeof(int));
      }
    }

    ret = close(pipefd[0]); // close the unused read end
    cpid = fork();

    if (read_count > 0 && cpid == 0) {
     child_rec(child_pipefd);
    } 
    return 0;
}
