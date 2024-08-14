/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:30 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/14 21:53:29 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	count_pipes(t_main *mini, int i)
{
	mini->pipecount = 0;
	while (mini->tokenized[++i])
		if (mini->tokenized[i] == PIPE)
			mini->pipecount++;
}

void	locate_pipes(t_main *mini, int i, int x)
{
	while (mini->tokenized[++i])
	{
		if (mini->tokenized[i] == PIPE)
		{
			mini->pipe_locs[x] = i;
			x++;
		}
	}
}


int	tokenize_status(char *tokenized, int i, int status)
{
	tokenized[i] = status;
	tokenized[i + 1] = status;
	return (i + 1);	
}


void	tag_all(int i, char *input, char *tokenized)
{
	while (tokenized[++i])
	{
		if (tokenized[i] == '\'')
			tokenized[i] = SINGLEQUOTE;
		else if (tokenized[i] == '\"')
			tokenized[i] = DOUBLEQUOTE;
		else if (tokenized[i] == '>' && tokenized[i + 1] == '>')
			i = tokenize_status(tokenized, i, APPEND);
		else if (tokenized[i] == '>')
			tokenized[i] = OUTPUT;
		else if (tokenized[i] == '<' && tokenized[i + 1] == '<')
			i = tokenize_status(tokenized, i, HEREDOC);
		else if (tokenized[i] == '<')
			tokenized[i] = INPUT;
		else if (tokenized[i] == '|')
			tokenized[i] = PIPE;
		else if (tokenized[i] == ' ')
			tokenized[i] = BLANK;
		else
			tokenized[i] = CHAR;
	}
}

void	if_single(char *tokenized, int i)
{
	while (tokenized[++i] != SINGLEQUOTE)
	{
		if (tokenized[i] == '$')
			tokenized[i] = DOLLARINSGL;
		else
			tokenized[i] = CHAR;
	}
}

void	if_double(char *tokenized, int i)
{
	while (tokenized[++i] != DOUBLEQUOTE)
	{
		if (tokenized[i] == '$')
			tokenized[i] = DOLLARINDBL;
		else
			tokenized[i] = CHAR;
	}
}

void	tag_chars_betw_quotes(char *tokenized, int flag, int i)
{
	while (tokenized[++i])
	{
		flag = 0;
		if (tokenized[i] == SINGLEQUOTE)
			flag = 1;
		else if (tokenized[i] == DOUBLEQUOTE)
			flag = 2;
		if (flag == 1)
			if_single(tokenized, i);
		else if (flag == 2)
			if_double(tokenized, i);
	}
}

char	*tokenize(char *input)
{
	int		i;
	char	*tokenized;
	int		flag;

	i = ft_strlen(input);
	tokenized = ft_substr(input, 0, i);
	tag_all(-1, input, tokenized);
	i = 0;
	tag_chars_betw_quotes(tokenized, 0, -1);
	return (tokenized);
} // 25

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

int check_char(int c)
{
	if (c == INPUT || c == HEREDOC
		|| c == OUTPUT || c == APPEND)
			return (1);
	return (0);
}

void	empty_inout_check(t_main *mini, int count, int i)
{
	while (mini->tokenized[i])
	{
		if (!check_char(mini->tokenized[i]) && mini->tokenized[i] != BLANK)
			count++;
		if (check_char(mini->tokenized[i]) && count == 0)
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
		if (mini->tokenized[i] == CHAR || mini->tokenized[i] == DOLLARINDBL 
			|| mini->tokenized[i] == DOLLARINSGL)
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
