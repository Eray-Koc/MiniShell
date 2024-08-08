#include "../minishell.h"




void open_files(t_main *mini)
{
	int	i;
	int ifd;
	int ofd;

	i = 0;
	set_zero(mini);
	ifd = 0;
	ofd = 0;
	while(mini->tokenized[i])
	{
		if (mini->tokenized[i] == OUTPUT)
		{
			ofd = open(mini->output[mini->oc], O_CREAT | O_TRUNC | O_WRONLY, 0644);
			mini->oc++;
			if (ofd == -1)
			{
				printf("dosya açamadı\n");
				exit(1);
			}
			if (mini->output[mini->oc])
				close (ofd);
			else
				dup2(ofd, 1);
		}
		else if (mini->tokenized[i] == APPEND)
		{
			i++;
			ofd = open(mini->append[mini->ac], O_CREAT | O_APPEND | O_WRONLY, 0644);
			mini->ac++;
			if (ofd == -1)
			{
				printf("dosya açamadı\n");
				exit(1);
			}
			if (mini->append[mini->ac])
				close (ofd);
			else
				dup2(ofd, 1);
		}
		else if (mini->tokenized[i] == INPUT)
		{
			ifd = open(mini->meta_input[mini->ic], O_RDONLY, 0644);
			mini->ic++;
			if (ifd == -1)
			{
				printf("dosya açamadı\n");
				exit(1);
			}
			if (mini->meta_input[mini->ic])
				close (ifd);
			else
				dup2(ifd, 0);
		}
		i++;
	}

}