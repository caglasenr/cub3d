/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:57:34 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/10 16:46:36 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	gc_free_all_and_exit(t_gc **gc, char *err_msg, int exit_code)
{
	t_gc	*current;
	t_gc	*next_node;

	get_next_line(-1);
	if (gc && *gc)
	{
		current = *gc;
		while (current)
		{
			next_node = current->next;
			free(current->ptr);
			free(current);
			current = next_node;
		}
		*gc = NULL;
	}
	if (err_msg)
	{
		ft_putstr_fd("Error\n", 2);
		ft_putstr_fd(err_msg, 2);
		ft_putstr_fd("\n", 2);
	}
	exit(exit_code);
}

void	*gc_malloc(t_gc **gc, size_t size)
{
	t_gc	*new_node;
	void	*new_ptr;

	new_ptr = malloc(size);
	if (!new_ptr)
		gc_free_all_and_exit(gc, "malloc failed", 1);
	new_node = malloc(sizeof(t_gc));
	if (!new_node)
	{
		free(new_ptr);
		gc_free_all_and_exit(gc, "malloc failed", 1);
	}
	new_node->ptr = new_ptr;
	new_node->next = *gc;
	*gc = new_node;
	return (new_ptr);
}

char	*gc_strdup(t_gc **gc, const char *s1)
{
	size_t	len;
	char	*copy;

	len = ft_strlen(s1);
	copy = gc_malloc(gc, len + 1);
	ft_strlcpy(copy, s1, len + 1);
	return (copy);
}

void	check_textures(t_config *cfg, t_gc **gc)
{
	char	*paths[5];
	size_t	len;
	int		i;
	int		fd;

	paths[0] = cfg->no_path;
	paths[1] = cfg->so_path;
	paths[2] = cfg->we_path;
	paths[3] = cfg->ea_path;
	paths[4] = cfg->door_path;
	i = -1;
	while (++i < 5)
	{
		if (!paths[i])
			continue ;
		len = ft_strlen(paths[i]);
		if (len < 5 || ft_strncmp(paths[i] + len - 4, ".xpm", 4) != 0)
			gc_free_all_and_exit(gc, "invalid texture extension", 1);
		fd = open(paths[i], O_RDONLY);
		if (fd < 0)
			gc_free_all_and_exit(gc, "cannot open a texture file", 1);
		close(fd);
	}
}
