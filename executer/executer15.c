/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer15.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:53:02 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/17 20:18:07 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	path_error(char *trimmed)
{
	ft_putstr_fd("PATH ERROR\n", 2);
	free (trimmed);
	exit(1);
}

void	error_message(char *path, char *trimmed, char **command)
{
	free_double_pointer(path);
	free(trimmed);
	ft_putstr_fd("minishell ", 2);
	ft_putstr_fd(command[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

char	*get_cmd_path(t_main *cmd, char **command, int i)
{
	char	**path;
	char	*temp;
	char	*temp2;
	char	*trimmed;

	trimmed = ft_strtrim(command[0], " ");
	path = get_path(cmd);
	if (!path)
		path_error(trimmed);
	while (path[++i])
	{
		temp = ft_strjoin (path[i], "/");
		temp2 = ft_strjoin (temp, trimmed);
		if (!access (temp2, X_OK))
		{
			free (temp);
			free (trimmed);
			free_double_pointer(path);
			return (temp2);
		}
		free(temp);
		free(temp2);
	}
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

void	clean_file_names(t_main *mini, int i)
{
	while (mini->inpwoutquotes[i] == 32)
		i++;
	while (mini->tokenized[i] == CHAR || mini->tokenized[i] == DOUBLEQUOTE || \
	mini->tokenized[i] == SINGLEQUOTE || mini->inpwoutquotes[i] == '.')
	{
		mini->inpwoutquotes[i] = 32;
		mini->tokenized[i] = BLANK;
		i++;
	}
}

void	clean_file_names_2(t_main *mini, int i, int p, char *tokenized)
{
	while (mini->pipe_sub[p][i] == 32)
		i++;
	while (tokenized[i] == CHAR || tokenized[i] == DOUBLEQUOTE || \
	tokenized[i] == SINGLEQUOTE || tokenized[i] == DOLLARINDBL || \
	tokenized[i] == DOLLARINSGL || tokenized[i] == DOLLAR)
	{
		mini->pipe_sub[p][i] = 32;
		tokenized[i] = BLANK;
		i++;
	}
	{
		mini->pipe_sub[p][i] = 32;
		tokenized[i] = BLANK;
		i++;
	}
}
