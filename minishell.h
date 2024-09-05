/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:10:39 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/05 19:36:30 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define EMPTYPIPE "minishell: syntax error near unexpected token "
# define TOOMUCHARG "minishell: there should be only one argument!"
# define MALLOC "There isn't enough space!"
# define ENV_LIMIT			1000

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include "lib/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

extern int	g_global_exit;

typedef enum e_status
{
	NONE = 3,
	PIPE = '|',
	APPEND = 1,
	HEREDOC = 2,
	INPUT = '<',
	OUTPUT = '>',
	BUILTIN,
	DOUBLEQUOTE = '"',
	SINGLEQUOTE = '\'',
	CHAR = 5,
	DOLLARINDBL = 6,
	DOLLARINSGL = 7,
	BLANK = 4
}			t_status;			

typedef struct s_garbage
{
	void				*data;
	struct s_garbage	*next;
}				t_garbage;

typedef struct s_env
{
	char	**env;
}			t_env;

typedef struct s_main
{
	char			*cmd;
	char			*rcmd;
	char			*input;
	char			*tokenized;
	char			*token2;
	int				pipecount;
	int				*pipe_locs;
	char			*inpwoutquotes;
	char			**pipe_sub;
	pid_t			pid;
	char			**arg_flags;
	char			**meta_input;
	char			**output;
	char			**heredoc;
	char			**append;
	int				status;
	char			**env;
	char			**args;
	//count		
	int				ac;
	int				ic;
	int				oc;
	int				hc;
	struct s_env	*main_env;
	struct s_main	*next;
}				t_main;

char	*tokenize(char *input);
void	isquote_closed(char *str, int i, int *dbc, int *sgc);
void	empyt_pipe_check(t_main *mini);
void	err_msg(int i);
void	empty_inout_check(t_main *mini, int count, int i);
void	split_cmd(t_main *mini);
void	one_cmd_exe(t_main *mini);
void	tab_to_space(char *str);
void	take_env(t_main *mini);
int		check_redirects(char *tokenized);
char	*remove_quotes(t_main *mini);
void	remove_quotes_from_append(t_main *mini, int i, int j, int x);
void	remove_quotes_from_meta_input(t_main *mini, int i, int j, int x);
void	remove_quotes_from_heredoc(t_main *mini, int i, int j, int x);
void	remove_quotes_from_output(t_main *mini, int i, int j, int x);
void	set_zero(t_main *mini);
void	open_files(t_main *mini);
void	take_redirects(t_main *mini, int i);
void	count_pipes(t_main *mini, int i);
void	locate_pipes(t_main *mini, int i, int x);
void	clean_unnecessary(t_main *mini, int flag, int i);
void	run_heredoc(t_main *mini, int fd_2[2]);
int		check_if_same(char *s1, char *s2);
void	printpwd(void);
void	cd(char *target_file);
void	clear_struct(t_main *mini, int *doublecount, int *singlecount);
void	controls(t_main *mini);
void	rcmd_exception(t_env *env);
void	quote_exception(t_main *mini);
void	set_zero(t_main *mini);
void	tag_chars_betw_quotes(char *tokenized, int flag, int i);
char	*remove_quotes(t_main *mini);
void	tag_all(int i, char *tokenized);
void	clean_file_names(t_main *mini, int i);
char	*get_cmd_path(t_main *cmd, char **command, int i);
void	count_redirects(t_main *mini);
void	if_append(t_main *mini, int index, int temp, int j);
void	if_heredoc(t_main *mini, int index, int temp, int j);
void	if_output(t_main *mini, int index, int temp, int j);
void	if_input(t_main *mini, int index, int temp, int j);
int		if_double(char *tokenized, int i);
int		if_single(char *tokenized, int i);
int		check_char(char c);
void	remove_quotes_foreach(t_main *mini);
void	pipe_exec(t_main *mini);
void	read_and_exec(t_main *mini);
int		run_builtin(t_main *mini, char *input);
void	one_cmd_exe_2(t_main *mini, int i);
int		check_builtin(t_main *mini);
int		check_builtin_for_pipe(char *str);
void	ft_cd(char **arg);
void	ft_echo(char **arg);
void	ft_exit(char **arg);
void	ft_export(t_main *mini, char **arg, int i, char *identifier);
int		check_env(t_main *mini, const char *identifier, const char *arg);
int		add_new_env(t_main *mini, const char *arg, int j);
void	ft_env(char **env, int status);
size_t	ft_strarrlen(char **arr);
void	ft_free_str_arr(char **str_arr);
char	*get_env(char **env, char *var);

#endif