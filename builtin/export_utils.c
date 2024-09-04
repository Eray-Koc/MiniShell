/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 15:59:41 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/04 20:06:14 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	find_existing_env_var(t_main *mini, const char *identifier)
{
	int		j;
	size_t	len;
	char	*env_var;

	j = 0;
	while (mini->env[j])
	{
		env_var = ft_strchr(mini->env[j], '=');
		if (env_var)
		{
			len = env_var - mini->env[j];
			if (ft_strncmp(identifier, mini->env[j], len) == 0
				&& ft_strlen(identifier) == len)
				return (j);
		}
		j++;
	}
	return (-1);
}

static	int	update_env_var(t_main *mini, int j, const char *arg)
{
	char	*new_env_var;

	free(mini->env[j]);
	new_env_var = ft_strdup(arg);
	if (!new_env_var)
		return (0);
	mini->env[j] = new_env_var;
	return (1);
}

int	check_env(t_main *mini, const char *identifier, const char *arg)
{
	int	j;

	j = 0;
	j = find_existing_env_var(mini, identifier);
	if (j != -1)
		return (update_env_var(mini, j, arg));
	return (0);
}



int	add_new_env(t_main *mini, const char *arg, int j)
{
	int		env_count;
	char	**new_env;

	j = 0;
	env_count = 0;
	while (mini->env[env_count])
		env_count++;
	new_env = malloc(sizeof(char *) * (env_count + 2));
	if (!new_env)
		return (0);
	while (j < env_count)
	{
		new_env[j] = mini->env[j];
		j++;
	}
	new_env[env_count] = ft_strdup(arg);
	if (!new_env[env_count])
	{
		free(new_env);
		return (0);
	}
	new_env[env_count + 1] = NULL;
	free(mini->env);
	mini->env = new_env;
	return (1);
}