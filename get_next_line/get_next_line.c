/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:46:09 by csener            #+#    #+#             */
/*   Updated: 2026/08/10 16:44:10 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

static char	*read_loop(int fd, char *storage, char *buffer)
{
	char	*temp;
	ssize_t	bytes_read;

	bytes_read = 1;
	while (!ft_strchr(storage, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(storage);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		temp = storage;
		storage = ft_strjoin(temp, buffer);
		free(temp);
		if (!storage)
			return (NULL);
	}
	return (storage);
}

static char	*read_and_join(int fd, char *storage)
{
	char	*buffer;

	if (!storage)
		storage = ft_strdup("");
	if (!storage)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
	{
		free(storage);
		return (NULL);
	}
	storage = read_loop(fd, storage, buffer);
	free(buffer);
	return (storage);
}

static char	*extract_line(char *storage)
{
	char	*line;
	int		i;

	if (!storage || !storage[0])
		return (NULL);
	i = 0;
	while (storage[i] && storage[i] != '\n')
		i++;
	if (storage[i] == '\n')
		i++;
	line = ft_substr(storage, 0, i);
	return (line);
}

static char	*update_storage(char *storage)
{
	char	*new_storage;
	char	*nl_ptr;

	if (!storage)
		return (NULL);
	nl_ptr = ft_strchr(storage, '\n');
	if (!nl_ptr || *(nl_ptr + 1) == '\0')
	{
		free(storage);
		return (NULL);
	}
	new_storage = ft_substr(nl_ptr, 1, ft_strlen(nl_ptr + 1));
	free(storage);
	return (new_storage);
}

char	*get_next_line(int fd)
{
	static char	*storage;
	char		*line;

	if (fd < 0)
	{
		free(storage);
		storage = NULL;
		return (NULL);
	}
	if (BUFFER_SIZE <= 0)
		return (NULL);
	storage = read_and_join(fd, storage);
	if (!storage)
		return (NULL);
	line = extract_line(storage);
	if (!line)
	{
		free(storage);
		storage = NULL;
	}
	else
		storage = update_storage(storage);
	return (line);
}
