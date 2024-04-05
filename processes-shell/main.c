#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Tasks
// [x] Parse the input into constituent pieces
// [] Execute commands with fork, exec, and wait (each command will be a child
// proces)
// [] Support custom commands: exit, cd, path

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
    char *argv[10];
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
      printf("Child process\n");

      char *main_cmd = argv[0];
      char cmd_str[100];
      sprintf(cmd_str, "/bin/%s", main_cmd);

      if ((strstr(main_cmd, "exit") == 0)) {
        exit(1);
      } else if ((strstr(main_cmd, "cd") == 0)) {
        // 0 or >1 args should be an error
        if (argc > 1) {
          perror("too many args in cd");
        }
        char *dir = argv[1];
        chdir(dir);
      } else if ((strstr(main_cmd, "path") == 0)) {
        // takes 0 or more args; each arg separated by whitespace
        // e.g., wish> path /bin /usr/bin
        for (size_t i = 1; i<argc; i++) {
          search_path[i-1] = argv[i]; // we use i-1 b/c of the main cmd being at argv[0]
        }
      }

      execv(cmd_str, argv);
    } else {
      // Parent process
      printf("Parent process\n");
      waitpid(pid, NULL, 0);
    }
    free(line);
  }

  return EXIT_SUCCESS;
}

void command_change_dir(char *dir) { chdir(dir); }

void command_path() {}
