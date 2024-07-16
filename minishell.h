#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "lib/libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>


# define NONE 0
# define PIPE 1
# define PIPEAPPEND 2
# define PIPEHEREDOC 3
# define PIPEHEREDOCAPPEND 4
# define APPEND 5
# define BUILTIN 6

typedef struct s_utils
{
	int a;
	int b;
	int c;
	int d;
}			t_utils;

typedef struct s_env
{
	char **env;
}			t_env;

typedef struct s_main
{
	char *cmd;
	char *arg_flags;
	char **input;
	char **output;
	char **heredoc;
	char **append;
	int status;
	char **env;
	struct s_env *main_env;
	struct s_main *next;
}				t_main;



#endif