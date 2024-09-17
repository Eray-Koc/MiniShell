/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:39:56 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/17 18:16:45 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	throw_error(char **arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg[1], 2);
	ft_putendl_fd(": No such file or directory", 2);
	g_global_exit = 1;
}

void	if_not_null(char **arg, char *pwd, char *temppwd)
{
	if (arg[1][0] == '/')
	{
		if (chdir(arg[1]) == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(arg[1], 2);
			ft_putendl_fd(": No such file or directory", 2);
			g_global_exit = 1;
		}
	}
	else if (arg[1][0] != '/')
	{
		free(pwd);
		pwd = getcwd(NULL, 0);
		temppwd = ft_strjoin(pwd, "/");
		free(pwd);
		pwd = ft_strjoin(temppwd, arg[1]);
		free(temppwd);
		if (chdir(pwd) == -1)
			throw_error(arg);
	}
}

void	if_null(char *home, t_main *mini)
{
	home = get_env(mini->env, "HOME");
	if (home == NULL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		g_global_exit = 1;
		return ;
	}
	if (chdir(home) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(home, 2);
		ft_putendl_fd(": No such file or directory", 2);
		g_global_exit = 1;
	}
}

void	ft_cd(char **arg, t_main *mini)
{
	char	*pwd;
	char	*temppwd;
	char	*home;

	pwd = getcwd(NULL, 0);
	temppwd = ft_strjoin(pwd, "/");
	free(pwd);
	pwd = ft_strjoin(temppwd, arg[1]);
	free(temppwd);
	if (!(arg[1] == NULL))
		if_not_null(arg, pwd, temppwd);
	else
		if_null(home, mini);
	free(pwd);
}
