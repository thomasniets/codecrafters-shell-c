#include "header.h"

static char *builtins[5] = {"echo", "exit", "type", "pwd", "cd"};
static int num_builtins = 5;

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

static int fork_execvp(char *input)
{
	pid_t pid;
	char *cmd;
	char *args[MAX_TOKENS];
	if (tokenize(input, args) == -1)
		return (1);
	pid = fork();
	if (pid < 0)
	{
		perror("Fork error");
		return (1);
	}
	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
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
	return (1);
}

static void echo_builtin(char *input)
{
	char *args[MAX_TOKENS];
	int i = 2; // skip echo itself and first space
	int args_num = tokenize_for_echo(input, args);
	if (args_num == -1)
		return;
	while (i < args_num - 1)
	{
		printf("%s", args[i]); // spaces saved as arguments
		i++;
	}
	printf("%s\n", args[i]); //-n option not handled
}

static void type_builtin(char *input, char *path)
{
	char *cmd = &input[5];
	int i = 0;
	while (i < num_builtins)
	{
		if (!strcmp(cmd, builtins[i]))
		{
			printf("%s is a shell builtin\n", cmd);
			return;
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
			else if (!strncmp(input, "echo ", 5)) // needs tokenisation
				echo_builtin(input);
			else if (!strcmp(input, "pwd"))
				printf("%s\n", getcwd(NULL, 1));
			else if (!strncmp(input, "cd ", 3))
				change_dir(&input[3]);
			else if (!strncmp(input, "type ", 5))
				type_builtin(input, path);
			else if (fork_execvp(input) == 1) // needs tokenisation
				printf("%s: command not found\n", input);
		}
	}
	return (0);
}
