/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:58:01 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/17 17:09:19 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	count_redirects(t_main *mini)
{
	int	i;

	i = 0;
	mini->ac = 0;
	mini->oc = 0;
	mini->hc = 0;
	mini->ic = 0;
	while (mini->tokenized[i])
	{
		if (mini->tokenized[i] == INPUT)
			mini->ic++;
		else if (mini->tokenized[i] == HEREDOC)
			mini->hc++;
		else if (mini->tokenized[i] == OUTPUT)
			mini->oc++;
		else if (mini->tokenized[i] == APPEND)
			mini->ac++;
		i++;
	}
	mini->ac = mini->ac / 2;
	mini->hc = mini->hc / 2;
}

void	if_append(t_main *mini, int index, int temp, int j)
{
	mini->ac++;
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
