#include "minishell.h"

int pipe_in_quotes(char *str)
{
	int	i;
	int pipecount;
	int *pipe_locs;
	int	x;
	int sgc;
	int dbc;
	int tmp;

	tmp = 0;
	sgc = 0;
	dbc = 0;
	x = 0;
	pipecount = 0;
	i = -1;
	while (str[++i])
		if (str[i] == PIPE)
			pipecount++;
	pipe_locs = malloc(sizeof(int) * pipecount);
	i = -1;
	while (str[++i])
	{
		if (str[i] == PIPE)
		{
			pipe_locs[x] = i;
			x++;
		}
	}
	i = -1;
	while (++i < pipecount)
	{
		x = pipe_locs[i];
		tmp = pipe_locs[i];
		while (0 <= x)
		{
			if (str[x] == SINGLEQUOTE)
				sgc++;
			else if (str[x] == DOUBLEQUOTE)
				dbc++;
			x--;
		}
		while (str[tmp])
		{
			if (str[x] == SINGLEQUOTE)
				sgc++;
			else if (str[x] == DOUBLEQUOTE)
				dbc++;
			tmp++;
		}
	}
	if (dbc % 2 == 0 && sgc == 0)
		return (1);
	return (0);
}

void tokenize(char *str)
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
	pipe_in_quotes(cpy);
	empyt_pipe_check(cpy);
	char **ibozof = ft_split(cpy, PIPE);
	int ii = 0;
	while(ibozof[ii])
	{
		empty_inout_check(ibozof[ii]);
		ii++;
	}
	free(cpy);
}

void isquote_closed(char *str, int i, int *dbc, int *sgc)
{
	while (str[++i])
	{
		if (str[i] == '\'' && *dbc % 2 == 0)
			*sgc += 1;
		else if (str[i] == '\"' && *sgc % 2 == 0)
			*dbc += 1;
	}
}
