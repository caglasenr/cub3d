/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:26:59 by csener            #+#    #+#             */
/*   Updated: 2025/06/20 16:35:19 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_config(t_config *cfg)
{
	cfg->no_path = NULL;
	cfg->so_path = NULL;
	cfg->we_path = NULL;
	cfg->ea_path = NULL;
	cfg->floor[0] = -1;
	cfg->floor[1] = -1;
	cfg->floor[2] = -1;
	cfg->ceiling[0] = -1;
	cfg->ceiling[1] = -1;
	cfg->ceiling[2] = -1;
	cfg->has_floor = 0;
	cfg->has_ceil = 0;
	cfg->map = NULL;
	cfg->map_h = 0;
	cfg->map_w = 0;
	cfg->player_dir = 0;
}

/*
** Walks the pre-map region: skips empty lines, parses each element, and stops
** at the first line that is not an element (that is where the map begins).
*/
int	parse_lines(t_config *cfg, char **lines, int *map_start)
{
	int	i;
	int	r;

	i = 0;
	while (lines[i])
	{
		if (is_empty_line(lines[i]))
		{
			i++;
			continue ;
		}
		r = parse_element_line(cfg, lines[i]);
		if (r < 0)
			return (-1);
		if (r == 0)
			break ;
		i++;
	}
	*map_start = i;
	if (!cfg->no_path || !cfg->so_path || !cfg->we_path
		|| !cfg->ea_path || !cfg->has_floor || !cfg->has_ceil)
		return (err("missing or incomplete scene element"));
	return (0);
}

int	build_map(t_config *cfg, char **lines, int start)
{
	if (collect_map(cfg, lines, start) != 0)
		return (-1);
	if (scan_map(cfg) != 0)
		return (-1);
	if (check_closed(cfg) != 0)
		return (-1);
	return (0);
}

int	parse_scene(t_config *cfg, char *path)
{
	char	**lines;
	int		fd;
	int		start;

	if (check_extension(path) != 0)
		return (err("map file must have the .cub extension"));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (err("cannot open the map file"));
	lines = read_file(fd);
	close(fd);
	if (!lines)
		return (err("read failure"));
	if (parse_lines(cfg, lines, &start) != 0)
		return (free_split(lines), -1);
	if (build_map(cfg, lines, start) != 0)
		return (free_split(lines), -1);
	free_split(lines);
	if (check_textures(cfg) != 0)
		return (-1);
	return (0);
}
