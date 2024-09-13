/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:40 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/11 16:41:19 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clear_struct(t_main *mini, int *doublecount, int *singlecount)
{
	*singlecount = 0;
	*doublecount = 0;
	mini->cmd = NULL;
	mini->pid_i = 0;
	mini->ifd = -1;
	mini->ofd = -1;
	mini->input = NULL;
	mini->tokenized = NULL;
	mini->pipecount = 0;
	mini->pipe_locs = 0;
	mini->inpwoutquotes = NULL;
	mini->meta_input = NULL;
	mini->output = NULL;
	mini->heredoc = NULL;
	mini->append = NULL;
	mini->args = NULL;
	mini->status = 0;
	mini->ac = 0;
	mini->ic = 0;
	mini->oc = 0;
	mini->hc = 0;
	mini->oerr = 0;
	mini->ierr = 0;
	mini->flagappend = 0;
	mini->flagheredoc = 0;
	mini->flaginput = 0;
	mini->flagheredoc = 0;
}

int	controls(t_main *mini)
{
	mini->tokenized = tokenize(mini->input);
	count_pipes(mini, -1);
	mini->pipe_locs = malloc(sizeof(int) * mini->pipecount);
	locate_pipes(mini, -1, 0);
	if (empty_inout_check(mini->input, mini->tokenized) == 1)
		return 0;
	return empyt_pipe_check(mini);
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

size_t	ft_strarrlen(char **arr)
{
	size_t	i;

	if (arr == NULL)
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

void	ft_free_str_arr(char **str_arr)
{
	int	i;

	i = 0;
	if (str_arr == NULL)
		return ;
	while (str_arr[i])
	{
		free(str_arr[i]);
		i++;
	}
	free(str_arr);
}
