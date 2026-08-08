/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caglasener <caglasener@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 17:00:00 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/07 17:00:00 by caglasener       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	move_w(t_game *game)
{
	if (!game->keys[119])
		return ;
	if (game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != '1'
		&& game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
		game->player.pos_x += game->player.dir_x * MOVE_SPEED;
	if (game->cfg.map[(int)(game->player.pos_y
			+ game->player.dir_y * MOVE_SPEED)][(int)game->player.pos_x] != '1'
		&& game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
		game->player.pos_y += game->player.dir_y * MOVE_SPEED;
}

void	move_s(t_game *game)
{
	if (!game->keys[115])
		return ;
	if (game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x - game->player.dir_x * MOVE_SPEED)] != '1'
		&& game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
		game->player.pos_x -= game->player.dir_x * MOVE_SPEED;
	if (game->cfg.map[(int)(game->player.pos_y
			- game->player.dir_y * MOVE_SPEED)][(int)game->player.pos_x] != '1'
		&& game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
		game->player.pos_y -= game->player.dir_y * MOVE_SPEED;
}

/* D tusu: yon vektorune dik olan (-dir_y, dir_x) vektorunu kullanir */
void	move_d(t_game *game)
{
	if (!game->keys[100])
		return ;
	if (game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x - game->player.dir_y * MOVE_SPEED)] != '1'
		&& game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
		game->player.pos_x -= game->player.dir_y * MOVE_SPEED;
	if (game->cfg.map[(int)(game->player.pos_y
			+ game->player.dir_x * MOVE_SPEED)][(int)game->player.pos_x] != '1'
		&& game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
		game->player.pos_y += game->player.dir_x * MOVE_SPEED;
}

/* A tusu: yon vektorune dik olan (dir_y, -dir_x) vektorunu kullanir */
void	move_a(t_game *game)
{
	if (!game->keys[97])
		return ;
	if (game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x + game->player.dir_y * MOVE_SPEED)] != '1'
		&& game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
		game->player.pos_x += game->player.dir_y * MOVE_SPEED;
	if (game->cfg.map[(int)(game->player.pos_y
			- game->player.dir_x * MOVE_SPEED)][(int)game->player.pos_x] != '1'
		&& game->cfg.map[(int)game->player.pos_y]
		[(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
		game->player.pos_y -= game->player.dir_x * MOVE_SPEED;
}
