/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:58:01 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/17 23:51:59 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	count_redirects(t_main *mini, char *tokenized)
{
	int	i;

	i = 0;
	mini->ac = 0;
	mini->oc = 0;
	mini->hc = 0;
	mini->ic = 0;
	while (tokenized[i])
	{
		if (tokenized[i] == INPUT)
			mini->ic++;
		else if (tokenized[i] == HEREDOC)
			mini->hc++;
		else if (tokenized[i] == OUTPUT)
			mini->oc++;
		else if (tokenized[i] == APPEND)
			mini->ac++;
		i++;
	}
	mini->ac = mini->ac / 2;
	mini->hc = mini->hc / 2;
}

void	if_append(t_main *mini, int index, int temp, int j)
{
	mini->ac++;
	mini->flagappend = 1;
	mini->append[mini->ac - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (check_char(mini->tokenized[index]) && mini->tokenized[index])
	{
		mini->append[mini->ac - 1][j] = mini->input[index];
		j++;
		index++;
	}
	mini->append[mini->ac - 1][j] = '\0';
}

void	if_heredoc(t_main *mini, int index, int temp, int j)
{
	mini->hc++;
	mini->flagheredoc = 1;
	mini->heredoc[mini->hc - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (check_char(mini->tokenized[index]) && mini->tokenized[index])
	{
		mini->heredoc[mini->hc - 1][j] = mini->input[index];
		j++;
		index++;
	}
	mini->heredoc[mini->hc - 1][j] = '\0';
}

void	if_output(t_main *mini, int index, int temp, int j)
{
	mini->oc++;
	mini->flagoutput = 1;
	mini->output[mini->oc - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (check_char(mini->tokenized[index]) && mini->tokenized[index])
	{
		mini->output[mini->oc - 1][j] = mini->input[index];
		j++;
		index++;
	}
	mini->output[mini->oc - 1][j] = '\0';
}

void	if_input(t_main *mini, int index, int temp, int j)
{
	mini->ic++;
	mini->flaginput = 1;
	mini->meta_input[mini->ic - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (check_char(mini->tokenized[index]) && mini->tokenized[index])
	{
		mini->meta_input[mini->ic - 1][j] = mini->input[index];
		j++;
		index++;
	}
	mini->meta_input[mini->ic - 1][j] = '\0';
}
