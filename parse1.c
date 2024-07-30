#include "minishell.h"

// echo asd | grep -l
//echo asd | grep -l |
//+zzz+zzz+|+zzzz+zz | ajshbdahjsd



void split_cmd(t_main *mini) // t_main *mini input-> mini->input / tokenized -> mini ->tokenized
{
	int i = -1;
	int j = 0;
	int x = -1;

	char **pipe_sub;
	pipe_sub = malloc(sizeof(char) * (mini->pipecount + 2));
	pipe_sub[mini->pipecount + 1] = "\0";
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
		while (pipe_sub[++i])
		{
			mini->input = pipe_sub[i];
			mini = mini->next;
			mini = malloc(sizeof(mini));
			mini->next = NULL;
		}
		mini = temp;

	}
	else
		one_cmd_exe(mini);
}	