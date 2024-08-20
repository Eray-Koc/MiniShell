/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:30 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/20 14:27:08 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void pipe_exec_2(t_main *mini, int i)
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
		one_cmd_exe_2(mini, i);
		exit(127);
	}
	close(fd[1]);
	dup2(fd[0],0);
	close(fd[0]);
	waitpid(mini->pid, 0, 0);
}


void read_and_exec(t_main *mini)
{
	int i;

	i = 0;
	while (mini->pipe_sub[i])
	{
		if (mini->pipe_sub[i + 1])
		{
			pipe_exec_2(mini, i);
		}
		else
		{	
			mini->pid = fork();
			if (mini->pid == 0)
			{
				one_cmd_exe_2(mini, i);
				exit(127);
			}
			waitpid(mini->pid, 0, 0);
		}
		i++;
	}
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

char *remove_quotes_2(char *input, char *tokenized)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	ret = malloc(sizeof(char) * ft_strlen(input) + 1);
	ft_bzero(ret, ft_strlen(input + 1));
	while (input[i])
	{
		if (tokenized[i] == SINGLEQUOTE
			|| tokenized[i] == DOUBLEQUOTE)
		{
			i++;
			continue ;
		}
		else
		{
			ret[j] = input[i];
			j++;
		}
		i++;
	}
	return (ret);
}



void remove_quotes_foreach(t_main *mini)
{
	int		i;
	char	*tokenized;

	i = 0;
	while (mini->pipe_sub[i])
	{
		tokenized = tokenize(mini->pipe_sub[i]);
 		mini->pipe_sub[i] = remove_quotes_2(mini->pipe_sub[i], tokenized);
		i++;
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
	ret = malloc(sizeof(char) * ft_strlen(mini->input) + 1);
	ft_bzero(ret, ft_strlen(mini->input + 1));
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
