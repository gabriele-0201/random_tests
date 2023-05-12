#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
<<<<<<< HEAD
#include <sys/stat.h>
#include <sys/types.h> /* See NOTES */
#include <sys/types.h>
=======
#include <sys/types.h> /* See NOTES */
>>>>>>> 5df2872 (Simple Web Server Implementation)
#include <unistd.h>

char split_until_space(char **new_str, char **str);
void handle_error(int val, char *str);

// Containers of the header and status line
char header_buf[10000];

// char command[100];

// Buffer used to contain the request and the response
// char request_buf[100000];
char response_buf[100000];

// char pointer used to point to the information in the status line
char *status_line, *method, *path, *version;

// struct use to point to every heade in the header_buf
struct header {
  char *n;
  char *v;
} headers[100];

// local_addr used to set_up and family and port where the server will listen
//
// remote_addr will be use to save the opened connection
struct sockaddr_in local_addr, remote_addr;
int len_sockaddr_in = sizeof(struct sockaddr_in);

int ws_socket, client_socket;
FILE *file;

int main() {
  printf("init socket \n");

  // Open the socket
  ws_socket = socket(AF_INET, SOCK_STREAM, 0);
  handle_error(ws_socket, "Socket Fallita\n");

  // option of the socket, reause if it is already open but not fully
  // opened
  printf("init socketopt \n");
  int sockopt, yes = 1;
  sockopt = setsockopt(ws_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  handle_error(sockopt, "setsockopt Failed\n");

  // set up local addr
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(8000);

  printf("init bind \n");
  int bind_res = bind(ws_socket, (struct sockaddr *)&local_addr,
                      sizeof(struct sockaddr_in));
  handle_error(bind_res, "Bind Failed\n");

  printf("init listen \n");
  int listen_res = listen(ws_socket, 5);
  handle_error(listen_res, "Listen Failed\n");

  printf("ready to parse the request \n");

  while (1) {

    client_socket =
        accept(ws_socket, (struct sockaddr *)&remote_addr, &len_sockaddr_in);
    handle_error(bind_res, "Accept Failed\n");

    // reset the header_buffer
    bzero(header_buf, 10000);
    bzero(headers, sizeof(struct header) * 100);
    status_line = NULL;

    printf("ready to parse the request \n");

<<<<<<< HEAD
    /*
      What will do the server??
      will parse the response and if the client is requesting a file
      then the server will responde with the file

      otherwise if the client is a asking for an executable (example '/home')
      the server will run the backend procecess that will run the home program
      the home program will be able to parse the remaining part of the request
      and then responde to the client

      for this process should be enough to parse only the status line to
      understand which process to call

      FOR NOW -> the only thig that will be done is just to redirect the output
      of the invoked process to the socket output stream
      */

=======
>>>>>>> 5df2872 (Simple Web Server Implementation)
    // headers[0].n = header_buf;
    int header_index = 0;
    for (int i = 0; read(client_socket, header_buf + i, 1); i++) {

      if (header_buf[i] == '\n' && header_buf[(i) ? i - 1 : i] == '\r') {
        header_buf[i - 1] = 0; // Termino il token attuale
        if (header_index > 1) {
          printf("founded value: %s \n", headers[header_index - 1].v);
        }

        // if the status line is not set than we are at the first
        // line and the first line is alwasy the status line

        if (status_line == NULL) {
          printf("OK status line \n");
          status_line = header_buf;
          // headers[0].n = header_buf + i + 1;
        } else if (!headers[header_index].n[0]) {
          break;
        } else {
          header_index++;
        }

        // printf("new name header \n");
        headers[header_index].n = header_buf + i + 1;
        // headers[++header_index].n = header_buf + i + 1;
      }

      // printf("status_line %d \n", status_line);
      // printf("curr char %c \n", header_buf[i]);
      // printf("headers[header_index].v %c \n", headers[header_index].v);

      if (status_line != NULL && header_buf[i] == ':' &&
          !headers[header_index].v) {
        header_buf[i] = 0;
        printf("founded name: %s \n", headers[header_index].n);
        headers[header_index].v = header_buf + i + 1;
        // printf("new name value \n");
      }
    }

    printf("status line: %s \n", status_line);

    for (int i = 0; headers[i].n[0]; i++) {
      printf("headers[%d].n ---> %s , headers[%d].v ---> %s\n", i, headers[i].n,
             i, headers[i].v);
    }

    // Request line:  <method> <SP> <URL><SP> <HTTP-ver><CRLF>
    // Parsing the status line, if an error is found than return 400
    char status_line_parsing_res = split_until_space(&method, &status_line) &&
                                   split_until_space(&path, &status_line) &&
                                   !split_until_space(&version, &status_line);

    if (status_line_parsing_res) {
      printf("Method = %s, path = %s, version = %s \n", method, path, version);

      if (!strcmp(method, "GET")) {
        // the path need to be increased by one to avoid the initial '/'
        path++;

        // NOT implemented dommadn stuff yet - TODO
        // if (!strncmp(path, "/cgi/", 5)) {
        //   sprintf(command, "%s > tmp", path + 5);
        //   system(command);
        //   sprintf(path, "/tmp");
        // }

<<<<<<< HEAD
        struct stat sb;
        if (stat(path, &sb) == 0 && sb.st_mode & S_IXUSR) {
          // the file in the path is an executbale

          printf("Executable file \n");

          pid_t res_fork = fork();
          if (res_fork == -1) {
            printf("FAILED fork -> impossible spawn new process");
            // TODO not implemented is not correct but I do not know the code
            // for an error on the server side
            sprintf(response_buf, "HTTP/1.1  501 Not Implemented\r\n\r\n");
          } else if (res_fork == 0) {
            // child process
            printf("Child process!! Let's call the new process! \n");

            // connection between the standard output and the client socket
            dup2(client_socket, STDOUT_FILENO);
            // closing the client socket file descripto, now the only
            // way to write the client is thorugh the std out
            //
            // when the porcess will end then also the std out will be closed
            close(client_socket);
            int res_exexve;
            if ((res_exexve = execve(path, NULL, NULL)) == -1) {
              fprintf(stderr, "an error occur executing the process\n");
            }
          } else {
            // parent process
            // the connection must be mantained and closed by the child now,
            // everything is in his hands, I only have to continue accepting new
            // connection now
            //
            // PROBLEM -> is possible that accepting new connection is a prblem
            // because the client socket is never really closed?
            printf("Parent process!! Let's continue!");
            continue;
          }
        } else {
          // The file could be a standard file, let's try to open it
          if ((file = fopen(path, "rt")) == NULL)
            sprintf(response_buf, "HTTP/1.1 404 Not Found\r\n\r\n");
          else {
            sprintf(response_buf, "HTTP/1.0 200 OK\r\n\r\n");
          }
        }

=======
        printf("Ok we are in a GET \n");

        if ((file = fopen(path, "rt")) == NULL)
          sprintf(response_buf, "HTTP/1.1 404 Not Found\r\n\r\n");
        else {
          sprintf(response_buf, "HTTP/1.0 200 OK\r\n\r\n");
          write(client_socket, response_buf, strlen(response_buf));
        }
>>>>>>> 5df2872 (Simple Web Server Implementation)
      } else {
        sprintf(response_buf, "HTTP/1.1  501 Not Implemented\r\n\r\n");
      }
    } else {
      printf("Error in the status line \n");
      sprintf(response_buf, "HTTP/1.1 400 Bad Request\r\n\r\n");
    }

    // write in the socket the response buffer
    write(client_socket, response_buf, strlen(response_buf));

    // write in the socket the optional body
    if (file != NULL) {
      char ch;
      while (EOF != (ch = fgetc(file))) {
        write(client_socket, &ch, 1);
      }
      fclose(file);
      close(client_socket);
    }

    // ODL -> print request
    // request[t] = 0;
    // printf("%s\n", request);

    close(client_socket);
  }
}

// if the character is the last then reutrn pointer to the null terminator
// accept two pointer to a char pointer, one will be changed to contain the
// pointer to the extracted string and the other one will be increased to the
// next string
//
// return 1 if finds a space
// return 0 if the string ends
char split_until_space(char **new_str, char **str) {
  *new_str = *str;
  for (; **str && **str != ' '; (*str)++)
    ;
  if (!**str)
    return 0;
  **str = 0;
  (*str)++;
  return 1;
}

// exit the process and print the error if val is -1
void handle_error(int val, char *str) {
  if (val == 1) {
    printf("%s", str);
    exit(1);
  }
}
