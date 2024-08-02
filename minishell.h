#ifndef MINISHELL_H
#define MINISHELL_H

# define EMPTYPIPE "minishell: syntax error near unexpected token "
# define TOOMUCHARG "minishell: there should be only one argument!"
#define MALLOC "There isn't enough space!"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "lib/libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>


typedef enum e_status{
	NONE = 0,
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

typedef struct s_garbage{
	void *data;
	struct s_garbage *next;
}				t_garbage;


typedef struct s_env
{
	char **env;
}			t_env;

typedef struct s_main
{

	char *cmd;
	char *rcmd;
	char *input;
	char *tokenized;
	int pipecount;
	int *pipe_locs;
	char *inpwoutquotes;
	pid_t pid;
	char **arg_flags;
	char **meta_input;
	char **output;
	char **heredoc; 
	char **append;
	int status;
	char **env;
	//count
	int ac;
	int ic;
	int oc;
	int hc;
	struct s_env *main_env;
	struct s_main *next;
}				t_main;



char *tokenize(t_main *mini);
int pipe_in_quotes(t_main *mini);
void isquote_closed(char *str, int i, int *dbc, int *sgc);
void empyt_pipe_check(t_main *mini);
void err_msg(int i);
void empty_inout_check(t_main *mini);
void split_cmd(t_main *mini);
void one_cmd_exe(t_main *mini);
void tab_to_space(char *str);
void take_env(t_main    *mini);
int check_redirects(char *tokenized);
void remove_quotes(t_main *mini);


void printpwd();
void cd(char *target_file);

#endif