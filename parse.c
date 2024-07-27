#include "minishell.h"

int pipe_in_quotes(t_main *mini)
{
	int	i;
	int	x;
	int sgc;
	int dbc;
	int tmp;

	tmp = 0;
	sgc = 0;
	dbc = 0;
	x = 0;
	mini->pipecount = 0;
	i = -1;
	while (mini->tokenized[++i])
		if (mini->tokenized[i] == PIPE)
			mini->pipecount++;
	mini->pipe_locs = malloc(sizeof(int) * mini->pipecount);
	i = -1;
	while (mini->tokenized[++i])
	{
		if (mini->tokenized[i] == PIPE)
		{
			mini->pipe_locs[x] = i;
			x++;
		}
	}
	i = -1;
	while (++i < mini->pipecount)
	{
		x = mini->pipe_locs[i];
		tmp = mini->pipe_locs[i];
		while (0 <= x)
		{
			if (mini->tokenized[x] == SINGLEQUOTE)
				sgc++;
			else if (mini->tokenized[x] == DOUBLEQUOTE)
				dbc++;
			x--;
		}
		while (mini->tokenized[tmp])
		{
			if (mini->tokenized[x] == SINGLEQUOTE)
				sgc++;
			else if (mini->tokenized[x] == DOUBLEQUOTE)
				dbc++;
			tmp++;
		}
	}
	if (dbc % 2 == 0 && sgc == 0)
		return (1);
	return (0);
}

char *tokenize(t_main *mini)
{
	int	i;

	i = 0;
	i = ft_strlen(mini->input);
	mini->tokenized = ft_substr(mini->input, 0, i);
	i = 0;
	while (mini->tokenized[i])
	{
		if (mini->tokenized[i] == '\'')
			mini->tokenized[i] = SINGLEQUOTE;
		else if (mini->tokenized[i] == '\"')
			mini->tokenized[i] = DOUBLEQUOTE;
		else if (mini->tokenized[i] == '>' && mini->tokenized[i + 1] == '>')
		{
			mini->tokenized[i++] = APPEND;
			mini->tokenized[i] = APPEND;
		}
		else if (mini->tokenized[i] == '>')
			mini->tokenized[i] = OUTPUT;
		else if (mini->tokenized[i] == '<' && mini->tokenized[i + 1] == '<')
		{
			mini->tokenized[i++] = HEREDOC;
			mini->tokenized[i] = HEREDOC;
		}
		else if (mini->tokenized[i] == '<')
			mini->tokenized[i] = INPUT;
		else if (mini->tokenized[i] == '|')
			mini->tokenized[i] = PIPE;
		else if (mini->tokenized[i] == ' ')
			mini->tokenized[i] = BLANK;
		else
			mini->tokenized[i] = CHAR;
		i++;
	}
	i = 0;
	while (mini->tokenized[i])
	{
		int	flag;
	
		flag = 0;
		if (mini->tokenized[i] == SINGLEQUOTE)
			flag = 1;
		else if (mini->tokenized[i] == DOUBLEQUOTE)
			flag = 2;
		if (flag == 1)
		{
			while (mini->tokenized[++i] != SINGLEQUOTE)
			{
				if (mini->tokenized[i] == '$')
					mini->tokenized[i] = DOLLARINSGL;
				else
					mini->tokenized[i] = CHAR;
			}
			flag = 0;
		}
		else if (flag == 2)
		{
			while (mini->tokenized[++i] != DOUBLEQUOTE)
			{
				if (mini->tokenized[i] == '$')
					mini->tokenized[i] = DOLLARINDBL;
				else
					mini->tokenized[i] = CHAR;
			}
			flag = 0;
		}
		i++;
	}
	pipe_in_quotes(mini);
	empyt_pipe_check(mini);
	empty_inout_check(mini);
	return (mini->tokenized);
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
