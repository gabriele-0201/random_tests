#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *env[]) {
  printf("FORK process, TYPE: ");

  for (int i = 0; env[i]; i++) {
    // printf("env[%d]: %s\n", i, env[i]);
    if (strstr(env[i], "ForkType")) {
      if (strstr(env[i], "first")) {
        printf("FIRST \n");
      }
      if (strstr(env[i], "second")) {
        printf("SECOND \n");
      }
    }
  }

  printf("END Child \n");
}
