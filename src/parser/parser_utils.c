/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:26:59 by csener            #+#    #+#             */
/*   Updated: 2025/06/20 16:35:19 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Prints the mandatory "Error\n" prefix followed by an explicit message.
** Always returns -1 so callers can do: return (err("..."));
*/
int	err(char *msg)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	return (-1);
}

int	check_extension(char *path)
{
	size_t	len;

	len = ft_strlen(path);
	if (len < 5)
		return (-1);
	if (ft_strncmp(path + len - 4, ".cub", 4) != 0)
		return (-1);
	return (0);
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_empty_line(char *line)
{
	int	i;

	if (!line)
		return (1);
	i = 0;
	while (line[i])
	{
		if (!is_space(line[i]))
			return (0);
		i++;
	}
	return (1);
}
