#include "minishell.h"


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

void empty_inout_check(t_main *mini)
{
	int i = 0;
	int count = 0;
	while (mini->tokenized[i])
	{
		if (mini->tokenized[i] != HEREDOC && mini->tokenized[i] != INPUT && mini->tokenized[i] != OUTPUT && mini->tokenized[i] != APPEND && mini->tokenized[i] != BLANK)
			count++;
		if ((mini->tokenized[i] == HEREDOC || mini->tokenized[i] == INPUT || mini->tokenized[i] == OUTPUT || mini->tokenized[i] == APPEND) && count == 0)
			printf("Sol taraf boş\n");
		if (mini->tokenized[i] == HEREDOC || mini->tokenized[i] == APPEND)
		{
			count = 0;
			i = i + 2;
		}
		else if (mini->tokenized[i] == INPUT || mini->tokenized[i] == OUTPUT)
		{
			count = 0;
			i++;
		}
		if(!mini->tokenized[i] && count == 0)
			printf("sağ boş\n");
		i++;
	}

	
}

//echo asd | grep "|"
//zzzz+zzz+|+zzzz+"z"

void empyt_pipe_check(t_main *mini)
{
	int	i;
	int count;

	count = 0;
	i = 0;
	while (mini->tokenized[i])
	{
		if (mini->tokenized[i] == CHAR || mini->tokenized[i] == DOLLARINDBL || mini->tokenized[i] == DOLLARINSGL)
			++count;
		if (count == 0 && mini->tokenized[i] == PIPE)
			printf("Elemanın sol taraf boş\n");
		else if (count != 0 && mini->tokenized[i] == PIPE)
			count = 0;
		i++;
		if (!mini->tokenized[i] && count == 0)
			printf("Elemanın sağ taraf boş\n");
	}
}

void start_cmd()
{
	char *rcmd;
	t_main cmd;
	t_env *env;
	int	doublecount;
	int singlecount;

	env = malloc(sizeof(t_env));
	if (!env)
		err_msg(2);
	take_env(&cmd);
	while (1)
	{
		doublecount = 0;
		singlecount = 0;
		//init_strc(&cmd, env);
		rcmd = readline("iboshell$> ");
		if(!rcmd)
		{
			free(env);
			printf("minishell: invalid input!\n"); //newline olayını sor
			exit(1);
		}
		add_history(rcmd);
		cmd.input = ft_strtrim(rcmd, "\t ");
		tab_to_space(cmd.input);
		free(rcmd);
		isquote_closed(cmd.input, -1, &doublecount, &singlecount);
		if (doublecount % 2 != 0 || singlecount % 2 != 0)
		{
			printf("Dquote!\n");
			free(cmd.input);
			continue;
		}
		tokenize(&cmd);
		split_cmd(&cmd);
		free(cmd.input);
	}		
}

void tab_to_space(char *str)
{
	int	i;

	i = -1;
	while(str[++i])
	{
		if(str[i] == '\t')
			str[i] = 32;
	}


}
int main(int ac, char **av)
{
	(void)av;
	if (ac != 1)
		err_msg(1);
	start_cmd();
}