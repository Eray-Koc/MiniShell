/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:30 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/14 19:23:44 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void  pipe_exec_2(t_main *mini, int i)
{
	int	fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipi error\n");
		exit(127);
	}
	mini->pid[mini->pid_i] = fork();
	mini->pid_i++;
	if (mini->pid[mini->pid_i - 1] == 0)
	{
		set_signal(CHILD_P);
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		if (mini->ierr == 1 || mini->oerr == 1)
			exit(1);
		one_cmd_exe_2(mini, i);
		exit(127);
	}
	close(fd[1]);
	dup2(fd[0],0);
	close(fd[0]);
}

void read_and_exec_pipe_sub(t_main *mini, int flag, int i, int fd[2])
{
	mini->pid[mini->pid_i] = fork();
	mini->pid_i++;
	if (mini->pid[mini->pid_i - 1] == 0)
	{
		set_signal(CHILD_P);
		if (mini->ofd != -1)
		{
			dup2(mini->ofd, 1);
			close(mini->ofd);
		}
		if (mini->ifd != -1)
		{
			dup2(mini->ofd, 0);
			close(mini->ifd);
		}
		if (mini->ierr == 1 || mini->oerr == 1)
			exit(1);
		if (!flag)
			one_cmd_exe_2(mini, i);
		else if (flag)
		{	
			run_builtin(mini, mini->pipe_sub[i]);
			exit(0);
		}
		exit(127);
	}
	close(fd[1]);
	dup2(fd[0],0);
	close(fd[0]);
}

void read_and_exec_pipe_sub_2(t_main *mini, int i, int flag, int fd[2])
{


	if (!flag)
	{
		pipe_exec_2(mini, i);
	}
	else
	{
		mini->pid[mini->pid_i] = fork();
		mini->pid_i++;
		if (mini->pid[mini->pid_i - 1] == 0)
		{
			set_signal(CHILD_P);
			close(fd[0]);
			dup2(fd[1], 1);
			close(fd[1]);
			run_builtin(mini, mini->pipe_sub[i]);
			exit(0);
		}
		close(fd[1]);
		dup2(fd[0],0);
		close(fd[0]);
	}
}

void read_and_exec_else(t_main *mini, int i, int flag)
{
	mini->pid[mini->pid_i] = fork();
	mini->pid_i++;
	if (mini->pid[mini->pid_i - 1] == 0)
	{
		set_signal(CHILD_P);
		if (mini->ofd != -1)
		{	
			dup2(mini->ofd, 1);
			close(mini->ofd);
		}
		if (mini->ierr == 1 || mini->oerr == 1)
			exit(1);
		if (!flag)
			one_cmd_exe_2(mini, i);
		else
		{
			run_builtin(mini, mini->pipe_sub[i]);
			exit(0);
		}
		exit(127);
	}
}

void read_and_exec_redirect(t_main *mini, char **tokenized, int i, int fd_2[2])
{
	take_redirects_2(mini, *tokenized, i);
	remove_quotes_from_append(mini, 0, 0, 0);
	remove_quotes_from_meta_input(mini, -1, 0, 0);
	remove_quotes_from_heredoc(mini, 0, 0, 0);
	remove_quotes_from_output(mini, 0);
	if (mini->heredoc && mini->heredoc[0])
	{
		run_heredoc(mini, fd_2);
	}
	open_files_2(mini, *tokenized);
	clean_unnecessary_2(mini, 0, 0, *tokenized, i);
}
void freeall(t_main *mini, char *tokenized)
{
    free(tokenized);
    tokenized = NULL;
    free_double_pointer(mini->append);
    mini->append = NULL; 
    free_double_pointer(mini->meta_input);
    mini->meta_input = NULL; 
    free_double_pointer(mini->output);
    mini->output = NULL; 
    free_double_pointer(mini->heredoc);
    mini->heredoc = NULL;
}

void read_and_exec(t_main *mini, int fd_2[2])
{
	int i;
	int flag;

	char	*tokenized;
	
	i = 0;
	while (mini->pipe_sub[i])
	{
		flag = 0;
		mini->ifd = -1;
		mini->ofd = -1;
		mini->ierr = 0;
		mini->oerr = 0;
		mini->flagappend = 0;
		mini->flagheredoc = 0;
		mini->flaginput = 0;
		mini->flagoutput = 0;
		tokenized = tokenize(mini->pipe_sub[i]);
		if (check_redirects(tokenized))
			read_and_exec_redirect(mini, &tokenized, i, fd_2);
		mini->pipe_sub[i] = remove_quotes_2(mini->pipe_sub[i], tokenized);
		if (check_builtin_2(mini->pipe_sub[i]) == BUILTIN)
			flag = 1;
		if (mini->pipe_sub[i + 1])
		{
			int	fd[2];
			if (pipe(fd) == -1)
			{
				perror("Pipe Error\n");
				exit(127);
			}
			if (mini->ofd != -1 || mini->ifd != -1)
				read_and_exec_pipe_sub(mini, flag, i, fd);
			else
				read_and_exec_pipe_sub_2(mini, i, flag, fd);
		
		}
		else
			read_and_exec_else(mini, i, flag);
		i++;
		set_zero(mini);
		freeall(mini, tokenized); // ls > a | cat a | ls > b | cat b  ---> double veriyor bi önceki tamam
	}
}

char *remove_quotes_2(char *input, char *tokenized)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	ret = malloc(sizeof(char) * ft_strlen(input) + 1);
	while (input[i])
	{
		if (tokenized[i] == SINGLEQUOTE
			|| tokenized[i] == DOUBLEQUOTE)
		{
			i++;
			continue ;
		}
		else
		{
			ret[j] = input[i];
			j++;
		}
		i++;
	}
	ret[j] = '\0';
	free(input);
	return (ret);
}

void	tag_chars_betw_quotes(char *tokenized, int flag, int i)
{
	while (tokenized[++i])
	{
		flag = 0;
		if (tokenized[i] == SINGLEQUOTE)
			flag = 1;
		else if (tokenized[i] == DOUBLEQUOTE)
			flag = 2;
		if (flag == 1)
			i = if_single(tokenized, i);
		else if (flag == 2)
			i = if_double(tokenized, i);
	}
}

char	*remove_quotes(t_main *mini)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	ret = malloc(sizeof(char) * ft_strlen(mini->input) + 1);
	while (mini->input[i])
	{
		if (mini->tokenized[i] == SINGLEQUOTE
			|| mini->tokenized[i] == DOUBLEQUOTE)
		{
			i++;
			continue ;
		}
		else
		{
			ret[j] = mini->input[i];
			j++;
		}
		i++;
	}
	ret[j] = 0;
	return (ret);
}
