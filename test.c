#include <stdlib.h>

int ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (-1);
		i++;
	}
	return (0);
	
}

int main()
{
	ft_strcmp("akif", "akit");
	system("leaks a.out");
}
