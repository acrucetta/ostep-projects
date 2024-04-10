#include <fcntl.h> // For open
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For close, dup2, execv
#include <unistd.h>

// Tasks
// [x] Parse the input into constituent pieces
// [x] Execute commands with fork, exec, and wait
// (each command will be a child proces)
// [x] Support custom commands: exit, cd, path
// [x] Add support for redirection (>)
// [] Add support for ampersand commands (&)
// [] Add program errors

int main(int argc, char *argv[]) {
  fprintf(stdout, "Welcome to the Wish shell! \n\n");
  char *search_path[10];
  search_path[0] = "/bin";

  while (1) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    printf("wish> ");
    while ((read = getline(&line, &len, stdin)) == -1) {
      if (feof(stdin)) {
        exit(0);
      }
      perror("getline");
    }

    line[strcspn(line, "\n")] = '\0';

    char *command;
    int n_children = 0;

    while ((command = strsep(&line, "&")) != NULL) {
      if (strlen(command) == 0)
        continue;

      pid_t pid = fork();

      if (pid == -1) {
        // Fork failed
        perror("fork");
        return 1;
      } else if (pid == 0) {
        char *cmd_argv[10] = {NULL};
        size_t cmd_argc = 0;
        char *token;
        bool redirect_stdout = false;
        char *filename;

        while ((token = strsep(&line, " ")) != NULL) {
          if (strstr(token, ">")) {
            redirect_stdout = true;
            continue;
          }
          if (redirect_stdout) {
            filename = token;
            break;
          }
          cmd_argv[cmd_argc] = token;
          cmd_argc++;
        }
        printf("%d: stdout with filename %s\n", redirect_stdout, filename);

        // BUILT-IN COMMANDS
        if ((strstr(cmd_argv[0], "exit") != NULL)) {
          exit(1);
        } else if ((strstr(cmd_argv[0], "cd") != NULL)) {
          if (cmd_argc > 1) {
            perror("too many args in cd");
          }
          char *dir = cmd_argv[1];
          chdir(dir);
          continue;
        } else if ((strstr(cmd_argv[0], "path") != NULL)) {
          printf("Setting new path\n");
          // takes 0 or more args; each arg separated by whitespace
          // e.g., wish> path /bin /usr/bin
          for (size_t i = 1; i <= cmd_argc; i++) {
            search_path[i - 1] =
                cmd_argv[i]; // we use i-1 b/c of the main cmd is at argv[0]
          }

          // Printing all the paths
          for (size_t i = 0; i < cmd_argc - 1; i++) {
            printf("%s\n", search_path[i]);
          }
          continue;
        }

        // SET-UP REDIRECTION FILE
        if (redirect_stdout) {
          int file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC,
                             S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
          if (file_fd < 0) {
            perror("Failed to open file for redirection");
            exit(EXIT_FAILURE);
          }

          if (dup2(file_fd, STDOUT_FILENO) < 0) {
            perror("Failed to redirect standard output");
            exit(EXIT_FAILURE);
          }
          close(file_fd);
        }

        // REGULAR COMMANDS
        for (size_t i = 0; i <= cmd_argc - 1; i++) {
          char *curr_path = search_path[i];
          char cmd_str[100];
          sprintf(cmd_str, "%s/%s", curr_path, cmd_argv[0]);
          if (access(cmd_str, X_OK) == 0) {
            execv(cmd_str, cmd_argv);
            break;
          }
        }
        perror("no valid commands");
      } else {
        n_children++;
      }
    }

    while (n_children > 0) {
      wait(NULL);
      n_children--;
    }

    free(line);
  }
  return 0;
}
