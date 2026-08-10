/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_router.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 15:14:56 by iogul             #+#    #+#             */
/*   Updated: 2026/08/10 15:15:20 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_config(t_config *cfg)
{
	ft_memset(cfg, 0, sizeof(t_config));
}

int	check_extension(char *path)
{
	size_t	len;

	len = ft_strlen(path);
	if (len < 5)
		return (-1);
	if (ft_strncmp(path + len - 4, ".cub", 4) != 0)
		return (-1);
	return (0);
}

static void	handle_map_line(char *line, t_parse_ctx *ctx)
{
	if (ctx->map_ended)
	{
		free(line);
		gc_free_all_and_exit(ctx->gc, "map divided by empty lines", 1);
	}
	ctx->map_started = true;
	append_map_line(line, &ctx->map_list, ctx->gc);
	free(line);
}

static void	process_line(char *line, t_parse_ctx *ctx)
{
	if (is_empty_line(line))
	{
		if (ctx->map_started)
			ctx->map_ended = true;
		free(line);
	}
	else if (is_config_line(line))
	{
		if (ctx->map_started)
		{
			free(line);
			gc_free_all_and_exit(ctx->gc, "config after map started", 1);
		}
		parse_config(line, ctx->cfg, ctx->gc);
		free(line);
	}
	else if (is_map_line(line))
		handle_map_line(line, ctx);
	else
	{
		free(line);
		gc_free_all_and_exit(ctx->gc, "unrecognized line in map file", 1);
	}
}

void	parse_cub_file(char *file_path, t_config *cfg, t_gc **gc)
{
	t_parse_ctx	ctx;
	char		*line;

	ctx.fd = open(file_path, O_RDONLY);
	if (ctx.fd < 0)
		gc_free_all_and_exit(gc, "cannot open the map file", 1);
	ctx.cfg = cfg;
	ctx.gc = gc;
	ctx.map_started = false;
	ctx.map_ended = false;
	ctx.map_list = NULL;
	line = get_next_line(ctx.fd);
	while (line)
	{
		process_line(line, &ctx);
		line = get_next_line(ctx.fd);
	}
	close(ctx.fd);
	if (!cfg->no_path || !cfg->so_path || !cfg->we_path || !cfg->ea_path
		|| !cfg->has_floor || !cfg->has_ceil)
		gc_free_all_and_exit(gc, "missing or incomplete scene element", 1);
	build_final_grid(cfg, ctx.map_list, gc);
	validate_map(cfg, gc);
	check_textures(cfg, gc);
}
