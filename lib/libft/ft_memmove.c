/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak <ibkocak@student.42istanbul.co>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:27:55 by fhosgor           #+#    #+#             */
/*   Updated: 2024/07/19 11:50:34 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const char	*sorc;
	char		*dest;

	sorc = src;
	dest = dst;
	if (len == 0 || dst == src)
		return (dst);
	else if (dst < src)
		ft_memcpy(dest, src, len);
	else if (src < dst)
	{
		while (len > 0)
		{
			len--;
			dest[len] = sorc[len];
		}
	}
	return (dst);
}
