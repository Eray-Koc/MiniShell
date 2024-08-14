/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:53:02 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/14 15:20:27 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	**get_path(t_main *mini)
{
	int	i;

	i = 0;
	while (mini->env && mini->env[i])
	{
		if (ft_strnstr (mini->env[i], "PATH=", 5))
		{
			return (ft_split (ft_strchr(mini->env[i], '/'), ':'));
		}
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(t_main *cmd, char **command)
{
	char	**path;
	char	*temp;
	char	*temp2;
	int		i;

	i = 0;
	path = get_path(cmd);
	if (!path)
	{
		printf("PATH ERROR\n");
		exit(1);
	}
	while (path[i])
	{
		temp = ft_strjoin (path[i], "/");
		temp2 = ft_strjoin (temp, command[0]);
		if (!access (temp2, X_OK))
		{
			free (temp);
			return (temp2);
		}
		free(temp);
		free(temp2);
		i++;
	}
	printf("minishell: %s: command not found\n", command[0]);
	exit(127);
	return (NULL);
	}

int	check_redirects(char *tokenized)
{
	int	i;

	i = -1;
	while (tokenized[++i])
		if (tokenized[i] == HEREDOC || tokenized[i] == APPEND
			|| tokenized[i] == INPUT || tokenized[i] == OUTPUT)
			return (1);
	return (0);
}

int	check_redirects_2(char *tokenized)
{
	int	i;

	i = -1;
	while (tokenized[++i])
		if (tokenized[i] == HEREDOC)
			return (1);
	return (0);
}

void	clean_unnecessary(t_main *mini)
{
	int	i;
	int	flag;

	flag = 0;
	i = 0;
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
			while (mini->inpwoutquotes[i] == 32)
				i++;
			while (mini->tokenized[i] == CHAR)
			{
				mini->inpwoutquotes[i] = 32;
				mini->tokenized[i] = BLANK;
				i++;
			}
		}
		i++;
	}
}

void	run_heredoc(t_main *mini, int fd_2[2])
{
	int		fd[2];
	char	*heredoc;
	int	i;

	i = 0;
	pipe (fd);
	mini->pid = fork();
	if (mini->pid == 0)
	{
		dup2(fd_2[0], 0);
		while (mini->heredoc[i])
		{
			heredoc = readline("> ");
			if (!mini->heredoc[i + 1] && !(ft_strlen(heredoc) == ft_strlen(mini->heredoc[i]) && ft_strncmp(heredoc, mini->heredoc[i], ft_strlen(heredoc)) == 0))
			{
				ft_putendl_fd(heredoc, fd[1]);
			}
			if (ft_strlen(heredoc) == ft_strlen(mini->heredoc[i]) && ft_strncmp(heredoc, mini->heredoc[i], ft_strlen(heredoc)) == 0)
			{
				i++;
			}
			free (heredoc);
		}
		exit (0);
	}
	close(fd[1]);
	dup2(fd[0], 0);
	close(fd[0]);
	waitpid(mini->pid, 0, 0);
}

void one_cmd_exe(t_main *mini)
{
	char **splitted_input;
	char	*path;
	splitted_input = ft_split(mini->inpwoutquotes, ' ');
	if (splitted_input[0][0] == '/')
	{
		if (access(splitted_input[0], X_OK))
		{
			printf("minishell: %s: No such file or directory\n", splitted_input[0]);
			exit(127);
		}
		else
			path = splitted_input[0];
	}
	else
		path = get_cmd_path(mini, splitted_input);
	execve(path, splitted_input, mini->env);
}
