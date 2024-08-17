/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:58:01 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/17 17:11:40 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	allocate_for_redirects(t_main *mini)
{
	mini->append = malloc(sizeof(char *) * (mini->ac + 1));
	mini->output = malloc(sizeof(char *) * (mini->oc + 1));
	mini->heredoc = malloc(sizeof(char *) * (mini->hc + 1));
	mini->meta_input = malloc(sizeof(char *) * (mini->ic + 1));
	mini->append[mini->ac] = NULL;
	mini->output[mini->oc] = NULL;
	mini->heredoc[mini->hc] = NULL;
	mini->meta_input[mini->ic] = NULL;
	if (!mini->append || !mini->heredoc || !mini->input || !mini->output)
		return ;
}

int	check_char(char c)
{
	if (c == CHAR || c == DOLLARINDBL || c == DOLLARINSGL
		|| c == DOUBLEQUOTE || c == SINGLEQUOTE)
		return (1);
	return (0);
}

void	fill_red(t_main *mini, int index, int status)
{
	int	temp;
	int	j;

	temp = 0;
	j = 0;
	while (mini->input[index] == ' ')
		index++;
	temp = index;
	while (check_char(mini->tokenized[index] && mini->tokenized[index]))
		index++;
	if (status == APPEND)
		if_append(mini, index, temp, 0);
	else if (status == HEREDOC)
		if_heredoc(mini, index, temp, 0);
	else if (status == OUTPUT)
		if_output(mini, index, temp, 0);
	else
		if_input(mini, index, temp, 0);
}

void	take_redirects(t_main *mini, int i)
{
	count_redirects(mini);
	allocate_for_redirects(mini);
	set_zero(mini);
	while (mini->tokenized[i])
	{
		if (mini->tokenized[i] == APPEND)
			fill_red(mini, i += 2, APPEND);
		else if (mini->tokenized[i] == HEREDOC)
			fill_red(mini, i += 2, HEREDOC);
		else if (mini->tokenized[i] == OUTPUT)
			fill_red(mini, i += 1, OUTPUT);
		else if (mini->tokenized[i] == INPUT)
			fill_red(mini, i += 1, INPUT);
		else
			i++;
	}
}

void	count_pipes(t_main *mini, int i)
{
	mini->pipecount = 0;
	while (mini->tokenized[++i])
		if (mini->tokenized[i] == PIPE)
			mini->pipecount++;
}
