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


void split_cmd(char *input, char *tokenized)
{
	int i = -1;
	int j = 0;
	int x = -1;
	int pipecount = 0;
	int *pipe_locs;

	char **pipe_sub;
	pipe_sub = malloc(sizeof(char) * (pipecount + 2));
	pipe_sub[pipecount + 1] = NULL;


	while (tokenized[++i])
		if (tokenized[i] == PIPE)
			pipecount++;
	
	pipe_locs = get_pipe_locs(tokenized, pipecount);
	i = 0;
	while (++x <= pipecount)
	{
		pipe_sub[j] = ft_substr(input, i, pipe_locs[j] - i);
		i = pipe_locs[j] + 1;
		pipe_sub[j] = ft_strtrim(pipe_sub[j], " ");
		j++;
	}
	i = 0;
	while (pipe_sub[i])
	{
		printf("%s\n", pipe_sub[i++]);
	}
}			