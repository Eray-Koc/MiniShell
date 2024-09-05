/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:39:56 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/05 19:14:47 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_checker(t_main *mini,char **arg)
{
	if (ft_strncmp(arg[0], "pwd", 3) == 0)
		printpwd();
	if (ft_strncmp(arg[0], "cd", 2) == 0)
		ft_cd(arg);
	if (ft_strncmp(arg[0], "echo", 4) == 0)
		ft_echo(arg);
	if (ft_strncmp(arg[0], "exit", 4) == 0)
		ft_exit(arg);
	if (ft_strncmp(arg[0], "export", 6) == 0 && !arg[1])
		ft_env(mini->env, 1);
	if (ft_strncmp(arg[0], "export", 6) == 0)
		ft_export(mini,arg, 1, NULL);
	if (ft_strncmp(arg[0], "env", 3) == 0)
		ft_env(mini->env, 0);
	
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
	builtin_checker(mini ,splitted_input);
	return 0;
}
