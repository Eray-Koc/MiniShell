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
	CHAR = 'Z', //5
	DOLLARINDBL = 6,
	DOLLARINSGL = 7,
	BLANK = '+' // 4
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
	char **arg_flags;
	char **input; // <
	char **output; // >
	char **heredoc; // <<
	char **append; // >>
	int status;
	char **env;
	struct s_env *main_env;
	struct s_main *next;
}				t_main;



void tokenize(char *str);
int pipe_in_quotes(char *str);
void isquote_closed(char *str, int i, int *dbc, int *sgc);
void empyt_pipe_check(char *str);
void err_msg(int i);
void empty_inout_check(char *str);


void printpwd();
void cd(char *target_file);

#endif