/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_router.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caglasener <caglasener@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:58:21 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/07 15:58:21 by caglasener       ###   ########.fr       */
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

void	check_textures(t_config *cfg, t_gc **gc)
{
	char	*paths[6];
	int		i;
	int		fd;

	paths[0] = cfg->no_path;
	paths[1] = cfg->so_path;
	paths[2] = cfg->we_path;
	paths[3] = cfg->ea_path;
	paths[4] = cfg->door_path;
	paths[5] = cfg->sprite_path;
	i = -1;
	while (++i < 6)
	{
		if (!paths[i])
			continue ;
		fd = open(paths[i], O_RDONLY);
		if (fd < 0)
			gc_free_all_and_exit(gc, "cannot open a texture file", 1);
		close(fd);
	}
}

// is_empty_line / is_config_line / is_map_line -> parser_utils.c'ye taşındı
// (bu dosya norm'un 5-fonksiyon limitine takılıyordu)

// Tek bir satırı işler: boşsa atla, config satırıysa parse_config'e ver,
// harita satırıysa listeye ekle, hiçbiri değilse hata ver.
// Döngü durumunu (fd, map_started, map_list) tek tek parametre yerine
// t_parse_ctx üzerinden alıyor ki fonksiyon 4 parametre norm limitini aşmasın.
static void	process_line(char *line, t_parse_ctx *ctx)
{
	if (is_empty_line(line))
		free(line);
	else if (is_config_line(line))
	{
		if (ctx->map_started)
		{
			free(line);
			gc_free_all_and_exit(ctx->gc,
				"configuration item after map has started", 1);
		}
		parse_config(line, ctx->cfg, ctx->gc);
		free(line);
	}
	else if (is_map_line(line))
	{
		ctx->map_started = true;
		append_map_line(line, &ctx->map_list, ctx->gc);
		free(line);
	}
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
