#include "minishell.h"

// void printpwd()
// {
// 	char *str;
// 	printf("Girdi\n");
// 	str =malloc(sizeof(char ) * 1024);
// 	getcwd(str, 1024);
// 	printf("%s\n", str);
// 	free(str);
// }

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

void empyt_inout_check(char *str)
{
	
}



void empyt_pipe_check(char *str)
{
	int	i;
	int count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == CHAR || str[i] == DOLLARINDBL || str[i] == DOLLARINSGL)
			++count;
		if (count == 0 && str[i] == PIPE)
			printf("Elemanın sol taraf boş\n");
		else if (count != 0 && str[i] == PIPE)
			count = 0;
		i++;
		if (!str[i] && count == 0)
			printf("Elemanın sağ taraf boş\n");
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
	if (!env)
		err_msg(2);
	take_env(NULL, env, 0, envr);
	while (1)
	{
		doublecount = 0;
		singlecount = 0;
		init_strc(&cmd, env);
		rcmd = readline("iboshell$> ");
		if(!rcmd)
		{
			free(env);
			printf("minishell: invalid input!\n"); //newline olayını sor
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
		// if (ft_strncmp("pwd", temp, 45) == 0)
		// 	printpwd();
		tokenize(temp);
	}		
}

int main(int ac, char **av, char **env)
{
	if (ac != 1)
		err_msg(1);
	start_cmd(env);

	// heredoc append için ayrı fonksiyon oluştur sağ sol boş mu kontrol et
	// input output için ayrı while luştur aynısını yap
}