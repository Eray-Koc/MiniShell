#include "minishell.h"

void err_msg()
{
	ft_putendl_fd("Too many arg", 2);
	exit (1);
}

int	take_env_size(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (i);
	while (env[i])
		i++;
	return (i);
}

void	take_env(t_main *cmds, t_env *env, int status, char **environ)
{
	int	i;

	i = -1;
	if (status == 0)
	{
		env->env = malloc(sizeof(char *) * (take_env_size(environ) + 1));
		if (!env->env)
			perror("Malloc!\n");
		while (++i < take_env_size(env->env))
			env->env[i] = ft_strdup(environ[i]);
		env->env[i] = NULL;
	}
	else
	{
		cmds->env = malloc(sizeof(char *) * (take_env_size(env->env) + 1));
		if (!cmds->env)
			perror("Malloc!\n");
		while (++i < take_env_size(env->env))
			cmds->env[i] = ft_strdup(env->env[i]);
		cmds->env[i] = NULL;
	}
}

void init_strc(t_main *cmd, t_env *env)
{
	cmd = malloc(sizeof(t_main));
	cmd->main_env = env;
	take_env(cmd, env, 1, NULL);
	cmd->status = 0;
	cmd->next = NULL;
}

void isquote_closed(char *str, int i, int *dbc, int *sgc)
{
	while (str[++i])
	{
		if (str[i] == '\'' && *dbc % 2 == 0)
			*sgc += 1;
		else if (str[i] == '\"' && *sgc % 2 == 0)
			*dbc += 1;
	}
}

void check_args(char *str)
{
	char **s;
	int	i;

	i = 0;
	s = ft_split(str, ' ');
	while (str[i])
	{
		if (str[i] == '|')
		{

		}
	}
}



int pipe_in_quotes(char *str)
{
	int	i;
	int pipecount;
	int *pipe_locs;
	int	x;
	int sgc;
	int dbc;
	int tmp;

	tmp = 0;
	sgc = 0;
	dbc = 0;
	x = 0;
	pipecount = 0;
	i = -1;
	while (str[++i])
		if (str[i] == '|')
			pipecount++;
	pipe_locs = malloc(sizeof(int) * pipecount);
	i = -1;
	while (str[++i])
	{
		if (str[i] == '|')
		{
			pipe_locs[x] = i;
			x++;
		}
	}
	i = -1;
	while (++i < pipecount)
	{
		x = pipe_locs[i];
		tmp = pipe_locs[i];
		while (0 <= x)
		{
			if (str[x] == '\'')
				sgc++;
			else if (str[x] == '\"')
				dbc++;
			x--;
		}
		while (str[tmp])
		{
			if (str[x] == '\'')
				sgc++;
			else if (str[x] == '\"')
				dbc++;
			tmp++;
		}
	}
	if (dbc % 2 == 0 && sgc == 0)
		return (1);
	else
		return (0);
	
}

void tag_chrs(char *str)
{
	int	i;
	char *cpy;

	i = 0;
	i = ft_strlen(str);
	cpy = malloc(sizeof(char) * i);
	if (!cpy)
		printf("hata"); /// BYRAYI DUZELTTTTTTTTTTTTTTTTTTT
	ft_strlcpy(cpy, str, i);
	i = 0;
	while (cpy[i])
	{
		if (cpy[i] == '\'')
			cpy[i] = SINGLEQUOTE;
		else if (cpy[i] == '\"')
			cpy[i] = DOUBLEQUOTE;
		else if (cpy[i] == '>' && cpy[i + 1] == '>')
			cpy[i] = APPEND;
		else if (cpy[i] == '>')
			cpy[i] = OUTPUT;
		else if (cpy[i] == '<' && cpy[i + 1] == '<')
			cpy[i] = HEREDOC;
		else if (cpy[i] == '<')
			cpy[i] = INPUT;
		else if (cpy[i] == '|')
			cpy[i] = PIPE;
		else
			cpy[i] = CHAR;
	}
}

void start_cmd(char **envr)
{
	char *rcmd;
	char *temp;
	t_main cmd;
	t_env *env;
	int	doublecount;
	int singlecount;

	env = malloc(sizeof(t_env));
	take_env(NULL, env, 0, envr);
	while (1)
	{
		doublecount = 0;
		singlecount = 0;
		init_strc(&cmd, env);
		rcmd = readline("minishell$> ");
		if(!rcmd)
		{
			free(env);
			printf("exit\n");
			exit(1);
		}
		add_history(rcmd);
		temp = ft_strtrim(rcmd, "\t ");
		free(rcmd);
		isquote_closed(temp, -1, &doublecount, &singlecount); 
		if (doublecount % 2 != 0 || singlecount % 2 != 0)
		{
			printf("Dquote!\n");
			free(temp);
			continue;
		}
		if (!pipe_in_quotes(temp))
			printf("Tırnak içide pipe var\n");
		else
			printf("Tırnak içinde pipe yok\n");
		
		
	}		
}

int main(int ac, char **av, char **env)
{
	if (ac != 1)
		err_msg();
	start_cmd(env);
}