/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:39:56 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/26 18:32:00 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
void	ft_exit(char **arg, int i)//freelemeleri yap
{
	int		j;

	j = 0;
	if (arg && arg[0])
	{
		arg = ft_split(*arg, ' ');
		j = ft_arg_count(arg);
	}
	if (arg && i == 1)
	{
		printf("exit\n");
		if (arg && arg[0])
			if (exit_with_arg(*arg, j) == 1)
				return ;
		exit(g_global_exit);
	}
}