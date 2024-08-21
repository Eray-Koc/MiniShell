/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:39:56 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/21 18:35:14 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_checker(char **arg)
{
	if (ft_strncmp(arg[0], "pwd", 3) == 0)
		printpwd();
}

int	run_builtin(t_main *mini, char *input)
{	
	char **splitted_input;
	int c;
	int	i;

	i = 1;
	c = 0;
	splitted_input	= ft_split(input, ' ');
	if (!splitted_input)
		return (-1);
	while (splitted_input[c])
	{
		//printf("%s\n", splitted_input[c]);
		c++;	
	}
	if (c > 0)
		mini->inpwoutquotes = splitted_input[0];
	mini->args = malloc(sizeof(char *) * c);
	while (splitted_input[i])
	{
		mini->args[i - 1] = splitted_input[i];
		i++;
	}
	mini->args[i - 1] = NULL;
	builtin_checker(splitted_input);
	return 0;
}
