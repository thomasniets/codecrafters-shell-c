#include "header.h"

static char *skip_whitespace(char *str)
{
    while (*str && isspace(*str))
        str++;
    return (str);
}

static char *extract_quoted(char *str, char quote_type) {
    char *current = str + 1;
    while (*current != quote_type && *current != '\0')
        current++;
    if (*current == quote_type)
        return (current + 1);
    return (NULL);
}

static char *extract_unquoted(char *str)
{
    char *current = str;
    while (*current != '\0' && !isspace(*current) && *current != '\'' && *current != '"')
        current++;
    return (current);
}

int tokenize(char *input, char **args)
{
	//probably better to make a copy of input before modifying it directly
	char *token_start;
	char quote_type;
    int token_num = 0;
    char *pos = input;
    char *new_pos;
	pos = skip_whitespace(pos);
    while (*pos != '\0')
	{
        new_pos = skip_whitespace(pos);
		if (new_pos != pos)
		{
			pos = new_pos;
			args[token_num] = strdup(" ");
			token_num++;
		}
        if (*pos == '\0')
            break ;
        token_start = pos;
        if (*pos == '\'' || *pos == '"')
		{
            quote_type = *pos;
            pos = extract_quoted(pos, quote_type);
            token_start++;
			args[token_num] = strndup(token_start, pos - token_start - 1);
        }
		else
		{
            pos = extract_unquoted(pos);
			args[token_num] = strndup(token_start, pos - token_start);
		}
		//will also save empty '' or "" token (important for echo)
        if (!args[token_num])
		{
			printf("strndup error\n");
            return (-1);
		}
        token_num++;
    }
    args[token_num] = NULL;
    return (token_num);
}
