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

void start_cmd(char **envr)
{
	char *rcmd;
	char *temp;
	t_main cmd;
	t_env *env;
	
	env = malloc(sizeof(t_env));
	take_env(NULL, env, 0, envr);
	while (1)
	{
		init_strc(&cmd, env);
		rcmd = readline("minishell$> ");
		if(!rcmd)
		{
			printf("exit\n");
			exit(1);
		}
		add_history(rcmd);
		temp = ft_strtrim(rcmd, "\t ");
		free(rcmd);
		
	}		
}

int main(int ac, char **av, char **env)
{
	if (ac != 1)
		err_msg();
	start_cmd(env);
}