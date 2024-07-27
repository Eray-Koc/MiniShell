#include "minishell.h"

char *tokenize_cmd(char *str)
{
	int	i;
	char *cpy;

	i = 0;
	i = ft_strlen(str);
	cpy = ft_substr(str, 0, i);
	i = 0;
	while (cpy[i])
	{
		if (cpy[i] == '\'')
			cpy[i] = SINGLEQUOTE;
		else if (cpy[i] == '\"')
			cpy[i] = DOUBLEQUOTE;
		else if (cpy[i] == '>' && cpy[i + 1] == '>')
		{
			cpy[i++] = APPEND;
			cpy[i] = APPEND;
		}
		else if (cpy[i] == '>')
			cpy[i] = OUTPUT;
		else if (cpy[i] == '<' && cpy[i + 1] == '<')
		{
			cpy[i++] = HEREDOC;
			cpy[i] = HEREDOC;
		}
		else if (cpy[i] == '<')
			cpy[i] = INPUT;
		else if (cpy[i] == '|')
			cpy[i] = PIPE;
		else if (cpy[i] == ' ')
			cpy[i] = BLANK;
		else
			cpy[i] = CHAR;
		i++;
	}
	i = 0;
	while (cpy[i])
	{
		int	flag;
	
		flag = 0;
		if (cpy[i] == SINGLEQUOTE)
			flag = 1;
		else if (cpy[i] == DOUBLEQUOTE)
			flag = 2;
		if (flag == 1)
		{
			while (cpy[++i] != SINGLEQUOTE)
			{
				if (cpy[i] == '$')
					cpy[i] = DOLLARINSGL;
				else
					cpy[i] = CHAR;
			}
			flag = 0;
		}
		else if (flag == 2)
		{
			while (cpy[++i] != DOUBLEQUOTE)
			{
				if (cpy[i] == '$')
					cpy[i] = DOLLARINDBL;
				else
					cpy[i] = CHAR;
			}
			flag = 0;
		}
		i++;
	}
	return (cpy);
}
int check_redirects(char *tokenized)
{
	int	i;

	i = -1;
	while (tokenized[++i])
		if (tokenized[i] == HEREDOC || tokenized[i] == APPEND || tokenized[i] == INPUT || tokenized[i] == OUTPUT)
			return (1);
	return (0);
}

void one_cmd_exe(char *input)
{
	char *tokenized;
	char **splitted_input;

	tokenized = tokenize_cmd(input);
	splitted_input = ft_split(input, ' ');
	
	if (check_redirects(tokenized) == 1)
		printf("İBOSHELL\n");
	else
		printf("KIRWESHELL\n");

}