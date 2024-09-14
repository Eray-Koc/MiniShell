/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:39:56 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/14 18:06:22 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_cd(char **arg)
{
	char *pwd;
	char *temppwd;
	
		
	pwd = getcwd(NULL, 0);
	temppwd =ft_strjoin(pwd, "/");
	free(pwd);
	pwd = ft_strjoin(temppwd, arg[1]);
	free(temppwd);
	if (!(arg[1] == NULL))
	{
		if (arg[1][0] == '/')
		{
			if (chdir(arg[1]) == -1)
			{
				ft_putstr_fd("minishell: cd: ",2);
				ft_putstr_fd(arg[1],2);
				ft_putendl_fd(": No such file or directory",2);
				g_global_exit = 1;
				
			}
		}
		else if (arg[1][0] != '/')
		{
			free(pwd);
			pwd = getcwd(NULL, 0);
			temppwd =ft_strjoin(pwd, "/");
			free(pwd);
			pwd = ft_strjoin(temppwd, arg[1]);
			free(temppwd);
			if (chdir(pwd) == -1)
			{
				ft_putstr_fd("minishell: cd: ",2);
				ft_putstr_fd(arg[1],2);
				ft_putendl_fd(": No such file or directory",2);
				g_global_exit = 1;
			}
		}
	}
	else
	{
		char	*home;

		{
			// Ev dizinini almak için HOME ortam değişkenini kullan
			home = getenv("HOME");
			if (home == NULL)
			{
				ft_putstr_fd("minishell: cd: HOME not set\n", 2);
				g_global_exit = 1;
				return;
			}
			if (chdir(home) == -1)
			{
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(home, 2);
				ft_putendl_fd(": No such file or directory", 2);
				g_global_exit = 1;
			}
		}
	}
	free(pwd);
}

