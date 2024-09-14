/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:18:14 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/14 23:52:26 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fderror(char *output, int c)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(output, 2);
	if (is_directory(output))
		ft_putstr_fd(": Is a directory\n", 2);
	else if (!is_file(output))
		ft_putstr_fd(": No such file or directory\n", 2);
	else if (c == 1 && access(output, R_OK))
		ft_putstr_fd(": Permission denied\n", 2);
	else if (c == 2 && access(output, W_OK))
		ft_putstr_fd(": Permission denied\n", 2);
	g_global_exit = 1;
}

void	cr_out_file_2(t_main *mini)
{
	if (mini->oerr == 1)
		return ;
	mini->ofd = open(mini->output[mini->oc], O_CREAT
			| O_TRUNC | O_WRONLY, 0644);
	mini->oc++;
	if (mini->ofd == -1)
	{
		mini->oerr = 1;
		fderror(mini->output[mini->oc - 1], 2);
	}
	if (mini->output[mini->oc])
		close (mini->ofd);
}

void	cr_inp_file_2(t_main *mini)
{
	if (mini->ierr == 1)
		return ;
	mini->ifd = open(mini->meta_input[mini->ic], O_RDONLY, 0644);
	mini->ic++;
	if (mini->ifd == -1)
	{
		mini->ierr = 1;
		fderror(mini->meta_input[mini->ic - 1], 1);
	}
	if (mini->meta_input[mini->ic])
		close (mini->ifd);
}

void	cr_out_file(t_main *mini, int *ofd)
{
	if (mini->oerr == 1)
		return ;
	*ofd = open(mini->output[mini->oc], O_CREAT
			| O_TRUNC | O_WRONLY, 0644);
	mini->oc++;
	if (*ofd == -1)
	{
		mini->oerr = 1;
		fderror(mini->output[mini->oc - 1], 2);
	}
	if (mini->output[mini->oc])
		close (*ofd);
	else
		dup2(*ofd, 1);
}

void	cr_appnd_file_2(t_main *mini)
{
	if (mini->oerr == 1)
		return ;
	mini->ofd = open(mini->append[mini->ac], O_CREAT
			| O_APPEND | O_WRONLY, 0644);
	mini->ac++;
	if (mini->ofd == -1)
	{
		mini->oerr = 1;
		fderror(mini->append[mini->ac - 1], 2);
	}
	if (mini->append[mini->ac])
		close (mini->ofd);
}

void	cr_appnd_file(t_main *mini, int *ofd)
{
	if (mini->oerr == 1)
		return ;
	*ofd = open(mini->append[mini->ac], O_CREAT
			| O_APPEND | O_WRONLY, 0644);
	mini->ac++;
	if (*ofd == -1)
	{
		mini->oerr = 1;
		fderror(mini->append[mini->ac - 1], 2);
	}
	if (mini->append[mini->ac])
		close (*ofd);
	else
		dup2(*ofd, 1);
}

void	cr_inp_file(t_main *mini, int *ifd)
{
	if (mini->ierr == 1)
		return ;
	*ifd = open(mini->meta_input[mini->ic], O_RDONLY, 0644);
	mini->ic++;
	if (*ifd == -1)
	{
		fderror(mini->meta_input[mini->ic - 1], 1);
	}
	if (mini->meta_input[mini->ic])
		close (*ifd);
	else
		dup2(*ifd, 0);
}

void	open_files_2(t_main *mini, char *tokenized)
{
	int	i;

	i = 0;
	set_zero(mini);
	mini->ifd = -1;
	mini->ofd = -1;
	while (tokenized[i])
	{
		if (tokenized[i] == OUTPUT)
			cr_out_file_2(mini);
		else if (tokenized[i] == APPEND)
		{
			i++;
			cr_appnd_file_2(mini);
		}
		else if (tokenized[i] == INPUT)
		{
			cr_inp_file_2(mini);
		}
		i++;
	}
}


void	open_files(t_main *mini)
{
	int	i;
	int	ifd;
	int	ofd;

	i = 0;
	set_zero(mini);
	ifd = 0;
	ofd = 0;
	while (mini->tokenized[i])
	{
		if (mini->tokenized[i] == OUTPUT)
		{
			cr_out_file(mini, &ofd);
		}
		else if (mini->tokenized[i] == APPEND)
		{
			i++;
			cr_appnd_file(mini, &ofd);
		}
		else if (mini->tokenized[i] == INPUT)
		{
			cr_inp_file(mini, &ifd);
		}
		i++;
	}
}
