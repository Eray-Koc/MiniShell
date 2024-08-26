/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:39:56 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/26 15:50:04 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	flag_control(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

void	echo_flag_control(char **arg, int *i)
{
	if (flag_control(arg[*i] + 1) == 1)
		return ;
	*i += 1;
	if (arg[*i] && arg[*i][0] == '-')
		echo_flag_control(arg, i);
}

void	echo_with_arg(char **arg)
{
	int		i;
	int		flag;

	i = 1;
	flag = 0;
	if (arg[i][0] == '-')
		echo_flag_control(arg, &i);
	flag = i;
	while (arg[i])
	{
		printf("%s", arg[i]);
		if (arg[i + 1])
			printf(" ");
		i++;
	}
	if (flag == 1 && arg[flag])
		printf("\n");
	free(arg);
}
void	ft_echo(char **arg)
{
	if (arg && arg[0])
	{
		echo_with_arg(arg);
	}
	else
		printf("\n");
}
