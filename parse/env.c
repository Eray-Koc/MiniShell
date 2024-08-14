/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:48 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/11 18:17:54 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	take_env_size(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (i);
	while (env[i])
		i++;
	return (i);
}

int	count_environ(char **environ)
{
	int	i;

	i = 0;
	if (!environ)
		return (0);
	while (environ[i])
		i++;
	return (i);
}

void	take_env(t_main *mini)
{
	extern char	**environ;
	int			i;

	mini->env = malloc(sizeof(char *) * (count_environ(environ) + 1));
	i = 0;
	while (environ[i])
	{
		mini->env[i] = ft_strdup(environ[i]);
		i++;
	}
}
