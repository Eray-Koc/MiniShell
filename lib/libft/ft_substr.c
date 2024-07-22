/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:28:30 by fhosgor           #+#    #+#             */
/*   Updated: 2024/07/22 19:51:19 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*a;
	size_t	j;

	j = ft_strlen(s);
	if (!s)
		return (NULL);
	if (start >= j)
	{
		a = malloc(1);
		if (!a)
			return (NULL);
		a[0] = '\0';
	}
	else
	{
		if (len > j - start)
			len = j - start;
		a = malloc(len + 1);
		if (!a)
			return (NULL);
		ft_strlcpy(a, s + start, len + 1);
	}
	return (a);
}
