/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:40 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/18 11:39:45 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clear_struct(t_main *mini, int *doublecount, int *singlecount)
{
	*singlecount = 0;
	*doublecount = 0;
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
