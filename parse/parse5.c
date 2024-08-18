/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:30 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/18 20:20:29 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void read_and_exec(t_main *mini)
{
	t_main *temp;
	int i;
	int pipecount;

	i = 0;
	pipecount = mini->pipecount;
	temp = mini;
	while (mini)
	{
		if (i < pipecount)
		{
			pipe_exec(mini);
		}
		else
		{
			
			one_cmd_exe(mini);
		}
		i++;
		mini = mini->next;
	}
	mini = temp;
}

void pipe_exec(t_main *mini)
{
	int	fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipi error\n");
		exit(127);
	}
	mini->pid = fork();
	if (mini->pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		one_cmd_exe(mini);
		exit(127);
	}
	close(fd[1]);
	dup2(fd[0],0);
	close(fd[0]);
	waitpid(mini->pid, 0, 0);
}

void remove_quotes_foreach(t_main *mini)
{
	t_main	*temp;

	temp = mini;
	while (mini)
	{
		mini->tokenized = tokenize(mini->input);
		mini->inpwoutquotes = remove_quotes(mini);
		mini = mini->next;
	}
	mini = temp;
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
			i = if_single(tokenized, i);
		else if (flag == 2)
			i = if_double(tokenized, i);
	}
}

char	*remove_quotes(t_main *mini)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	ret = malloc(sizeof(char) * ft_strlen(mini->input));
	while (mini->input[i])
	{
		if (mini->tokenized[i] == SINGLEQUOTE
			|| mini->tokenized[i] == DOUBLEQUOTE)
		{
			i++;
			continue ;
		}
		else
		{
			ret[j] = mini->input[i];
			j++;
		}
		i++;
	}
	return (ret);
}
