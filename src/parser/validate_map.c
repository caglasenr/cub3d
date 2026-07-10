/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:26:59 by csener            #+#    #+#             */
/*   Updated: 2025/06/20 16:35:19 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Recursive flood fill from the player. If it ever steps out of the grid or
** onto a space, the walkable area is not enclosed by walls -> map is open.
*/
static void	flood(t_fill *f, char **g, int y, int x)
{
	if (y < 0 || y >= f->h || x < 0 || x >= f->w)
	{
		f->leak = 1;
		return ;
	}
	if (g[y][x] == '1' || g[y][x] == 'V')
		return ;
	if (g[y][x] == ' ')
	{
		f->leak = 1;
		return ;
	}
	g[y][x] = 'V';
	flood(f, g, y + 1, x);
	flood(f, g, y - 1, x);
	flood(f, g, y, x + 1);
	flood(f, g, y, x - 1);
}

static void	fill_row(char *dst, char *src, int w)
{
	int	x;
	int	len;

	len = (int)ft_strlen(src);
	x = -1;
	while (++x < w)
	{
		if (x < len)
			dst[x] = src[x];
		else
			dst[x] = ' ';
	}
	dst[w] = '\0';
}

/*
** Builds a rectangular copy of the map padded to map_w with spaces, so the
** flood fill can index every row safely regardless of jagged line lengths.
*/
static char	**build_grid(t_config *cfg)
{
	char	**g;
	int		y;

	g = malloc(sizeof(char *) * (cfg->map_h + 1));
	if (!g)
		return (NULL);
	g[cfg->map_h] = NULL;
	y = -1;
	while (++y < cfg->map_h)
	{
		g[y] = malloc(cfg->map_w + 1);
		if (!g[y])
			return (free_split(g), NULL);
		fill_row(g[y], cfg->map[y], cfg->map_w);
	}
	return (g);
}

int	check_closed(t_config *cfg)
{
	char	**g;
	t_fill	f;

	g = build_grid(cfg);
	if (!g)
		return (-1);
	f.h = cfg->map_h;
	f.w = cfg->map_w;
	f.leak = 0;
	flood(&f, g, cfg->player_y, cfg->player_x);
	free_split(g);
	if (f.leak)
		return (err("map is not closed by walls"));
	return (0);
}
