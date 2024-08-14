/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:10:39 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/14 20:07:11 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define EMPTYPIPE "minishell: syntax error near unexpected token "
# define TOOMUCHARG "minishell: there should be only one argument!"
# define MALLOC "There isn't enough space!"

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include "lib/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

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
	pid_t			pid;
	char			**arg_flags;
	char			**meta_input;
	char			**output;
	char			**heredoc;
	char			**append;
	int				status;
	char			**env;
	//count		
	int				ac;
	int				ic;
	int				oc;
	int				hc;
	struct s_env	*main_env;
	struct s_main	*next;
}				t_main;

char	*tokenize(char *input);
int		pipe_in_quotes(t_main *mini, int sgc, int dbc, int tmp);
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
void	take_redirects(t_main *mini);
void	clean_unnecessary(t_main *mini);
void	count_pipes(t_main *mini, int i);
void	locate_pipes(t_main *mini, int i, int x);



void	run_heredoc(t_main *mini, int fd_2[2]);

void	printpwd(void);
void	cd(char *target_file);

#endif