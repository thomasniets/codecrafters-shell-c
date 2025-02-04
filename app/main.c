#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  char input[100];
  while (1)
  {
    printf("$ ");
    fflush(stdout);
    fgets(input, 100, stdin); //fgets results in \n in the end
    if (input[0] != '\n')
    {
      if (strcmp(input, "exit 0\n") == 0)
        exit(0);
      input[strlen(input) - 1] = '\0';
      printf("%s: command not found\n", input);
    }
  }
  return 0;
}
