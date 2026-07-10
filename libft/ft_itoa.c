/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:30:15 by csener            #+#    #+#             */
/*   Updated: 2025/06/24 13:30:17 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	get_count(long int n)
{
	int	count;

	if (n == 0)
		return (1);
	if (n < 0)
		n = -n;
	count = 0;
	while (n > 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static void	fill_str(char *str, long num, int len)
{
	str[len] = '\0';
	if (num == 0)
	{
		str[0] = '0';
		return ;
	}
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	while (num > 0)
	{
		str[len - 1] = (num % 10) + '0';
		num /= 10;
		len--;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	num;

	num = n;
	len = get_count(num);
	if (num < 0)
		len++;
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	fill_str(str, num, len);
	return (str);
}
