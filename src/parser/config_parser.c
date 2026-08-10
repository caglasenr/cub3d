/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:58:21 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/10 15:44:58 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	save_texture(char **target, char *line, int i, t_gc **gc)
{
	char	*path;
	int		len;

	if (*target != NULL)
		gc_free_all_and_exit(gc, "same texture defined more than once", 1);
	i = skip_whitespaces(line, i);
	path = gc_strdup(gc, line + i);
	len = ft_strlen(path);
	if (len > 0 && path[len - 1] == '\n')
		path[len - 1] = '\0';
	*target = path;
}

void	parse_color(int *target, char *line, int i, t_gc **gc)
{
	char	**rgb;
	int		r;
	int		g;
	int		b;
	int		j;

	i = skip_whitespaces(line, i);
	rgb = ft_split(line + i, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3] != NULL)
		gc_free_all_and_exit(gc, "invalid color format, expected R,G,B", 1);
	if (!is_all_digits(rgb[0]) || !is_all_digits(rgb[1])
		|| !is_all_digits(rgb[2]))
		gc_free_all_and_exit(gc, "colors must only contain numbers", 1);
	r = ft_atoi(rgb[0]);
	g = ft_atoi(rgb[1]);
	b = ft_atoi(rgb[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		gc_free_all_and_exit(gc, "color values must be between 0 and 255", 1);
	target[0] = r;
	target[1] = g;
	target[2] = b;
	j = 0;
	while (rgb[j])
		free(rgb[j++]);
	free(rgb);
}

static bool	parse_texture_config(char *line, int i, t_config *cfg, t_gc **gc)
{
	if (ft_strncmp(line + i, "NO ", 3) == 0)
		save_texture(&(cfg->no_path), line, i + 3, gc);
	else if (ft_strncmp(line + i, "SO ", 3) == 0)
		save_texture(&(cfg->so_path), line, i + 3, gc);
	else if (ft_strncmp(line + i, "WE ", 3) == 0)
		save_texture(&(cfg->we_path), line, i + 3, gc);
	else if (ft_strncmp(line + i, "EA ", 3) == 0)
		save_texture(&(cfg->ea_path), line, i + 3, gc);
	else if (ft_strncmp(line + i, "DO ", 3) == 0)
		save_texture(&(cfg->door_path), line, i + 3, gc);
	else
		return (false);
	return (true);
}

void	parse_config(char *line, t_config *cfg, t_gc **gc)
{
	int	i;

	i = skip_whitespaces(line, 0);
	if (parse_texture_config(line, i, cfg, gc))
		return ;
	if (line[i] == 'F' && (line[i + 1] == ' ' || line[i + 1] == '\t'))
	{
		if (cfg->has_floor)
			gc_free_all_and_exit(gc, "floor color defined more than once", 1);
		parse_color(cfg->floor, line, i + 1, gc);
		cfg->has_floor = 1;
	}
	else if (line[i] == 'C' && (line[i + 1] == ' ' || line[i + 1] == '\t'))
	{
		if (cfg->has_ceil)
			gc_free_all_and_exit(gc, "ceiling color defined more than once", 1);
		parse_color(cfg->ceiling, line, i + 1, gc);
		cfg->has_ceil = 1;
	}
	else
		gc_free_all_and_exit(gc, "invalid configuration identifier", 1);
}

void	check_textures(t_config *cfg, t_gc **gc)
{
	char	*paths[5];
	int		i;
	int		fd;
	size_t	len;

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
