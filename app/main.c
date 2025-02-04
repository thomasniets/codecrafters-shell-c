#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void type_builtin(char *input)
{
  if (!strcmp(&input[5], "echo") || !strcmp(&input[5], "exit") || !strcmp(&input[5], "type"))
    printf("%s is a shell builtin\n", &input[5]);
  else
    printf("%s: command not found\n", &input[5]);
}

int main() {
  char input[100];
  while (1)
  {
    printf("$ ");
    fflush(stdout);
    fgets(input, 100, stdin); //fgets results in \n in the end
    if (input[0] != '\n')
    {
      input[strlen(input) - 1] = '\0';
      if (strcmp(input, "exit 0") == 0)
        exit(0);
      else if (strncmp(input, "echo ", 5) == 0)
        printf("%s\n", &input[5]);
      else if (strncmp(input, "type ", 5) == 0)
        type_builtin(input);
      else
        printf("%s: command not found\n", input);
    }
  }
  return 0;
}
