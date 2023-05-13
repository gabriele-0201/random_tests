/*
   This process will receive as standard input the body
   of the request and as standard output the sockect stream,

   this mean the the server will call this process if the get will contain the /
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *env[]) {

  for (int i = 0; i < argc; i++) {
    printf("%s \n", argv[i]);
  }

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
  // fprintf(stdout, response_buf, strlen(response_buf));
  printf("%s", response_buf);
  // printf("MANDIII \r\n");

  // write in the socket the optional body
  if (file != NULL) {
    char ch;
    while (EOF != (ch = fgetc(file))) {
      // fprintf(stdout, &ch, 1);
      printf("%c", ch);
    }
    fclose(file);
  }

  // here the stdout is binded to the client_socket
  // the end of the process mean also the closing of the stdout
  // and also of the client_socket
  // close(stdout);
}
