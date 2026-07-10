/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:30:22 by csener            #+#    #+#             */
/*   Updated: 2025/06/24 13:30:24 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*p;
	unsigned char		to_find;

	p = (const unsigned char *)s;
	to_find = (unsigned char)c;
	while (n > 0)
	{
		if (*p == to_find)
			return ((void *)p);
		p++;
		n--;
	}
	return (NULL);
}
