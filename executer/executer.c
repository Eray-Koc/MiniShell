/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak <ibkocak@student.42istanbul.co>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:53:02 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/12 21:15:24 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	clean_unnecessary_2(t_main *mini, int flag, int i, char *tokenized, int p)
{
	while (tokenized[i])
	{
		if (tokenized[i] == OUTPUT || tokenized[i] == INPUT)
		{
			tokenized[i] = BLANK;
			mini->pipe_sub[p][i] = 32;
			flag = 1;
		}
		else if (tokenized[i] == HEREDOC || tokenized[i] == APPEND)
		{
			tokenized[i] = BLANK;
			mini->pipe_sub[p][i] = 32;
			tokenized[i + 1] = BLANK;
			mini->pipe_sub[p][i + 1] = 32;
			flag = 1;
		}
		if (flag)
		{
			flag = 0;
			clean_file_names_2(mini, i, p, tokenized);
		}
		i++;
	}
}


void	clean_unnecessary(t_main *mini, int flag, int i)
{
	while (mini->tokenized[i])
	{
		if (mini->tokenized[i] == OUTPUT || mini->tokenized[i] == INPUT)
		{
			mini->tokenized[i] = BLANK;
			mini->inpwoutquotes[i] = 32;
			flag = 1;
		}
		else if (mini->tokenized[i] == HEREDOC || mini->tokenized[i] == APPEND)
		{
			mini->tokenized[i] = BLANK;
			mini->inpwoutquotes[i] = 32;
			mini->tokenized[i + 1] = BLANK;
			mini->inpwoutquotes[i + 1] = 32;
			flag = 1;
		}
		if (flag)
		{
			flag = 0;
			clean_file_names(mini, i);
		}
		i++;
	}
}

int	check_if_same(char *s1, char *s2)
{
	if (ft_strncmp(s1, s2, ft_strlen(s1)) == 0 \
	&& ft_strncmp(s2, s1, ft_strlen(s2)) == 0)
		return (1);
	return (0);
}

void	heredoc_runner(t_main *mini, int fd[2], int fd_2[2], int i)
{
	char	*heredoc;

	mini->hpid = fork();
	if (mini->hpid == 0)
	{
		set_signal(HEREDOC_P);
		dup2(fd_2[0], 0);
		dup2(fd_2[1], 1);
		while (mini->heredoc[i])
		{
			heredoc = readline("> ");
			if (!mini->heredoc[i + 1] && \
			!check_if_same(mini->heredoc[i], heredoc))
			{
				ft_putendl_fd(heredoc, fd[1]);
			}
			if (check_if_same(mini->heredoc[i], heredoc))
			{
				i++;
			}
			free (heredoc);
		}
		exit (0);
	}
}

void	run_heredoc(t_main *mini, int fd_2[2])
{
	int		fd[2];

	pipe (fd);
	heredoc_runner(mini, fd, fd_2, 0);
	close(fd[1]);
	dup2(fd[0], 0);
	close(fd[0]);
	waitpid(mini->hpid, 0, 0);
}


void	one_cmd_exe_2(t_main *mini, int i)
{
	char	**splitted_input;
	char	*path;

	splitted_input = ft_split(mini->pipe_sub[i], ' ');
	if (splitted_input[0] && splitted_input[0][0] && splitted_input[0][0] == '/')
	{
		path = splitted_input[0];
		if (is_directory(path))
			if (runcommanderror(splitted_input, 2))
				return ;
		if (!is_file(path))
			if (runcommanderror(splitted_input, 0))
				return ;
		if (access(path, X_OK))
			if (runcommanderror(splitted_input, 1))
				return ;
	}
	else
	{
		path = get_cmd_path(mini, splitted_input, -1);
	}
	execve(path, splitted_input, mini->env);
}





void	one_cmd_exe(t_main *mini)
{
	char	**splitted_input;
	char	*path;

	splitted_input = ft_split(mini->inpwoutquotes, ' ');
	if (!splitted_input[0])
	{
		printf("minishell: %s: command not found\n",mini->inpwoutquotes);
			exit(127);
	}
	if (splitted_input[0] && splitted_input[0][0] && splitted_input[0][0] == '/')
	{
		path = splitted_input[0];
		if (is_directory(path))
			if (runcommanderror(splitted_input, 2))
				return ;
		if (!is_file(path))
			if (runcommanderror(splitted_input, 0))
				return ;
		if (access(path, X_OK))
			if (runcommanderror(splitted_input, 1))
				return ;
	}
	else
	{
		path = get_cmd_path(mini, splitted_input, -1);
	}
	execve(path, splitted_input, mini->env);
}
