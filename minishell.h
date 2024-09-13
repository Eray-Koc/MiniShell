/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:10:39 by erkoc             #+#    #+#             */
/*   Updated: 2024/09/13 21:10:35 by erkoc            ###   ########.fr       */
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
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include "lib/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

int	g_global_exit;

#define CHILD_P 1
#define MAIN_P 2
#define HEREDOC_P 3

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
	DOLLARINDBL = '$',
	DOLLARINSGL = '$',
	DOLLAR = '$',
	BLANK = 4
}			t_status;			

typedef struct s_env
{
	char	**env;
}			t_env;


typedef struct s_main
{
	char			*cmd;
	char			*input;
	char			*tokenized;
	int				pipecount;
	int				*pipe_locs;
	char			*inpwoutquotes;
	char			**pipe_sub;
	pid_t			*pid;
	pid_t			hpid;
	char			**meta_input;
	char			**output;
	char			**heredoc;
	char			**append;
	int				status;
	char			**env;
	char			**args; // yeni
	int				status1;	//yeni
	//count	
	int				pid_i;
	int				ac;
	int				ic;
	int				oc;
	int				hc;

	int ifd;
	int ofd;
	int oerr;
	int ierr;


	int flagappend;
	int flagoutput;
	int flaginput;
	int flagheredoc;
}				t_main;




void	ft_unset(t_main *mini, char **argv);
void	remove_env(t_main *mini, const char *arg);
char	*get_env(char **env, char *var);
void	ft_cd(char **arg);
void	ft_echo(char **arg);
size_t	counter_argv(char *str);
void	ft_exit(char **arg);
int		ft_strequ(const char *s1, const char *s2);
void	ft_export(t_main *mini, char **arg, int i, char *identifier);
int		valid_identifier(const char *str);
void	ft_env(char **env, int status);
char	*tokenize(char *input);
void	isquote_closed(char *str, int i, int *dbc, int *sgc);
int		empyt_pipe_check(t_main *mini);
void	err_msg(int i);
int	empty_inout_check(char *input, char *tokenized);
void	split_cmd(t_main *mini);
void	one_cmd_exe(t_main *mini);
void	tab_to_space(char *str);
void	take_env(t_main *mini);
int		check_redirects(char *tokenized);
char	*remove_quotes(t_main *mini);
void	remove_quotes_from_append(t_main *mini, int i, int j, int x);
void	remove_quotes_from_meta_input(t_main *mini, int i, int j, int x);
void	remove_quotes_from_heredoc(t_main *mini, int i, int j, int x);
void	remove_quotes_from_output(t_main *mini, int i);
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
int		controls(t_main *mini);
void	rcmd_exception(t_env *env);
void	quote_exception(t_main *mini);
void	set_zero(t_main *mini);
void	tag_chars_betw_quotes(char *tokenized, int flag, int i);
char	*remove_quotes(t_main *mini);
void	tag_all(int i, char *tokenized);
void	clean_file_names(t_main *mini, int i);
char	*get_cmd_path(t_main *cmd, char **command, int i);
void	count_redirects(t_main *mini, char *tokenized);
void	if_append(t_main *mini, int index, int temp, int j);
void	if_heredoc(t_main *mini, int index, int temp, int j);
void	if_output(t_main *mini, int index, int temp, int j);
void	if_input(t_main *mini, int index, int temp, int j);
int		if_double(char *tokenized, int i);
void	if_dollar(t_main *mini, int index);
int		if_single(char *tokenized, int i);
int		check_char(char c);
void	remove_quotes_foreach(t_main *mini);
void	pipe_exec(t_main *mini);
void	read_and_exec(t_main *mini, int fd_2[2]);

void	one_cmd_exe_2(t_main *mini, int i);
void	take_redirects_2(t_main *mini, char *tokenized, int j);
void	open_files_2(t_main *mini, char *tokenized);
void	clean_unnecessary_2(t_main *mini, int flag, int i, char *tokenized, int p);
void	clean_file_names_2(t_main *mini, int i, int p, char *tokenized);
char 	*remove_quotes_2(char *input, char *tokenized);

size_t	ft_strarrlen(char **arr);
void	ft_free_str_arr(char **str_arr);
int		check_env(t_main *mini, const char *identifier, const char *arg);
int		add_new_env(t_main *mini, const char *arg, int j);
int		check_builtin_2(char *pipe);
int		run_builtin(t_main *mini, char *input);
void	free_double_pointer(char **str);
void 	to_be_freed2(t_main *mini);
int		empty_inout_check(char *input, char *tokenized);
int		find_env_index(char **env, char *var);
void	check_dollar(t_main *mini, int i, int temp);
int		set_dolar(t_main *mini);
void	set_signal(int c);
void	handler_sigint(int sig);
int		runcommanderror(char **command, int i);
int		is_file(const char *path);
void	error_write(char *command);
int		is_directory(const char *path);
#endif