#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: wzip <filename> \n");
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
      char current_char;
      char char_count = 0;

      while (*ptr != '\0') {
        if (!current_char) {
          current_char = *ptr;
          char_count++;
          ptr++;
          continue;
        }
        if (*ptr == current_char) {
          char_count++;
          ptr++;
          continue;
        } else {
          size_t written = fwrite(&char_count, sizeof(int), 1, stdout);
          if (written != 1) {
            fprintf(stderr, "Error writing to stdout");
            return EXIT_FAILURE;
          }
          size_t written_char = fwrite(&current_char, sizeof(char), 1, stdout);
          if (written_char != 1) {
            fprintf(stderr, "Error writing to stdout");
            return EXIT_FAILURE;
          }
          current_char = *ptr;
          char_count = 1;
        }
        ptr++;
      }
    }
    fclose(fp);
  }
}
