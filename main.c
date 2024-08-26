/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:49:13 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/26 15:59:37 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_global_exit;

char	*ft_strtrim_2(char *s1, char const *set, t_env *env)
{
	size_t	len;
	size_t	i;
	char	*res;

	i = 0;
	g_global_exit = 0;
	if (!s1)
		rcmd_exception(env);
	add_history(s1);
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	len = ft_strlen(s1 + i);
	while (len && ft_strchr(set, (s1 + i)[len - 1]))
		len--;
	res = ft_substr(s1, i, len);
	free(s1);
	return (res);
}

void	start_cmd(int doublecount, int singlecount)
{
	t_main	cmd;
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		err_msg(2);
	take_env(&cmd);
	while (1)
	{
		clear_struct(&cmd, &singlecount, &doublecount);
		cmd.input = ft_strtrim_2(readline("iboshell$> "), "\t ", env);
		tab_to_space(cmd.input);
		isquote_closed(cmd.input, -1, &doublecount, &singlecount);
		if (doublecount % 2 != 0 || singlecount % 2 != 0)
		{
			quote_exception(&cmd);
			continue ;
		}
		controls(&cmd);
		split_cmd(&cmd);
		free(cmd.input);
	}
}

int	main(int ac, char **av)
{
	(void)av;
	if (ac != 1)
		err_msg(1);
	start_cmd(0, 0);
}
