/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:58:01 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/21 18:26:33 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fill_struct(t_main *mini, char **pipe_sub, int x, int j)
{
	int		i;

	i = 0;
	while (++x <= mini->pipecount)
	{
		if (mini->pipe_locs[j] == 0)
			pipe_sub[j] = ft_substr(mini->input, mini->pipe_locs[j - 1] + 1, \
			ft_strlen(mini->input));
		pipe_sub[j] = ft_substr(mini->input, i, mini->pipe_locs[j] - i);
		i = mini->pipe_locs[j] + 1;
		pipe_sub[j] = ft_strtrim(pipe_sub[j], " ");
		j++;
	}
}

int	check_builtin(t_main *mini)
{
	char	**splitted_input;

	splitted_input = ft_split(mini->inpwoutquotes, ' ');
	if (check_if_same(splitted_input[0], "pwd") \
	|| check_if_same(splitted_input[0], "cd") \
	|| check_if_same(splitted_input[0], "echo") \
	|| check_if_same(splitted_input[0], "export") \
	|| check_if_same(splitted_input[0], "unset") \
	|| check_if_same(splitted_input[0], "env") \
	|| check_if_same(splitted_input[0], "exit"))
	{
		free (splitted_input);
		return (BUILTIN);
	}
	return (NONE);
}

void	not_builtin(t_main *mini, int fd_2[2])
{
	if (check_redirects(mini->tokenized) == 1)
	{
		take_redirects(mini, 0);
		remove_quotes_from_append(mini, 0, 0, 0);
		remove_quotes_from_meta_input(mini, -1, 0, 0);
		remove_quotes_from_heredoc(mini, 0, 0, 0);
		remove_quotes_from_output(mini, 0, 0, 0);
		if (mini->heredoc[0])
		{
			run_heredoc(mini, fd_2);
		}
		open_files(mini);
		mini->token2 = ft_strdup(mini->tokenized);
		clean_unnecessary(mini, 0, 0);
	}
	mini->pid = fork();
	if (mini->pid == 0)
	{
		if (check_builtin(mini) == BUILTIN)
			run_builtin(mini, mini->inpwoutquotes);
		//else
			//one_cmd_exe(mini);
		exit(0);
	}
	waitpid(mini->pid, 0, 0);
}

void	split_cmd(t_main *mini)
{
	int	fd_2[2];

	fd_2[0] = dup(0);
	fd_2[1] = dup(1);
	if (mini->pipecount > 0)
	{
		mini->pipe_sub = malloc(sizeof(char *) * (mini->pipecount + 2));
		mini->pipe_sub[mini->pipecount + 1] = NULL;
		fill_struct(mini, mini->pipe_sub, -1, 0);
		remove_quotes_foreach(mini);
		read_and_exec(mini);
	}
	else
	{
		mini->inpwoutquotes = remove_quotes(mini);
		if (check_builtin(mini) == BUILTIN)
			run_builtin(mini , mini->inpwoutquotes);
		else
			not_builtin(mini, fd_2);
	}
	dup2(fd_2[0], 0);
	dup2(fd_2[1], 1);
}
