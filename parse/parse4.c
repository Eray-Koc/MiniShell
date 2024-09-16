/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:30 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/16 19:59:53 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	tag_all(int i, char *tokenized)
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
		else if (tokenized[i] == '$')
			tokenized[i] = DOLLAR;
		else if (tokenized[i] == ' ')
			tokenized[i] = BLANK;
		else
			tokenized[i] = CHAR;
	}
}

int	if_single(char *tokenized, int i)
{
	i++;
	while (tokenized && tokenized[i] && tokenized[i] != SINGLEQUOTE)
	{
		if (tokenized[i] == '$')
			tokenized[i] = DOLLARINSGL;
		else
			tokenized[i] = CHAR;
		i++;
	}
	return (i);
}

int	if_double(char *tokenized, int i)
{
	i++;
	while (tokenized && tokenized[i] && tokenized[i] != DOUBLEQUOTE)
	{
		if (tokenized[i] == '$')
			tokenized[i] = DOLLARINDBL;
		else
			tokenized[i] = CHAR;
		i++;
	}
	return (i);
}
