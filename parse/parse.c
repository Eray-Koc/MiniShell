/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak <ibkocak@student.42istanbul.co>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:30 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/11 23:41:03 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*tokenize(char *input)
{
	char	*tokenized;

	//i = ft_strlen(input);
	tokenized = ft_strdup(input); //ft_substr(input, 0, i);
	tag_all(-1, tokenized);
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

int	check_right(char *tokenized, int i)
{
	if (!tokenized[i])
		return (NONE);
	while (tokenized[i])
	{
		while (tokenized[i] == BLANK)
			i++;
		if (!tokenized[i])
			return (NONE);
		else if (tokenized[i] == APPEND)
			return (APPEND);
		else if (tokenized[i] == INPUT)
			return (INPUT);
		else if (tokenized[i] == OUTPUT)
			return (OUTPUT);
		else if (tokenized[i] == HEREDOC)
			return (HEREDOC);
		else
			return (0);
		i++;
	}
	return (0);
}


int	empty_inout_check(char *input, char *tokenized)
{
	input[0] = input[0];
	/* outputun solu bos olabilir --> ">" için "bash: syntax error near unexpected token `newline'" 
	"> >" için "bash: syntax error near unexpected token `>'"

	inputun solu bos olabilir sağı olamaz
	heredoc solu bos olabilir sağ boş olamaz
	append solu bos olabilir sağı olamaz 
	*/
	int controller = -1;
	int i = 0;

	while (tokenized[i])
	{
		if (tokenized[i] == HEREDOC || tokenized[i] == APPEND)
		{

			i = i + 2;
			controller = check_right(tokenized, i);
		}
		else if (tokenized[i] == INPUT || tokenized[i] == OUTPUT)
		{
			i++;
			controller = check_right(tokenized, i);
		}
		if (controller == HEREDOC)
			printf("minishell: syntax error near unexpected token `<<'\n");
		else if (controller == APPEND)
			printf("minishell: syntax error near unexpected token `>>'\n");
		else if (controller == OUTPUT)
			printf("minishell: syntax error near unexpected token `>'\n");
		else if (controller == INPUT)
			printf("minishell: syntax error near unexpected token `<'\n");
		else if (controller == NONE)
			printf("minishell: syntax error near unexpected token `newline'\n");
		if (controller != 0 && controller != -1)
			return (1);
		i++;
	}
	return 0;
}

int	empyt_pipe_check(t_main *mini)
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
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			return (0);
		}
		else if (count != 0 && mini->tokenized[i] == PIPE)
			count = 0;
		i++;
		if (!mini->tokenized[i] && count == 0)
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			return (0);
		}
	}
	return (1);
}
