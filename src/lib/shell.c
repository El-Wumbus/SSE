#include "shell.h"

char *readline(void) {
  int bufsize = RL_BUFFERSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "%s:: allocation error\n", PROGRAM_NAME);
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += RL_BUFFERSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "%s:: allocation error\n", PROGRAM_NAME);
        exit(EXIT_FAILURE);
      }
    }
  }
}

void commandloop(void) {
  char *line;
  char **args;
  int status;

  do {
    printf("> ");
    line = readline();
    args = splitline(line);
    status = executeprogram(args);

    free(line);
    free(args);
  } while (status);
}

int cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "%s:: expected argument to \"cd\"\n", PROGRAM_NAME);
  } else {
    if (chdir(args[1]) != 0) {
      perror("sse");
    }
  }
  return 1;
}