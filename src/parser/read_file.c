/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:26:59 by csener            #+#    #+#             */
/*   Updated: 2025/06/20 16:35:19 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	strip_newline(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

static int	append_line(char ***arr, int count, char *line)
{
	char	**new;
	int		i;

	new = malloc(sizeof(char *) * (count + 2));
	if (!new)
		return (-1);
	i = -1;
	while (++i < count)
		new[i] = (*arr)[i];
	new[count] = line;
	new[count + 1] = NULL;
	free(*arr);
	*arr = new;
	return (0);
}

/*
** Reads the whole file into a NULL-terminated array of lines (one malloc'd
** string per line, trailing '\n' removed). Empty lines are preserved so the
** element/map separation rules can be enforced later.
*/
char	**read_file(int fd)
{
	char	**lines;
	char	*line;
	int		count;

	lines = malloc(sizeof(char *));
	if (!lines)
		return (NULL);
	lines[0] = NULL;
	count = 0;
	line = get_next_line(fd);
	while (line)
	{
		strip_newline(line);
		if (append_line(&lines, count, line) != 0)
			return (free(line), free_split(lines), NULL);
		count++;
		line = get_next_line(fd);
	}
	return (lines);
}
