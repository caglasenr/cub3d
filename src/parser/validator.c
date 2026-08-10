/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 15:34:46 by iogul             #+#    #+#             */
/*   Updated: 2026/08/10 15:45:21 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	try_push(t_flood_ctx *ctx, int x, int y)
{
	if (x < 0 || x >= ctx->w || y < 0 || y >= ctx->h)
	{
		ctx->leaked = true;
		return ;
	}
	if (ctx->copy[y][x] == ' ')
	{
		ctx->leaked = true;
		return ;
	}
	if (ctx->copy[y][x] == '1' || ctx->copy[y][x] == 'V')
		return ;
	ctx->copy[y][x] = 'V';
	ctx->stack[ctx->top].x = x;
	ctx->stack[ctx->top].y = y;
	ctx->top++;
}

static bool	init_flood_ctx(t_flood_ctx *ctx, t_config *cfg, char **cp, t_cell s)
{
	ctx->copy = cp;
	ctx->w = cfg->map_w;
	ctx->h = cfg->map_h;
	ctx->leaked = false;
	ctx->stack = malloc(sizeof(t_cell) * (cfg->map_w * cfg->map_h + 1));
	if (!ctx->stack)
		return (false);
	ctx->top = 0;
	cp[s.y][s.x] = 'V';
	ctx->stack[ctx->top] = s;
	ctx->top++;
	return (true);
}

bool	flood_fill(t_config *cfg, int x, int y, char **copy)
{
	t_flood_ctx	ctx;
	t_cell		cur;
	t_cell		start;

	if (x < 0 || x >= cfg->map_w || y < 0 || y >= cfg->map_h)
		return (false);
	if (copy[y][x] == ' ')
		return (false);
	start.x = x;
	start.y = y;
	if (!init_flood_ctx(&ctx, cfg, copy, start))
		return (false);
	while (ctx.top > 0)
	{
		ctx.top--;
		cur = ctx.stack[ctx.top];
		try_push(&ctx, cur.x, cur.y - 1);
		try_push(&ctx, cur.x, cur.y + 1);
		try_push(&ctx, cur.x - 1, cur.y);
		try_push(&ctx, cur.x + 1, cur.y);
	}
	free(ctx.stack);
	return (!ctx.leaked);
}

static void	check_unclosed_zeros(t_config *cfg, t_gc **gc)
{
	int	x;
	int	y;

	y = -1;
	while (++y < cfg->map_h)
	{
		x = -1;
		while (cfg->map[y][++x])
		{
			if (cfg->map[y][x] == '0' || cfg->map[y][x] == 'D')
			{
				if (y == 0 || y == cfg->map_h - 1 || x == 0
					|| !cfg->map[y][x + 1])
					gc_free_all_and_exit(gc, "map is not closed", 1);
				if (cfg->map[y - 1][x] == ' ' || cfg->map[y + 1][x] == ' '
					|| cfg->map[y][x - 1] == ' ' || cfg->map[y][x + 1] == ' ')
					gc_free_all_and_exit(gc, "map is not closed", 1);
			}
		}
	}
}

void	validate_map(t_config *cfg, t_gc **gc)
{
	char	**map_copy;
	int		y;

	map_copy = gc_malloc(gc, sizeof(char *) * (cfg->map_h + 1));
	y = -1;
	while (++y < cfg->map_h)
		map_copy[y] = gc_strdup(gc, cfg->map[y]);
	map_copy[y] = NULL;
	if (!flood_fill(cfg, cfg->player_x, cfg->player_y, map_copy))
		gc_free_all_and_exit(gc, "map is not closed", 1);
	check_unclosed_zeros(cfg, gc);
}
