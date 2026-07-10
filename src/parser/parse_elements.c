/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:26:59 by csener            #+#    #+#             */
/*   Updated: 2025/06/20 16:35:19 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*skip_spaces(char *s)
{
	while (*s && is_space(*s))
		s++;
	return (s);
}

int	set_texture(char **dst, char *rest)
{
	char	*path;

	if (*dst)
		return (err("duplicate texture identifier"));
	path = ft_strtrim(rest, " \t");
	if (!path)
		return (-1);
	if (!path[0] || ft_strchr(path, ' ') || ft_strchr(path, '\t'))
		return (free(path), err("invalid texture path"));
	*dst = path;
	return (1);
}

int	set_color(int *rgb, int *has, char *rest)
{
	if (*has)
		return (err("duplicate color identifier"));
	if (parse_rgb(rest, rgb) != 0)
		return (err("invalid color value"));
	*has = 1;
	return (1);
}

/*
** Returns 1 if the line was a valid element, 0 if the line is not an element
** (so the map begins here), or -1 on a malformed/duplicated element.
*/
int	parse_element_line(t_config *cfg, char *line)
{
	char	*p;

	p = skip_spaces(line);
	if (!ft_strncmp(p, "NO", 2) && is_space(p[2]))
		return (set_texture(&cfg->no_path, p + 2));
	if (!ft_strncmp(p, "SO", 2) && is_space(p[2]))
		return (set_texture(&cfg->so_path, p + 2));
	if (!ft_strncmp(p, "WE", 2) && is_space(p[2]))
		return (set_texture(&cfg->we_path, p + 2));
	if (!ft_strncmp(p, "EA", 2) && is_space(p[2]))
		return (set_texture(&cfg->ea_path, p + 2));
	if (p[0] == 'F' && is_space(p[1]))
		return (set_color(cfg->floor, &cfg->has_floor, p + 1));
	if (p[0] == 'C' && is_space(p[1]))
		return (set_color(cfg->ceiling, &cfg->has_ceil, p + 1));
	return (0);
}

int	check_textures(t_config *cfg)
{
	char	*paths[4];
	int		i;
	int		fd;

	paths[0] = cfg->no_path;
	paths[1] = cfg->so_path;
	paths[2] = cfg->we_path;
	paths[3] = cfg->ea_path;
	i = -1;
	while (++i < 4)
	{
		fd = open(paths[i], O_RDONLY);
		if (fd < 0)
			return (err("cannot open a texture file"));
		close(fd);
	}
	return (0);
}
