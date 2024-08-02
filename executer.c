#include "minishell.h"




void remove_quotes(t_main *mini)
{
	int i = 0;
	int j = 0;
	mini->inpwoutquotes = malloc(sizeof(char) * ft_strlen(mini->input));
	while (mini->input[i])
	{
		if (mini->tokenized[i] == SINGLEQUOTE || mini->tokenized[i] == DOUBLEQUOTE)
		{
			i++;
			continue;
		}
		else
		{
			mini->inpwoutquotes[j] = mini->input[i];
			j++;
		}
		i++;
	}
}



char **get_path(t_main *mini)
{
    int i;
    
    i = 0;
    while (mini->env && mini->env[i])
    {
        if (ft_strnstr(mini->env[i], "PATH=", 5))
		{
            return (ft_split(ft_strchr(mini->env[i],'/'), ':'));
        }
        i++;
    }
    return (NULL);
}

char *get_cmd_path(t_main *cmd, char **command)
{
    char **path;
    char *temp;
    char *temp2;
    int i;

    i = 0;
    path = get_path(cmd);
    if (!path)
    {
        printf("PATH ERROR\n");
        exit(1);
    }
    while (path[i])
    {
        temp = ft_strjoin(path[i], "/");
        temp2 = ft_strjoin(temp, command[0]);
        if (!access(temp2, X_OK))
        {
            free(temp);
            return (temp2);
        }
        free(temp);
        free(temp2);
        i++;
    }
    printf("minishell: %s: command not found\n",command[0]);
    exit(127);
    return (NULL);
}



int check_redirects(char *tokenized)
{
	int	i;

	i = -1;
	while (tokenized[++i])
		if (tokenized[i] == HEREDOC || tokenized[i] == APPEND || tokenized[i] == INPUT || tokenized[i] == OUTPUT)
			return (1);
	return (0);
}

void one_cmd_exe(t_main *mini)
{
	char **splitted_input;
	char	*path;

	splitted_input = ft_split(mini->inpwoutquotes, ' ');
	if (check_redirects(mini->tokenized) == 1)
		return;
	else
	{
		if (splitted_input[0][0] == '/')
		{
			if (access(splitted_input[0], X_OK))
			{
				printf("minishell: %s: No such file or directory\n", splitted_input[0]);
            	exit(127);
			}
			else
				path = splitted_input[0];
		}
		else
			path = get_cmd_path(mini, splitted_input);
	}
	execve(path, splitted_input, mini->env);
}