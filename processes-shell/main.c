#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Tasks
// [x] Parse the input into constituent pieces
// [x] Execute commands with fork, exec, and wait (each command will be a child
// proces)
// [x] Support custom commands: exit, cd, path
// [] Add support for redirection (>)
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
    }

    // Remove newline char
    line[strcspn(line, "\n")] = '\0';

    // Split the string into parts with strsep()
    char *argv[10] = {NULL};
    size_t argc = 0;
    char *token;

    while ((token = strsep(&line, " ")) != NULL) {
      argv[argc] = token;
      argc++;
    }

    pid_t pid = fork();

    if (pid == -1) {
      // Fork failed
      perror("fork");
      return 1;
    } else if (pid == 0) {
      // Child Process

      char *main_cmd = argv[0];

      // Checking for built-in commands
      if ((strstr(main_cmd, "exit") != NULL)) {
        exit(1);
      } else if ((strstr(main_cmd, "cd") != NULL)) {
        if (argc > 1) {
          perror("too many args in cd");
        }
        char *dir = argv[1];
        chdir(dir);
        continue;
      } else if ((strstr(main_cmd, "path") != NULL)) {
        printf("Setting new path\n");
        // takes 0 or more args; each arg separated by whitespace
        // e.g., wish> path /bin /usr/bin
        for (size_t i = 1; i <= argc; i++) {
          search_path[i - 1] =
              argv[i]; // we use i-1 b/c of the main cmd is at argv[0]
        }

        // Printing all the paths
        for (size_t i = 0; i < argc - 1; i++) {
          printf("%s\n", search_path[i]);
        }
        continue;
      }

      // Executing regular commands
      for (size_t i = 0; i <= argc - 1; i++) {
        char *curr_path = search_path[i];
        char cmd_str[100];
        sprintf(cmd_str, "%s/%s", curr_path, main_cmd);
        printf("trying path: %s\n", cmd_str);
        if (access(cmd_str, X_OK) == 0) {
          execv(cmd_str, argv);
          break;
        }
      }
      perror("no valid commands");
    } else {
      // Parent Process
      int rc_wait = wait(NULL);
    }
    free(line);
  }
  return 0;
}
