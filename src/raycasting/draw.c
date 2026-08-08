/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caglasener <caglasener@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 17:00:00 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/07 17:00:00 by caglasener       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>

/*
** Hangi texture (0=NO,1=SO,2=WE,3=EA,4=DO) kullanilacagini secer:
** kapiya carpildiysa dogrudan 4; degilse side ve isinin yon isaretinden
** (kuzey/guney/dogu/bati) cikarilir.
*/
static int	get_face(t_ray *ray)
{
	if (ray->hit_char == 'D')
		return (4);
	if (ray->side == 0)
	{
		if (ray->ray_dir_x > 0)
			return (3);
		return (2);
	}
	if (ray->ray_dir_y > 0)
		return (1);
	return (0);
}

/*
** wall_x ([0,1) araligi) texture genisligine gore piksel sutununa
** cevrilir; belirli yon/side kombinasyonlarinda texture aynalanir ki
** duvarin iki yuzu de tutarli gorunsun.
*/
static int	get_tex_x(t_ray *ray, t_img *texture, int face)
{
	int	tex_x;

	tex_x = (int)(ray->wall_x * texture[face].width);
	if (ray->side == 0 && ray->ray_dir_x > 0)
		tex_x = texture[face].width - tex_x - 1;
	if (ray->side == 1 && ray->ray_dir_y < 0)
		tex_x = texture[face].width - tex_x - 1;
	return (tex_x);
}

/*
** Her ekran satirinda tex_y, step kadar ilerletilerek texture'dan
** okunur ve frame'e basilir.
*/
static void	draw_column(t_img *frame, t_img *texture, t_ray *ray, int x)
{
	int		tex_x;
	int		tex_y;
	double	step;
	double	tex_pos;
	int		y;

	tex_x = get_tex_x(ray, texture, get_face(ray));
	step = get_tex_step(ray, texture, get_face(ray), &tex_pos);
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		tex_y = (int)tex_pos;
		tex_pos += step;
		put_pixel(frame, x, y,
			tex_pixel(&texture[get_face(ray)], tex_x, tex_y));
		y++;
	}
}

static void	render_frame(t_img *frame, t_player *player, t_config *cfg,
		t_img *textures)
{
	int		x;
	double	camera_x;
	t_ray	ray;

	draw_background(frame, cfg);
	x = 0;
	while (x < WIN_W)
	{
		camera_x = 2.0 * x / (double)WIN_W - 1.0;
		cast_ray(player, cfg, camera_x, &ray);
		wall_height(&ray);
		draw_column(frame, textures, &ray, x);
		x++;
	}
}

int	render_loop(t_game *game)
{
	move_player(game);
	render_frame(&game->frame, &game->player, &game->cfg, game->textures);
	mlx_put_image_to_window(game->mlx, game->win, game->frame.ptr, 0, 0);
	return (0);
}
