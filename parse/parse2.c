#include "../minishell.h"

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