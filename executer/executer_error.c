/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:06:47 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/17 20:06:48 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

void	error_write(char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
}

int	is_file(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISREG(path_stat.st_mode));
}

int	runcommanderror(char **command, int i)
{
	if (i == 0)
	{
		error_write(command[0]);
		ft_putendl_fd(": No such file or directory", 2);
		g_global_exit = 127;
		exit(127);
	}
	else if (i == 1)
	{
		error_write(command[0]);
		ft_putendl_fd(": Permission denied", 2);
		g_global_exit = 126;
		exit(126);
	}
	else if (i == 2)
	{
		error_write(command[0]);
		ft_putendl_fd(": is a directory", 2);
		g_global_exit = 126;
		exit(126);
	}
	return (0);
}
