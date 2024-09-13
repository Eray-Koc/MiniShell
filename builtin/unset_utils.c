/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:39:56 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/12 16:03:28 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_env_variables(char **env)
{
	int	count;

	count = 0;
	while (env[count])
		count++;
	return (count);
}

static	void	finalize_new_env(t_main *mini, char **new_env, int found)
{
	if (found)
	{
		free(mini->env);
		mini->env = new_env;
	}
	else
		free(new_env);
}

char	**envs(int size)
{
	char	**new_env;

	new_env = malloc(sizeof(char *) * size);
	if (!new_env)
		return (NULL);
	return (new_env);
}

void	remove_env(t_main *mini, const char *arg)
{
	int		i;
	int		j;
	int		env_size;
	int		found;
	char	**new_env;

	i = 0;
	j = 0;
	found = 0;
	env_size = count_env_variables(mini->env);
	new_env = envs(env_size);
	while (i < env_size)
	{
		if (ft_strlen(arg) == counter_argv(mini->env[i])
			&& !ft_strncmp(mini->env[i], arg, counter_argv(mini->env[i])))
		{
			free(mini->env[i++]);
			found = 1;
		}
		else
			new_env[j++] = mini->env[i++];
	}
	new_env[j] = NULL;
	finalize_new_env(mini, new_env, found);
}
