#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef PATH
#define PATH "/usr/bin:/usr/local/bin"
#endif

// static char **split(char *path)
// {
//   char **path_bits = strtok(path, ":");
//   return(path_bits);
// }

static char *find_exec(char *path, char *cmd)
{
  char *res;
  char *string = strdup(path);
   char *token = strtok(string, ":");
   while(token != NULL)
   {
      res = strdup(token);
      strcat(res, "/");
      strcat(res, cmd);
      // printf( "checking: %s\n", res);
      if (access(res, F_OK) == 0)
        return(res);
      token = strtok(NULL, " ");
      free(res);
   }
   free(token);
   return(NULL);
  }

static void type_builtin(char *input, char *path)
{
  char *cmd = &input[5];
  char *res = find_exec(path, cmd);
  if (!strcmp(cmd, "echo") || !strcmp(cmd, "exit") || !strcmp(cmd, "type"))
    printf("%s is a shell builtin\n", cmd);
  else if (res)
    printf("%s is %s\n", cmd, res);
  else
    printf("%s: not found\n", cmd);
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
        type_builtin(input, PATH);
      else
        printf("%s: command not found\n", input);
    }
  }
  return(0);
}
