#include "minishell.h"

int *get_pipe_locs(char *tokenized, int pipecount)
{
	int *pipe_locs;
	int i;

	int x = 0;
	i = -1;
	pipe_locs = malloc(sizeof(int) * pipecount);
	while (tokenized[++i])
	{
		if (tokenized[i] == PIPE)
		{
			pipe_locs[x] = i;
			x++;
		}
	}
	return (pipe_locs);
}
// echo asd | grep -l
//echo asd | grep -l |
//+zzz+zzz+|+zzzz+zz | ajshbdahjsd

void split_cmd(char *input, char *tokenized)
{
	int i = -1;
	int j = 0;
	int x = -1;
	int pipecount = 0;
	int *pipe_locs;

	char **pipe_sub;
	pipe_sub = malloc(sizeof(char) * (pipecount + 2));
	pipe_sub[pipecount + 1] = "\0";


	while (tokenized[++i])
		if (tokenized[i] == PIPE)
			pipecount++;
	
	pipe_locs = get_pipe_locs(tokenized, pipecount);
	i = 0;
	if (pipecount > 0)
	{
		while (++x <= pipecount)
		{
			if (pipe_locs[j] == 0)
			{
				pipe_sub[j] = ft_substr(input, pipe_locs[j - 1], ft_strlen(input) + 10);
			}
			pipe_sub[j] = ft_substr(input, i, pipe_locs[j] - i);
			printf("%d\n", pipe_locs[j]);
			i = pipe_locs[j] + 1;
			pipe_sub[j] = ft_strtrim(pipe_sub[j], " ");
			j++;
		}
		i = -1;
		//while (pipe_sub[++i])
		//	mlt_cmd_exe(input);
	}
	else
		one_cmd_exe(input);
}	