/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:39:56 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/12 16:07:43 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	counter_argv(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	return (i);
}

void	ft_unset(t_main *mini, char **argv)
{
	int		i;
	char	*var;

	g_global_exit = 0;
	if (!mini || !argv)
		return ;
	i = 1;
	while (argv[i])
	{
		var = get_env(mini->env, argv[i]);
		if (!valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: unset: '", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier \n", STDERR_FILENO);
			g_global_exit = 1;
		}
		else if (!ft_strequ(var, ""))
			remove_env(mini, argv[i]);
		i++;
		free(var);
	}
}
