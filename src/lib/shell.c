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

char **splitline(char *line) {
  int bufsize = TOKEN_BUFFERSIZE, position = 0x0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;

  if (!tokens) {
    fprintf(stderr, "%s:: allocation error\n", PROGRAM_NAME);
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOKEN_DELIMITERS);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOKEN_BUFFERSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens) {
        fprintf(stderr, "%s:: allocation error\n", PROGRAM_NAME);
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOKEN_DELIMITERS);
  }
  tokens[position] = NULL;
  return tokens;
}

void commandloop(char prompt[PROMPT_BUFFERSIZE]) {
  char *line;
  char **args;
  int status;
  if (prompt == NULL) {
    prompt = "> ";
  }

  do {
    printf("%s", prompt);
        line = readline();
    args = splitline(line);
    status = executeprogram(args);

    free(line);
    free(args);
  } while (status);
}

int launch(char **args) {
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror(PROGRAM_NAME);
      exit(EXIT_FAILURE);
    }
  } else if (pid < 0) {
    perror(PROGRAM_NAME);
    exit(EXIT_FAILURE);
  } else {
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {"cd", "help", "exit"};

int (*builtin_func[])(char **) = {&shellcd, &shellhelp, &shellexit};

int num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

/*
  Builtin function implementations.
*/
int shellcd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "%s:: expected argument to \"cd\"\n", PROGRAM_NAME);
  } else {
    if (chdir(args[1]) != 0) {
      perror(PROGRAM_NAME);
    }
  }
  return 1;
}

int shellhelp(char **args) {
  printf("SSE (Shell Special Edition)\n");
  printf("cd, help, and exit are bult-in functions\n");
}

int shellexit(char **args) { return 0; }

int executeprogram(char **args) {
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return launch(args);
}
