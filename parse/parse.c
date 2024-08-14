/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:30 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/11 18:17:31 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pipe_in_quotes(t_main *mini)
{
	int	i;
	int	x;
	int	sgc;
	int	dbc;
	int	tmp;

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

char	*tokenize(char *input)
{
	int		i;
	char	*tokenized;

	i = 0;
	i = ft_strlen(input);
	tokenized = ft_substr(input, 0, i);
	i = 0;
	while (tokenized[i])
	{
		if (tokenized[i] == '\'')
			tokenized[i] = SINGLEQUOTE;
		else if (tokenized[i] == '\"')
			tokenized[i] = DOUBLEQUOTE;
		else if (tokenized[i] == '>' && tokenized[i + 1] == '>')
		{
			tokenized[i++] = APPEND;
			tokenized[i] = APPEND;
		}
		else if (tokenized[i] == '>')
			tokenized[i] = OUTPUT;
		else if (tokenized[i] == '<' && tokenized[i + 1] == '<')
		{
			tokenized[i++] = HEREDOC;
			tokenized[i] = HEREDOC;
		}
		else if (tokenized[i] == '<')
			tokenized[i] = INPUT;
		else if (tokenized[i] == '|')
			tokenized[i] = PIPE;
		else if (tokenized[i] == ' ')
			tokenized[i] = BLANK;
		else
			tokenized[i] = CHAR;
		i++;
	}
	i = 0;
	while (tokenized[i])
	{
		int	flag;

		flag = 0;
		if (tokenized[i] == SINGLEQUOTE)
			flag = 1;
		else if (tokenized[i] == DOUBLEQUOTE)
			flag = 2;
		if (flag == 1)
		{
			while (tokenized[++i] != SINGLEQUOTE)
			{
				if (tokenized[i] == '$')
					tokenized[i] = DOLLARINSGL;
				else
					tokenized[i] = CHAR;
			}
			flag = 0;
		}
		else if (flag == 2)
		{
			while (tokenized[++i] != DOUBLEQUOTE)
			{
				if (tokenized[i] == '$')
					tokenized[i] = DOLLARINDBL;
				else
					tokenized[i] = CHAR;
			}
			flag = 0;
		}
		i++;
	}
	return (tokenized);
}

void	isquote_closed(char *str, int i, int *dbc, int *sgc)
{
	while (str[++i])
	{
		if (str[i] == '\'' && *dbc % 2 == 0)
			*sgc += 1;
		else if (str[i] == '\"' && *sgc % 2 == 0)
			*dbc += 1;
	}
}

void	empty_inout_check(t_main *mini)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (mini->tokenized[i])
	{
		if (mini->tokenized[i] != HEREDOC && mini->tokenized[i] != INPUT
			&& mini->tokenized[i] != OUTPUT
			&& mini->tokenized[i] != APPEND
			&& mini->tokenized[i] != BLANK)
			count++;
		if ((mini->tokenized[i] == HEREDOC || mini->tokenized[i] == INPUT
				|| mini->tokenized[i] == OUTPUT
					|| mini->tokenized[i] == APPEND)
			&& count == 0)
			printf("Sol taraf boş\n");
		if (mini->tokenized[i] == HEREDOC || mini->tokenized[i] == APPEND)
		{
			count = 0;
			i = i + 2;
		}
		else if (mini->tokenized[i] == INPUT || mini->tokenized[i] == OUTPUT)
		{
			count = 0;
			i++;
		}
		if (!mini->tokenized[i] && count == 0)
			printf("sağ boş\n");
		i++;
	}
}

void	empyt_pipe_check(t_main *mini)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (mini->tokenized[i])
	{
		if (mini->tokenized[i] == CHAR || mini->tokenized[i] == DOLLARINDBL || mini->tokenized[i] == DOLLARINSGL)
			++count;
		if (count == 0 && mini->tokenized[i] == PIPE)
			printf("Elemanın sol taraf boş\n");
		else if (count != 0 && mini->tokenized[i] == PIPE)
			count = 0;
		i++;
		if (!mini->tokenized[i] && count == 0)
			printf("Elemanın sağ taraf boş\n");
	}
}