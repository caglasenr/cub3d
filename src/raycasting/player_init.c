/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 17:00:00 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/09 15:24:38 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_pos(t_config *cfg, t_player *player)
{
	player->pos_x = cfg->player_x + 0.5;
	player->pos_y = cfg->player_y + 0.5;
}

static void	set_dir_ns(char dir, t_player *player)
{
	if (dir == 'N')
	{
		player->dir_x = 0.0;
		player->dir_y = -1.0;
		player->plane_x = 0.66;
		player->plane_y = 0;
	}
	else
	{
		player->dir_x = 0.0;
		player->dir_y = 1.0;
		player->plane_x = -0.66;
		player->plane_y = 0;
	}
}

static void	set_dir_ew(char dir, t_player *player)
{
	if (dir == 'E')
	{
		player->dir_x = 1.0;
		player->dir_y = 0.0;
		player->plane_x = 0.0;
		player->plane_y = 0.66;
	}
	else
	{
		player->dir_x = -1.0;
		player->dir_y = 0.0;
		player->plane_x = 0.0;
		player->plane_y = -0.66;
	}
}

void	set_dir_plane(t_config *cfg, t_player *player)
{
	if (cfg->player_dir == 'N' || cfg->player_dir == 'S')
		set_dir_ns(cfg->player_dir, player);
	else
		set_dir_ew(cfg->player_dir, player);
}

void	init_player(t_config *cfg, t_player *player)
{
	set_pos(cfg, player);
	set_dir_plane(cfg, player);
}
