/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caglasener <caglasener@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 17:00:00 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/08 17:55:16 by caglasener       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_walkable(t_game *game, double x, double y)
{
	char	cell;

	cell = game->cfg.map[(int)y][(int)x];
	return (cell != '1' && cell != 'D');
}

void	move_w(t_game *game)
{
	double	new_x;
	double	new_y;

	if (!game->keys[119])
		return ;
	new_x = game->player.pos_x
		+ game->player.dir_x * MOVE_SPEED;
	new_y = game->player.pos_y
		+ game->player.dir_y * MOVE_SPEED;
	if (is_walkable(game, new_x, game->player.pos_y))
		game->player.pos_x = new_x;
	if (is_walkable(game, game->player.pos_x, new_y))
		game->player.pos_y = new_y;
}

void	move_s(t_game *game)
{
	double	new_x;
	double	new_y;

	if (!game->keys[115])
		return ;
	new_x = game->player.pos_x
		- game->player.dir_x * MOVE_SPEED;
	new_y = game->player.pos_y
		- game->player.dir_y * MOVE_SPEED;
	if (is_walkable(game, new_x, game->player.pos_y))
		game->player.pos_x = new_x;
	if (is_walkable(game, game->player.pos_x, new_y))
		game->player.pos_y = new_y;
}

/* D tusu: yon vektorune dik olan (-dir_y, dir_x) vektorunu kullanir */
void	move_d(t_game *game)
{
	double	new_x;
	double	new_y;

	if (!game->keys[100])
		return ;
	new_x = game->player.pos_x
		- game->player.dir_y * MOVE_SPEED;
	new_y = game->player.pos_y
		+ game->player.dir_x * MOVE_SPEED;
	if (is_walkable(game, new_x, game->player.pos_y))
		game->player.pos_x = new_x;
	if (is_walkable(game, game->player.pos_x, new_y))
		game->player.pos_y = new_y;
}

/* A tusu: yon vektorune dik olan (dir_y, -dir_x) vektorunu kullanir */
void	move_a(t_game *game)
{
	double	new_x;
	double	new_y;

	if (!game->keys[97])
		return ;
	new_x = game->player.pos_x
		+ game->player.dir_y * MOVE_SPEED;
	new_y = game->player.pos_y
		- game->player.dir_x * MOVE_SPEED;
	if (is_walkable(game, new_x, game->player.pos_y))
		game->player.pos_x = new_x;
	if (is_walkable(game, game->player.pos_x, new_y))
		game->player.pos_y = new_y;
}
