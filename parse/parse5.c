/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:17:30 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/17 17:16:26 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	tag_chars_betw_quotes(char *tokenized, int flag, int i)
{
	while (tokenized[++i])
	{
		flag = 0;
		if (tokenized[i] == SINGLEQUOTE)
			flag = 1;
		else if (tokenized[i] == DOUBLEQUOTE)
			flag = 2;
		if (flag == 1)
			i = if_single(tokenized, i);
		else if (flag == 2)
			i = if_double(tokenized, i);
	}
}

char	*remove_quotes(t_main *mini)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	ret = malloc(sizeof(char) * ft_strlen(mini->input));
	while (mini->input[i])
	{
		if (mini->tokenized[i] == SINGLEQUOTE
			|| mini->tokenized[i] == DOUBLEQUOTE)
		{
			i++;
			continue ;
		}
		else
		{
			ret[j] = mini->input[i];
			j++;
		}
		i++;
	}
	return (ret);
}
