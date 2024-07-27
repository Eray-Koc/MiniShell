#include "minishell.h"

// echo asd | grep -l
//echo asd | grep -l |
//+zzz+zzz+|+zzzz+zz | ajshbdahjsd

void split_cmd(t_main *mini) // t_main *mini input-> mini->input / tokenized -> mini ->tokenized
{
	int i = -1;
	int j = 0;
	int x = -1;
	//int *pipe_locs;

	char **pipe_sub;
	pipe_sub = malloc(sizeof(char) * (mini->pipecount + 2));
	pipe_sub[mini->pipecount + 1] = "\0";
	
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
		//while (pipe_sub[++i])
		//	mlt_cmd_exe(input);
	}
	else
		one_cmd_exe(mini);
}	