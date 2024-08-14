/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:49:13 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/14 20:04:42 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_struct(t_main *mini, int *doublecount, int *singlecount)
{
	singlecount = 0;
	doublecount = 0;
	mini->cmd = NULL;
	mini->rcmd = NULL;
	mini->input = NULL;
	mini->tokenized = NULL;
	mini->token2 = NULL;
	mini->pipecount = 0;
	mini->pipe_locs = 0;
	mini->inpwoutquotes = NULL;
	mini->arg_flags = NULL;
	mini->meta_input = NULL;
	mini->output = NULL;
	mini->heredoc = NULL;
	mini->append = NULL;
	mini->status = 0;
	mini->ac = 0;
	mini->ic = 0;
	mini->oc = 0;
	mini->hc = 0;
	mini->next = NULL;
}

void	controls(t_main *mini)
{
	mini->tokenized = tokenize(mini->input);
	count_pipes(mini, -1);
	mini->pipe_locs = malloc(sizeof(int) * mini->pipecount);
	locate_pipes(mini, -1, 0);
	empyt_pipe_check(mini);
	empty_inout_check(mini, 0, 0);
}

void	rcmd_exception(t_env *env)
{
	free(env);
	printf("exit\n");
	exit(1);
}

void	quote_exception(t_main *mini)
{
	printf("Dquote!\n");
	free (mini->input);
}

void	set_zero(t_main *mini)
{
	mini->ac = 0;
	mini->hc = 0;
	mini->oc = 0;
	mini->ic = 0;
}

char	*ft_strtrim_2(char *s1, char const *set, t_env *env)
{
	size_t	len;
	size_t	i;
	char	*res;

	i = 0;
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
