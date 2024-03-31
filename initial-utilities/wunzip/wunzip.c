#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: wuzip <filename> \n");
    exit(1);
  }
  for (size_t i = 1; i < argc; i++) {
    char *filename = argv[i];
    FILE *fp = fopen(filename, "r");
    if (!fp) {
      fprintf(stderr, "File could not be opened.");
      return EXIT_FAILURE;
    }
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    while ((linelen = getline(&line, &linecap, fp)) > 0) {
      char *ptr = line;
      while (*ptr != '\0') {
        if (isdigit(*ptr)) {
          int char_count = *ptr - '0';
          ptr++;
          char current_char = *ptr;
          for (size_t i = 0; i < char_count; i++) {
            fwrite(&current_char, sizeof(char), 1, stdout);
          }
        }
        ptr++;
      }
    }
    fclose(fp);
  }
}
