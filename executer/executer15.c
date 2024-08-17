/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exacuter15.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:53:02 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/17 17:17:03 by erkoc            ###   ########.fr       */
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

char	*get_cmd_path(t_main *cmd, char **command, int i)
{
	char	**path;
	char	*temp;
	char	*temp2;

	path = get_path(cmd);
	if (!path)
	{
		printf("PATH ERROR\n");
		exit(1);
	}
	while (path[++i])
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

void	clean_file_names(t_main *mini, int i)
{
	while (mini->inpwoutquotes[i] == 32)
		i++;
	while (mini->tokenized[i] == CHAR)
	{
		mini->inpwoutquotes[i] = 32;
		mini->tokenized[i] = BLANK;
		i++;
	}
}
