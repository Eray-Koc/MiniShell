/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:30 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/17 17:13:54 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*tokenize(char *input)
{
	int		i;
	char	*tokenized;

	i = ft_strlen(input);
	tokenized = ft_substr(input, 0, i);
	tag_all(-1, tokenized);
	i = 0;
	tag_chars_betw_quotes(tokenized, 0, -1);
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

int	check_iohc(int c)
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
		if (!check_iohc(mini->tokenized[i]) && mini->tokenized[i] != BLANK)
			count++;
		if (check_iohc(mini->tokenized[i]) && count == 0)
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
