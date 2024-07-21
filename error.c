#include "minishell.h"

void err_msg(int i)
{
	if (i == 1)
		ft_putendl_fd(TOOMUCHARG, 2);
	else if (i == 2)
		ft_putendl_fd(MALLOC, 2); 
	exit (1);
}