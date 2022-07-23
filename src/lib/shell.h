#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define RL_BUFFERSIZE 1024
#define PROGRAM_NAME "sse"

void commandloop(void);
char *readline(void);
char **splitline(char *line);
int launch(char **args);
int executeprogram(char **args);

/* These are the builtin functions that are called when the user types in the
 * command. */
int shellcd(char **args);
int shellhelp(char **args);
int shellexit(char **args);

#endif // SHELL_H++
