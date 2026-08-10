/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 17:00:00 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/10 19:17:13 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>
#include <stdlib.h>

int	close_window(t_game *game)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		if (game->textures[i].ptr)
			mlx_destroy_image(game->mlx, game->textures[i].ptr);
		i++;
	}
	if (game->frame.ptr)
		mlx_destroy_image(game->mlx, game->frame.ptr);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	gc_free_all_and_exit(&game->gc, NULL, 0);
	return (0);
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == 65307)
		close_window(game);
	if (keycode >= 0 && keycode < 65536)
		game->keys[keycode] = 1;
	if (keycode == 101)
		toggle_door(game);
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (keycode >= 0 && keycode < 65536)
		game->keys[keycode] = 0;
	return (0);
}

void	move_player(t_game *game)
{
	move_w(game);
	move_s(game);
	move_d(game);
	move_a(game);
	rotate_right(game);
	rotate_left(game);
}

void	toggle_door(t_game *game)
{
	int	door_x;
	int	door_y;

	door_x = (int)(game->player.pos_x + game->player.dir_x);
	door_y = (int)(game->player.pos_y + game->player.dir_y);
	if (game->cfg.map[door_y][door_x] == 'D')
		game->cfg.map[door_y][door_x] = 'o';
	else if (game->cfg.map[door_y][door_x] == 'o')
		game->cfg.map[door_y][door_x] = 'D';
}
