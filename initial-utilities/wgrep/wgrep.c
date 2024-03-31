#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("wgrep: searchterm [file ...]\n");
    exit(1);
  }

  for (size_t i = 2; i < argc; i++) {
    char *filename = argv[i];
    FILE *fp = fopen(filename, "r");
    if (!fp) {
      fprintf(stderr, "File could not be opened.");
      return EXIT_FAILURE;
    }

    char *searchterm = argv[1];
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, fp)) > 0) {
      if (strstr(line, searchterm)) {
        fprintf(stdout, "%s", line);
      }
    }
    fclose(fp);
  }
}
