#include <stdio.h>

int main() {
  // Flush after every printf
  setbuf(stdout, NULL);

  // Uncomment this block to pass the first stage
  printf("$ ");

  // Wait for user input
  char input[100];
  fgets(input, 100, stdin);
  if (input[0] != '\0')
    printf("%s: command not found", input);
  return 0;
}
