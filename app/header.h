#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>

#define SEP " \n\t\0"
#define MAX_TOKENS 100

int tokenize(char *input, char **args);
int tokenize_for_echo(char *input, char **args);