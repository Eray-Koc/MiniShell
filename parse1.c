#include "minishell.h"

// echo asd | grep -l
//echo asd | grep -l |
//+zzz+zzz+|+zzzz+zz | ajshbdahjsd


void pipe_exec(t_main *mini)
{
	int fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe error");
		exit(127);
	}
	mini->pid = fork();
	if (mini->pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		one_cmd_exe_2(mini);
		exit(127);
	}
	close(fd[1]);
	dup2(fd[0],0);
	close(fd[0]);
}

void read_and_exec(t_main *mini)
{
	t_main *temp;

	temp = mini;
	while (temp)
	{
		if (temp->next)
			pipe_exec(temp);
		else
			one_cmd_exe(temp);
		temp = temp->next;
	}
}


void split_cmd(t_main *mini) // t_main *mini input-> mini->input / tokenized -> mini ->tokenized
{
	int i = -1;
	int j = 0;
	int x = -1;

	char **pipe_sub;
	pipe_sub = malloc(sizeof(char) * (mini->pipecount + 2));
	pipe_sub[mini->pipecount + 1] = NULL;
	t_main *temp;
	
	i = 0;
	if (mini->pipecount > 0)
	{
		while (++x <= mini->pipecount)
		{
			if (mini->pipe_locs[j] == 0)
			{
				pipe_sub[j] = ft_substr(mini->input, mini->pipe_locs[j - 1], ft_strlen(mini->input) + 10);
			}
			pipe_sub[j] = ft_substr(mini->input, i, mini->pipe_locs[j] - i);
			i = mini->pipe_locs[j] + 1;
			pipe_sub[j] = ft_strtrim(pipe_sub[j], " ");
			j++;
		}
		i = -1;
		temp = mini;
		while (++i < mini->pipecount + 2)
		{
			mini->input = pipe_sub[i];
			if (i + 1 < mini->pipecount + 2)
			{
				mini->next = malloc(sizeof(t_main));
				mini->next->env = mini->env;
				mini->next->tokenized = mini->tokenized;
				mini = mini->next;
			}
			mini->next = NULL;
		}
		mini = temp;
		read_and_exec(mini);

	}
	else
		one_cmd_exe(mini);
}	