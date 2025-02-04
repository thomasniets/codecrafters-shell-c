#include <stdio.h>
#include <string.h>

int main() {
  while (1)
  {
    // Flush after every printf
    setbuf(stdout, NULL);

    // Uncomment this block to pass the first stage
    printf("$ ");

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin); //fgets results in \n in the end
    if (input[0] != '\n')
    {
      input[strlen(input) - 1] = '\0';
      printf("%s: command not found\n", input);
    }
  }
  return 0;
}
