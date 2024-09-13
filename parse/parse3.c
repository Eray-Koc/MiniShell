/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:58:01 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/13 21:40:45 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_double_pointer(char **str)
{
	int i;

	i = 0;
	//if (!str || !str[0])
	//	return ;
	if (!str)
		return ;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
	str = NULL;
}

void to_be_freed2(t_main *mini)
{
	printf("Malloc Error!\n");
	free(mini->input);
	free(mini->tokenized);
	free(mini->pid);
	free(mini->pipe_locs);
	free_double_pointer(mini->pipe_sub);
	free_double_pointer(mini->append);
	free_double_pointer(mini->meta_input);
	free_double_pointer(mini->output);
	free_double_pointer(mini->heredoc);
}

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
		to_be_freed2(mini);
}

int	check_char(char c)
{
	if (c == CHAR || c == DOLLARINDBL || c == DOLLARINSGL
		|| c == DOUBLEQUOTE || c == SINGLEQUOTE || c == DOLLAR)
		return (1);
	return (0);
}

void if_dollar_else(t_main *mini, int index, int envindex)
{
	char *right;
	char *left;
	char *templeft;
	int tempindex;
	
	right = ft_substr(mini->input, 0, index - 1);
	tempindex = 0;
	while (mini->env[envindex][tempindex] != '=')
		tempindex++;
	tempindex++;
	templeft = ft_substr(mini->env[envindex], tempindex, ft_strlen(mini->env[envindex]) - tempindex);
	while (mini->input[index] && (mini->tokenized[index] == CHAR || mini->tokenized[index] == DOUBLEQUOTE || mini->tokenized[index] == SINGLEQUOTE))
		index++;
	left = ft_strjoin(templeft, mini->input + index);
	free(templeft);
	free(mini->input);
	mini->input = ft_strjoin(right, left);
	free(left);
	free(right);
}

void if_dollar_if(t_main *mini, int index)
{
	char *right;
	char *left;

	if (index - 1 == 0)
		right = ft_strdup("");
	else
		right = ft_substr(mini->input, 0, index - 1);
	while (mini->input[index] && (mini->tokenized[index] == CHAR || mini->tokenized[index] == DOUBLEQUOTE || mini->tokenized[index] == SINGLEQUOTE))
		index++;
	if (ft_strlen(mini->input) - index == 0)
		left = ft_strdup("");
	else
		left = ft_substr(mini->input, index, ft_strlen(mini->input) - index);
	free(mini->input);
	mini->input = ft_strjoin(right, left);
	free(left);
	free(right);
}


void if_dollar_exit_status(t_main *mini, int index)
{
	char *right;
	char *left;
	char *templeft;
	
	if (index - 1 == 0)
		right = ft_strdup("");
	else
		right = ft_substr(mini->input, 0, index - 1);
	while (mini->input[index] && mini->input[index] && (mini->tokenized[index] == CHAR || mini->tokenized[index] == DOUBLEQUOTE || mini->tokenized[index] == SINGLEQUOTE))
		index++;
	templeft = ft_itoa(g_global_exit);
	while (mini->input[index] && mini->input[index] && (mini->tokenized[index] == CHAR || mini->tokenized[index] == DOUBLEQUOTE || mini->tokenized[index] == SINGLEQUOTE))
		index++;
	left = ft_strjoin(templeft, mini->input + index);
	free(mini->input);
	mini->input = ft_strjoin(right, left);
	free(left);
	free(templeft);
	free(right);
}

void if_dollar(t_main *mini, int index)
{
	int envindex;
	int tempindex;
	char *finddolar;

	tempindex = index;
	while (mini->input[tempindex] && (mini->tokenized[tempindex] == CHAR || mini->tokenized[tempindex] == DOUBLEQUOTE || mini->tokenized[tempindex] == SINGLEQUOTE))
		tempindex++;
	finddolar = ft_substr(mini->input, index, tempindex - index);
	envindex = find_env_index(mini->env, finddolar);
	if (check_if_same(finddolar, "?") == 1)
		if_dollar_exit_status(mini, index);
	else if (envindex == -1)
		if_dollar_if(mini, index);
	else
		if_dollar_else(mini, index, envindex);
	free(finddolar);
}

void	fill_red(t_main *mini, int index, int status)
{
	int	temp;

	temp = 0;
	while (mini->input[index] == ' ')
		index++;
	temp = index;
	while (check_char(mini->tokenized[index]) && mini->tokenized[index])
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

void	if_append_2(t_main *mini, int index, int temp, int j, char *tokenized, int p)
{
	mini->ac++;
	mini->flagappend = 1;
	mini->append[mini->ac - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (tokenized[index] && check_char(tokenized[index]))
	{
		mini->append[mini->ac - 1][j] = mini->pipe_sub[p][index];
		j++;
		index++;
	}
	mini->append[mini->ac - 1][j] = '\0';
}

void	if_heredoc_2(t_main *mini, int index, int temp, int j, char *tokenized, int p)
{
	mini->hc++;
	mini->flagheredoc = 1;
	mini->heredoc[mini->hc - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (tokenized[index] && check_char(tokenized[index]))
	{
		mini->heredoc[mini->hc - 1][j] = mini->pipe_sub[p][index];
		j++;
		index++;
	}
	mini->heredoc[mini->hc - 1][j] = '\0';
}

void	if_output_2(t_main *mini, int index, int temp, int j, char *tokenized, int p)
{
	mini->oc++;
	mini->flagoutput = 1;
	mini->output[mini->oc - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (tokenized[index] && check_char(tokenized[index]))
	{
		mini->output[mini->oc - 1][j] = mini->pipe_sub[p][index];
		j++;
		index++;
	}
	mini->output[mini->oc - 1][j] = '\0';
}

void	if_input_2(t_main *mini, int index, int temp, int j, char *tokenized, int p)
{
	mini->ic++;
	mini->flaginput = 1;
	mini->meta_input[mini->ic - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (tokenized[index] && check_char(tokenized[index]))
	{
		mini->meta_input[mini->ic - 1][j] = mini->pipe_sub[p][index];
		j++;
		index++;
	}
	mini->meta_input[mini->ic - 1][j] = '\0';
}

void	fill_red_2(t_main *mini, int index, int status, char *tokenized, int j)
{
	int	temp;

	temp = 0;
	while (mini->pipe_sub[j][index] == ' ')
		index++;
	temp = index;
	while (tokenized[index] && check_char(tokenized[index]))
		index++;
	if (status == APPEND)
		if_append_2(mini, index, temp, 0, tokenized, j);
	else if (status == HEREDOC)
		if_heredoc_2(mini, index, temp, 0, tokenized, j);
	else if (status == OUTPUT)
		if_output_2(mini, index, temp, 0, tokenized, j);
	else
		if_input_2(mini, index, temp, 0, tokenized, j);
}

void	take_redirects_2(t_main *mini, char *tokenized, int j)
{
	int i = 0;
	count_redirects(mini, tokenized);
	allocate_for_redirects(mini);
	set_zero(mini);
	while (tokenized[i])
	{
		if (tokenized[i] == APPEND)
			fill_red_2(mini, i+=2, APPEND, tokenized, j);
		else if (tokenized[i] == HEREDOC)
			fill_red_2(mini, i += 2, HEREDOC, tokenized, j);
		else if (tokenized[i] == OUTPUT)
			fill_red_2(mini, i += 1, OUTPUT, tokenized, j);
		else if (tokenized[i] == INPUT)
			fill_red_2(mini, i += 1, INPUT, tokenized, j);
		else
			i++;
	}
	set_zero(mini);
}

void check_dollar(t_main *mini, int i, int temp)
{
	while (mini->tokenized[i])
	{
		temp = i;
		if (mini->tokenized[i] == DOLLAR)
		{
				if_dollar(mini, ++i);
				free(mini->tokenized);
				mini->tokenized = tokenize(mini->input);
		}
		else
			i++;
	}
}

void	take_redirects(t_main *mini, int i)
{
	count_redirects(mini, mini->tokenized);
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
	mini->pid = malloc(sizeof(pid_t) * (mini->pipecount + 1));
}
