/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:49:13 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/17 23:53:20 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strtrim_2(char *s1, char const *set)
{
	size_t	len;
	size_t	i;
	char	*res;

	i = 0;
	add_history(s1);
	if (!s1[0])
	{
		free (s1);
		return (NULL);
	}
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	len = ft_strlen(s1 + i);
	while (len && ft_strchr(set, (s1 + i)[len - 1]))
		len--;
	res = ft_substr(s1, i, len);
	free(s1);
	return (res);
}

void	to_be_freed(t_main *mini)
{
	if (mini->input)
		free(mini->input);
	if (mini->tokenized)
		free(mini->tokenized);
	if (mini->pid)
		free(mini->pid);
	if (mini->pipe_locs)
		free(mini->pipe_locs);
}

void	free_struct(t_main *mini)
{
	free(mini->input);
	free(mini->tokenized);
	free(mini->pipe_locs);
	free(mini->pid);
}

void	start_cmd(int doublecount, int singlecount)
{
	t_main	cmd;
	char	*temp;

	take_env(&cmd);
	while (1)
	{
		set_signal(MAIN_P);
		clear_struct(&cmd, &singlecount, &doublecount);
		temp = readline("fukishell$> ");
		if (!temp)
		{
			ft_putstr_fd("exit\n", 1);
			free_double_pointer(cmd.env);
			exit (0);
		}
		add_history(temp);
		if (!temp[0])
		{
			free (temp);
			continue ;
		}
		cmd.input = ft_strtrim(temp, "\t ");
		free(temp);
		if (!cmd.input[0])
		{
			free(cmd.input);
			g_global_exit = 127;
			continue ;
		}
		tab_to_space(cmd.input);
		isquote_closed(cmd.input, -1, &doublecount, &singlecount);
		if (doublecount % 2 != 0 || singlecount % 2 != 0)
		{
			quote_exception(&cmd);
			continue ;
		}
		if (!controls(&cmd))
		{
			to_be_freed(&cmd);
			continue ;
		}
		split_cmd(&cmd);
		free_struct(&cmd);
	}
}

int	main(int ac, char **av)
{
	(void)av;
	if (ac != 1)
		err_msg(1);
	start_cmd(0, 0);
}
