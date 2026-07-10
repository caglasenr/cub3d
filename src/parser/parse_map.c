/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:26:59 by csener            #+#    #+#             */
/*   Updated: 2025/06/20 16:35:19 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_map_char(char c)
{
	return (c == '0' || c == '1' || c == ' '
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static int	is_player(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

/*
** Copies the contiguous map region (lines[start..end]) into cfg->map.
** An empty line inside this region means the map is not a single block
** and always-last, which is a rule violation.
*/
int	collect_map(t_config *cfg, char **lines, int start)
{
	int	n;
	int	i;

	n = 0;
	while (lines[start + n])
	{
		if (is_empty_line(lines[start + n]))
			return (err("empty line inside map or map is not last"));
		n++;
	}
	if (n == 0)
		return (err("map is missing"));
	cfg->map = malloc(sizeof(char *) * (n + 1));
	if (!cfg->map)
		return (-1);
	cfg->map[n] = NULL;
	cfg->map_h = n;
	i = -1;
	while (++i < n)
	{
		cfg->map[i] = ft_strdup(lines[start + i]);
		if (!cfg->map[i])
			return (-1);
	}
	return (0);
}

static int	scan_row(t_config *cfg, int y, int *players)
{
	int		x;
	char	c;

	x = -1;
	while (cfg->map[y][++x])
	{
		c = cfg->map[y][x];
		if (!is_map_char(c))
			return (err("invalid character in map"));
		if (is_player(c))
		{
			cfg->player_x = x;
			cfg->player_y = y;
			cfg->player_dir = c;
			(*players)++;
		}
	}
	return (0);
}

int	scan_map(t_config *cfg)
{
	int	y;
	int	players;
	int	len;

	players = 0;
	cfg->map_w = 0;
	y = -1;
	while (++y < cfg->map_h)
	{
		if (scan_row(cfg, y, &players) != 0)
			return (-1);
		len = (int)ft_strlen(cfg->map[y]);
		if (len > cfg->map_w)
			cfg->map_w = len;
	}
	if (players != 1)
		return (err("map must contain exactly one player"));
	return (0);
}
