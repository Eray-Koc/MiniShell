/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 18:08:01 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/11 18:09:51 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_quotes_from_append(t_main *mini, int i, int j, int x)
{
	char	*tokenized;
	char	*ret;

	if (!mini->append[i])
		return ;
	while (mini->append[i])
	{
		ret = malloc(sizeof(char) * ft_strlen(mini->append[i]));
		tokenized = ft_substr(mini->append[i], 0, ft_strlen(mini->append[i]));
		tokenized = tokenize(mini->append[i]);
		x = 0;
		j = 0;
		while (mini->append[i][j])
		{
			if (tokenized[j] == DOUBLEQUOTE || tokenized[j] == SINGLEQUOTE)
			{
				j++;
				continue ;
			}
			else
			{
				ret[x++] = mini->append[i][j];
			}
			j++;
		}
		ft_strlcpy(mini->append[i], ret, ft_strlen(mini->append[i]) + 1);
		free (tokenized);
		free (ret);
		i++;
	}
}

void	remove_quotes_from_meta_input(t_main *mini, int i, int j, int x)
{
	char	*tokenized;
	char	*ret;

	if (!mini->meta_input[i])
		return ;
	while (mini->meta_input[i])
	{
		ret = malloc(sizeof(char) * ft_strlen(mini->meta_input[i]));
		tokenized = ft_substr(mini->meta_input[i], 0, ft_strlen(mini->meta_input[i]));
		tokenized = tokenize(mini->meta_input[i]);
		x = 0;
		j = 0;
		while (mini->meta_input[i][j])
		{
			if (tokenized[j] == DOUBLEQUOTE || tokenized[j] == SINGLEQUOTE)
			{
				j++;
				continue ;
			}
			else
			{
				ret[x++] = mini->meta_input[i][j];
			}
			j++;
		}
		ft_strlcpy(mini->meta_input[i], ret,
			ft_strlen(mini->meta_input[i]) + 1);
		free (tokenized);
		free (ret);
		i++;
	}
}

void	remove_quotes_from_heredoc(t_main *mini, int i, int j, int x)
{
	char	*tokenized;
	char	*ret;

	if (!mini->heredoc[i])
		return ;
	while (mini->heredoc[i])
	{
		ret = malloc(sizeof(char) * ft_strlen(mini->heredoc[i]));
		tokenized = ft_substr(mini->heredoc[i], 0, ft_strlen(mini->heredoc[i]));
		tokenized = tokenize(mini->heredoc[i]);
		x = 0;
		j = 0;
		while (mini->heredoc[i][j])
		{
			if (tokenized[j] == DOUBLEQUOTE || tokenized[j] == SINGLEQUOTE)
			{
				j++;
				continue ;
			}
			else
			{
				ret[x++] = mini->heredoc[i][j];
			}
			j++;
		}
		ft_strlcpy(mini->heredoc[i], ret, ft_strlen(mini->heredoc[i]) + 1);
		free (tokenized);
		free (ret);
		i++;
	}
}

void	remove_quotes_from_output(t_main *mini, int i, int j, int x)
{
	char	*tokenized;
	char	*ret;

	if (!mini->output[i])
		return ;
	while (mini->output[i])
	{
		ret = malloc(sizeof(char) * ft_strlen(mini->output[i]));
		tokenized = ft_substr(mini->output[i], 0, ft_strlen(mini->output[i]));
		tokenized = tokenize(mini->output[i]);
		j = 0;
		x = 0;
		while (mini->output[i][j])
		{
			if (tokenized[j] == DOUBLEQUOTE || tokenized[j] == SINGLEQUOTE)
			{
				j++;
				continue ;
			}
			else
			{
				ret[x++] = mini->output[i][j];
			}
			j++;
		}
		ft_strlcpy(mini->output[i], ret, ft_strlen(mini->output[i]) + 1);
		free (tokenized);
		free (ret);
		i++;
	}
}

void	tab_to_space(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\t')
			str[i] = 32;
	}
}
