#include <stdio.h>
#include <stdlib.h>

/*
 * Wcat is a library that allows us to
 * print the contents of a file to the
 * command line.
 */
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: ./wcat <filename>");
    return EXIT_FAILURE;
  }
  for (size_t i = 1; i < argc; i++) {
    char *filename = argv[i];
    FILE *fp = fopen(filename, "r");
    if (!fp) {
      fprintf(stderr, "File could not be opened.");
      return EXIT_FAILURE;
    }

    // Read and print the file
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
      printf("%s", buffer);
    }
    fclose(fp);
  }
  return EXIT_SUCCESS;
}
