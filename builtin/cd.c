#include "../minishell.h"

void cd(char *target_file)
{
	chdir(target_file);
}