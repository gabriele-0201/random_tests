#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// TODO: spawn a child process that will run a total different program
// based on a new env variable

void spawn_process(int type) {
  int channels[2];
  char read_buffer[1024];

  if (pipe(channels) == -1) {
    printf("IMP create channels \n");
    exit(1);
  }

  char *possibilities[] = {"ForkType=first", "ForkType=second"};
  char *envp[] = {possibilities[type], 0};

  /* print current env variables
  for (int i = 0; env[i]; i++) {
    printf("env[%d]: %s\n", i, env[i]);
  }
  */

  // make first fork
  pid_t res_fork = fork();
  if (res_fork == -1) {
    printf("FAILED fork");
  } else if (res_fork == 0) {
    // CHILD PROCESS
    printf("Hello from the child process\n");

    // dup2(channels[1], STDOUT_FILENO);
    // close(channels[0]);
    // close(channels[1]);

    int res_exexve;
    if ((res_exexve = execve("fork", NULL, envp)) == -1) {
      printf("an error occur \n");
    }
  } else {
    // PARENT PROCESS
    close(channels[1]);
    int nbytes = read(channels[0], read_buffer, sizeof(read_buffer));
    // printf("Fork process Output:\n\n%.*s\n\n", nbytes, read_buffer);

    printf("Back in the parent from FIRST fork \n");
  }
}

int main(int argc, char *argv[], char *env[]) {
  printf("Parent process \n");

  /* print args
  printf("args: \n");
  for (int i = 0; i < argc; i++)
    printf("argv[%d]: %s\n", i, argv[i]);
  printf("\n");
  */

  spawn_process(0);
  spawn_process(1);

  printf("ENDED parent \n");
}
