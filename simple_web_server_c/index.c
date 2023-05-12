/*
   This process will receive as standard input the body
   of the request and as standard output the sockect stream,

   this mean the the server will call this process if the get will contain the /
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *env[]) {

  // printf("Index process started");

  char response_buf[1000];
  char *path = "index.html";
  FILE *file;

  if ((file = fopen(path, "rt")) == NULL)
    sprintf(response_buf, "HTTP/1.1 404 Not Found\r\n\r\n");
  else {
    sprintf(response_buf, "HTTP/1.0 200 OK\r\n\r\n");
  }

  // write in the socket the response buffer
  fprintf(stdout, response_buf, strlen(response_buf));

  // write in the socket the optional body
  if (file != NULL) {
    char ch;
    while (EOF != (ch = fgetc(file))) {
      fprintf(stdout, &ch, 1);
    }
    fclose(file);
  }
}
