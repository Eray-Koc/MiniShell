#include "../minishell.h"




void open_files(t_main *mini)
{
	int	i;
	int fd;

	i = 0;
	set_zero(mini);
	while(mini->input[i])
	{
		if (mini->input[i] == OUTPUT)
		{
			fd = open(mini->output[mini->oc], O_CREAT | O_TRUNC | O_WRONLY, 0644);
			mini->oc++;
			if (fd == -1)
			{
				printf("dosya açamadı\n");
				exit(1);
			}
			if (mini->output[mini->oc])
				close (fd);
			else
				dup2(fd, 1);
		}
		i++;
	}

}