/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:39:56 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/04 18:32:57 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	str_isdigit(char *str)
{
	int		i;
	char	*temp;

	temp = ft_strtrim(str, " \t");
	if ((temp[0] == '-' || temp[0] == '+') && ft_strlen(temp) > 1)
		i = 1;
	else
		i = 0;
	while (temp[i])
	{
		if (!ft_isdigit(temp[i]))
			return (free(temp), 1);
		i++;
	}
	return (free(temp), 0);
}
void	ft_exit(char **arg)//freelemeleri yap
{
	if (arg[1])
	{
		if (arg[2] != NULL)
		{
			ft_putstr_fd("bash: exit: too many arguments\n", STDERR_FILENO);
			g_global_exit = 1;
			return ;
		}
		if (str_isdigit(arg[1]))
		{
			ft_putstr_fd("bash: exit: ", STDERR_FILENO);
			ft_putstr_fd(arg[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit(255);
		}
		g_global_exit = ft_atoi(arg[1]);
	}
	printf("exit\n");
	exit(g_global_exit);
}