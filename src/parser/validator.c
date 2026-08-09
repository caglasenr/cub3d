/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:58:21 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/09 15:23:20 by iogul            ###   ########.fr       */
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

static bool	init_flood_ctx(t_flood_ctx *ctx, t_config *cfg, char **copy,
		t_cell start)
{
	ctx->copy = copy;
	ctx->w = cfg->map_w;
	ctx->h = cfg->map_h;
	ctx->leaked = false;
	ctx->stack = malloc(sizeof(t_cell) * (cfg->map_w * cfg->map_h + 1));
	if (!ctx->stack)
		return (false);
	ctx->top = 0;
	copy[start.y][start.x] = 'V';
	ctx->stack[ctx->top] = start;
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

void	validate_map(t_config *cfg, t_gc **gc)
{
	char	**map_copy;
	int		y;

	map_copy = gc_malloc(gc, sizeof(char *) * (cfg->map_h + 1));
	y = 0;
	while (y < cfg->map_h)
	{
		map_copy[y] = gc_strdup(gc, cfg->map[y]);
		y++;
	}
	map_copy[y] = NULL;
	if (!flood_fill(cfg, cfg->player_x, cfg->player_y, map_copy))
		gc_free_all_and_exit(gc,
			"map is not closed, there are gaps in the walls", 1);
}
