/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 16:33:34 by iogul             #+#    #+#             */
/*   Updated: 2026/08/10 16:45:57 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	color_error(char **rgb, char *line, t_gc **gc, char *msg)
{
	int	j;

	j = 0;
	if (rgb)
	{
		while (rgb[j])
			free(rgb[j++]);
		free(rgb);
	}
	if (line)
		free(line);
	gc_free_all_and_exit(gc, msg, 1);
}

void	parse_color(int *target, char *line, int i, t_gc **gc)
{
	char	**rgb;
	int		j;

	i = skip_whitespaces(line, i);
	rgb = ft_split(line + i, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
		color_error(rgb, line, gc, "invalid color format, expected R,G,B");
	if (!is_all_digits(rgb[0]) || !is_all_digits(rgb[1])
		|| !is_all_digits(rgb[2]))
		color_error(rgb, line, gc, "colors must only contain numbers");
	target[0] = ft_atoi(rgb[0]);
	target[1] = ft_atoi(rgb[1]);
	target[2] = ft_atoi(rgb[2]);
	if (target[0] < 0 || target[0] > 255 || target[1] < 0 || target[1] > 255
		|| target[2] < 0 || target[2] > 255)
		color_error(rgb, line, gc, "color values must be between 0 and 255");
	j = -1;
	while (rgb[++j])
		free(rgb[j]);
	free(rgb);
}

void	save_texture(char **target, char *line, int i, t_gc **gc)
{
	char	*path;
	int		len;

	if (*target != NULL)
	{
		free(line);
		gc_free_all_and_exit(gc, "same texture defined more than once", 1);
	}
	i = skip_whitespaces(line, i);
	path = gc_strdup(gc, line + i);
	len = ft_strlen(path);
	if (len > 0 && path[len - 1] == '\n')
		path[len - 1] = '\0';
	*target = path;
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
			color_error(NULL, line, gc, "floor color defined more than once");
		parse_color(cfg->floor, line, i + 1, gc);
		cfg->has_floor = 1;
	}
	else if (line[i] == 'C' && (line[i + 1] == ' ' || line[i + 1] == '\t'))
	{
		if (cfg->has_ceil)
			color_error(NULL, line, gc, "ceiling color defined more than once");
		parse_color(cfg->ceiling, line, i + 1, gc);
		cfg->has_ceil = 1;
	}
	else
		color_error(NULL, line, gc, "invalid configuration identifier");
}
