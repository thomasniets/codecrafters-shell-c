#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

static char *find_exec(char *path, char *cmd)
{
	char *res = malloc(1024);
	char *string = strdup(path);
	char *token = strtok(string, ":");
	while (token != NULL)
	{
		snprintf(res, 1024, "%s/%s", token, cmd);
		if (access(res, X_OK) == 0)
		{
			free(string);
			return (res);
		}
		token = strtok(NULL, ":");
	}
	free(res);
	free(string);
	return (NULL);
}

static int check_cmd(char *input)
{
    pid_t pid;
    char *cmd;
    char *argv[20];
    char *token = strtok(input, " ");
    int i = 0;
    while (token && i < 19)
    {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;
    if (!argv[0])
        return (1);
    pid = fork();
    if (pid < 0)
    {
        perror("Fork error");
        return (1);
    }
    if (pid == 0)
    {
        if (execvp(argv[0], argv) == -1)
			_exit(1);
    }
    else
    {
		int status;
        waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            return (1);
        return (0);
    }
	return(1);
}

static void type_builtin(char *input, char *path)
{
	char *cmd = &input[5];
	char *builtins[5] = {"echo", "exit", "type", "pwd", "cd"};
	int i = 0;
	while (i < 5)
	{
		if (!strcmp(cmd, builtins[i]))
		{
			printf("%s is a shell builtin\n", cmd);
			return ;
		}
		i++;
	}
	char *res = find_exec(path, cmd);
	if (res)
		printf("%s is %s\n", cmd, res);
	else
		printf("%s: not found\n", cmd);
}

static void change_dir(char *new)
{
	if (!strncmp("~", new, 2))
	{
		char *home = getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "Error: HOME not set\n");
			exit(1);
		}
		if (chdir(home) == -1)
			printf("cd: %s: No such file or directory\n", home);
	}
	else
	{
		if (chdir(new) == -1)
			printf("cd: %s: No such file or directory\n", new);
	}
}

int main()
{
	char input[100];
	char *path = getenv("PATH");
	if (!path)
	{
		fprintf(stderr, "Error: PATH not set\n");
		exit(1);
	}
	while (1)
	{
		printf("$ ");
		fflush(stdout);
		fgets(input, 100, stdin); // fgets results in \n in the end
		if (input[0] != '\n')
		{
			input[strlen(input) - 1] = '\0';
			if (!strcmp(input, "exit 0"))
				exit(0);
			else if (!strncmp(input, "echo ", 5))
				printf("%s\n", &input[5]);
			else if (!strcmp(input, "pwd"))
				printf("%s\n", getcwd(NULL, 1));
			else if (!strncmp(input, "cd ", 3))
				change_dir(&input[3]);
			else if (!strncmp(input, "type ", 5))
				type_builtin(input, path);
			else if (check_cmd(input) == 1)
				printf("%s: command not found\n", input);
		}
	}
	return (0);
}
