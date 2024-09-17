/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:58:01 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/17 22:39:46 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fill_struct(t_main *mini, char **pipe_sub, int x, int j)
{
	int		i;
	char	*temp;

	i = 0;
	while (++x <= mini->pipecount)
	{
		if (mini->pipe_locs[j] == 0)
		{
			temp = ft_substr(mini->input, mini->pipe_locs[j - 1] + 1, \
			ft_strlen(mini->input));
			pipe_sub[j] = ft_strtrim(temp, " ");
			free(temp);
		}
		else
		{
			temp = ft_substr(mini->input, i, mini->pipe_locs[j] - i);
			i = mini->pipe_locs[j] + 1;
			pipe_sub[j] = ft_strtrim(temp, " ");
			free(temp);
		}
		j++;
	}
}

int	check_builtin_2(char *pipe)
{
	char	**splitted_input;

	if (!pipe)
		return (0);
	splitted_input = ft_split(pipe, ' ');
	if (!splitted_input[0])
		return (free(splitted_input), NONE);
	if (check_if_same(splitted_input[0], "pwd") \
	|| check_if_same(splitted_input[0], "cd") \
	|| check_if_same(splitted_input[0], "echo") \
	|| check_if_same(splitted_input[0], "export") \
	|| check_if_same(splitted_input[0], "unset") \
	|| check_if_same(splitted_input[0], "env") \
	|| check_if_same(splitted_input[0], "exit"))
	{
		free_double_pointer(splitted_input);
		return (BUILTIN);
	}
	free_double_pointer(splitted_input);
	return (NONE);
}

int	check_builtin(t_main *mini)
{
	char	**splitted_input;

	splitted_input = ft_split(mini->inpwoutquotes, ' ');
	if (!splitted_input[0])
		return (free(splitted_input), NONE);
	if (check_if_same(splitted_input[0], "pwd") \
	|| check_if_same(splitted_input[0], "cd") \
	|| check_if_same(splitted_input[0], "echo") \
	|| check_if_same(splitted_input[0], "export") \
	|| check_if_same(splitted_input[0], "unset") \
	|| check_if_same(splitted_input[0], "env") \
	|| check_if_same(splitted_input[0], "exit"))
	{
		free_double_pointer(splitted_input);
		return (BUILTIN);
	}
	free_double_pointer(splitted_input);
	return (NONE);
}

int	if_redirect(t_main *mini, char *temp, int fd_2[2])
{
	take_redirects(mini, 0);
	remove_quotes_from_append(mini, 0, 0, 0);
	remove_quotes_from_meta_input(mini, -1, 0, 0);
	remove_quotes_from_heredoc(mini, 0, 0, 0);
	remove_quotes_from_output(mini, 0);
	if (mini->heredoc[0])
		run_heredoc(mini, fd_2);
	if (g_global_exit != 1)
	{
		open_files(mini);
		clean_unnecessary(mini, 0, 0);
		temp = ft_strtrim(mini->inpwoutquotes, " ");
		if (!temp[0])
		{
			g_global_exit = 0;
			free(temp);
			return (1);
		}
		free (temp);
	}
	return (0);
}

void	not_builtin(t_main *mini, int fd_2[2])
{
	char	*temp;

	if (check_redirects(mini->tokenized) == 1)
		if (if_redirect(mini, temp, fd_2))
			return ;
	mini->pid[mini->pid_i] = fork();
	mini->pid_i++;
	if (mini->pid[mini->pid_i - 1] == 0)
	{
		set_signal(CHILD_P);
		if (mini->ierr == 1 || mini->oerr == 1 || g_global_exit == 1)
			exit(1);
		one_cmd_exe(mini);
		exit(0);
	}
}

void	ft_wait(t_main *mini)
{
	int	i;
	int	status;

	i = 0;
	while (i < mini->pid_i)
	{
		signal(SIGINT, &handler_sigint);
		waitpid(mini->pid[i], &status, 0);
		if (WIFEXITED(status))
			g_global_exit = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_global_exit = 128 + WTERMSIG(status);
		i++;
	}
}

void	builtin_checker(t_main *mini, char **arg)
{
	if (check_if_same(arg[0], "pwd"))
		printpwd();
	if (check_if_same(arg[0], "cd"))
		ft_cd(arg);
	if (check_if_same(arg[0], "echo"))
		ft_echo(arg);
	if (check_if_same(arg[0], "exit"))
		ft_exit(arg);
	if (check_if_same(arg[0], "export") && !arg[1])
		ft_env(mini->env, 1);
	if (check_if_same(arg[0], "export"))
		ft_export(mini, arg, 1, NULL);
	if (check_if_same(arg[0], "env"))
		ft_env(mini->env, 0);
	if (check_if_same(arg[0], "unset"))
		ft_unset(mini, arg);
}

void	locate_spaces(char *mininput)
{
	char	*tokenized;
	int		i;

	i = -1;
	tokenized = tokenize(mininput);
	while (tokenized[++i])
		if (tokenized[i] == BLANK)
			mininput[i] = BLANK;
	free (tokenized);
}

void	quote_check(char temp, int *squotes, int *dquotes)
{
	if (temp == 34 && *squotes % 2 == 0)
		(*dquotes)++;
	if (temp == 39 && *dquotes % 2 == 0)
		(*squotes)++;
}

int	pp_counter(char *temp, int squotes, int dquotes, char c)
{
	int	i;
	int	d;

	d = 0;
	i = -1;
	while (temp[++i] && (quote_check(temp[i], &squotes, &dquotes), 1))
		if (temp[i] == c && (dquotes % 2 == 0 && squotes % 2 == 0))
			d++;
	d++;
	return (d);
}

void	sub(char *temp, char ***ret, char c, int j)
{
	int		i;
	int		d;
	int		squotes;
	int		dquotes;
	char	*tmp;

	i = -1;
	d = 0;
	squotes = 0;
	dquotes = 0;
	while (temp[++i] && (quote_check(temp[i], &squotes, &dquotes), 1))
	{
		if (temp[i] == c && (dquotes % 2 == 0 && squotes % 2 == 0))
		{
			tmp = ft_substr(temp, d, i - d);
			(*ret)[j] = ft_strtrim(tmp, " \t");
			free(tmp);
			j++;
			d = i + 1;
		}
	}
	tmp = ft_substr(temp, d, i - d);
	(*ret)[j++] = ft_strtrim(tmp, " \t");
	free(tmp);
	(*ret)[j] = NULL;
}

char	**mm_split(char *temp, char c)
{
	char	**ret;

	ret = malloc(sizeof(char *) * (pp_counter(temp, 0, 0, c) + 1));
	if (!ret)
		return (NULL);
	sub(temp, &ret, c, 0);
	free(temp);
	temp = NULL;
	return (ret);
}

void	clean_builtin_redirect(t_main *mini, int i, int sq, int dq)
{
	while (mini->input[i] && (quote_check(mini->input[i], &sq, &dq), 1))
	{
		if (ft_strchr("<>", mini->input[i]) && sq % 2 == 0 && dq % 2 == 0)
			break ;
		i++;
	}
	while (mini->input[i] && (mini->input[i] == '<' || mini->input[i] == '>'))
	{
		mini->input[i] = ' ';
		i++;
	}
	while (mini->input[i] && mini->input[i] == ' ')
		i++;
	while (mini->input[i] && (quote_check(mini->input[i], &sq, &dq), 1))
	{
		if (mini->input[i] == ' ' && sq % 2 == 0 && dq % 2 == 0)
			break ;
		mini->input[i] = ' ';
		i++;
	}
	if (mini->input[i])
		clean_builtin_redirect(mini, i, 0, 0);
}

int	remove_quotes_foreach(char **splitted_input, int x)
{
	char	*temp;

	while (splitted_input[++x])
	{
		temp = tokenize(splitted_input[x]);
		splitted_input[x] = remove_quotes_2(splitted_input[x], temp);
		free (temp);
	}
	if (!splitted_input)
		return (1);
	return (0);
}

void	take_args(t_main *mini, char **splitted_input, int i)
{
	int	c;

	c = 0;
	while (splitted_input[c])
		c++;
	if (c > 0)
	{
		free (mini->inpwoutquotes);
		mini->inpwoutquotes = ft_strdup(splitted_input[0]);
	}
	mini->args = malloc(sizeof(char *) * c);
	while (splitted_input[i])
	{
		mini->args[i - 1] = ft_strdup(splitted_input[i]);
		i++;
	}
	mini->args[i - 1] = NULL;
}

int	run_builtin(t_main *mini, char *input)
{
	char	*temp;
	char	**splitted_input;
	char	**pipe_split;
	int		c;	

	c = 0;
	clean_builtin_redirect(mini, 0, 0, 0);
	locate_spaces(mini->input);
	pipe_split = mm_split(ft_strdup(mini->input), '|');
	if (mini->pid_i > 0)
		mini->pid_i -= 1;
	splitted_input = ft_split(pipe_split[mini->pid_i], BLANK);
	if (remove_quotes_foreach(splitted_input, -1))
		return (-1);
	take_args(mini, splitted_input, 1);
	if (mini->ierr != 1 && mini->oerr != 1)
		builtin_checker(mini, splitted_input);
	free_double_pointer(splitted_input);
	free_double_pointer(mini->args);
	free_double_pointer(pipe_split);
	return (0);
}

void eerf(t_main *mini)
{
	if (mini->flagappend)
		free_double_pointer(mini->append);
	else
		free(mini->append);
	if (mini->flaginput)
		free_double_pointer(mini->meta_input);
	else
		free(mini->meta_input);
	if (mini->flagoutput)
		free_double_pointer(mini->output);
	else
		free(mini->output);
	if (mini->flagheredoc)
		free_double_pointer(mini->heredoc);
	else
		free(mini->heredoc);
}

int set_dolar(t_main *mini)
{
	char	*check;

	check_dollar(mini, 0, 0);
	free(mini->tokenized);
	mini->tokenized = tokenize(mini->input);
	locate_pipes(mini, -1, 0);
	check = ft_strtrim(mini->input, " ");
	if (ft_strlen(check) > 0)
	{
		free(check);
		return (1);
	}
	else
	{
		free(check);
		return (-1);
	}
}

void	pipe_exec(t_main *mini, int fd_2[2])
{
	mini->pipe_sub = malloc(sizeof(char *) * (mini->pipecount + 2));
	mini->pipe_sub[mini->pipecount + 1] = NULL;
	fill_struct(mini, mini->pipe_sub, -1, 0);
	read_and_exec(mini, fd_2);
	free_double_pointer(mini->pipe_sub);
}

void	single_exec(t_main *mini, int fd_2[2])
{
	mini->inpwoutquotes = remove_quotes(mini);
	if (check_builtin(mini) == BUILTIN)
	{
		if (check_redirects(mini->tokenized) == 1)
		{
			take_redirects(mini, 0);
			remove_quotes_from_append(mini, 0, 0, 0);
			remove_quotes_from_meta_input(mini, -1, 0, 0);
			remove_quotes_from_heredoc(mini, 0, 0, 0);
			remove_quotes_from_output(mini, 0);
			if (mini->heredoc && mini->heredoc[0])
				run_heredoc(mini, fd_2);
			if (g_global_exit != 1)
			{
				open_files(mini);
				clean_unnecessary(mini, 0, 0);
			}
		}
		if (g_global_exit != 1)
			run_builtin(mini, mini->inpwoutquotes);
	}
	else
		not_builtin(mini, fd_2);
}

void	split_cmd(t_main *mini)
{
	int		fd_2[2];
	int		h_f;

	fd_2[0] = dup(0);
	fd_2[1] = dup(1);
	h_f = 0;
	if (set_dolar(mini) == -1)
		return ;
	if (mini->pipecount > 0)
		pipe_exec(mini, fd_2);
	else
	{
		single_exec(mini, fd_2);
		free(mini->inpwoutquotes);
		eerf(mini);
	}
	dup2(fd_2[0], 0);
	dup2(fd_2[1], 1);
	ft_wait(mini);
}
