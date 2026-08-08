/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caglasener <caglasener@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 17:00:00 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/07 17:00:00 by caglasener       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_ray_dir(t_ray *ray, t_player *player, double camera_x)
{
	ray->ray_dir_x = player->dir_x + player->plane_x * camera_x;
	ray->ray_dir_y = player->dir_y + player->plane_y * camera_x;
	ray->map_x = (int)player->pos_x;
	ray->map_y = (int)player->pos_y;
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
}

static void	init_ray_step(t_ray *ray, t_player *player)
{
	if (ray->ray_dir_x > 0)
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1 - player->pos_x)
			* ray->delta_dist_x;
	}
	else
	{
		ray->step_x = -1;
		ray->side_dist_x = (player->pos_x - ray->map_x) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y > 0)
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1 - player->pos_y)
			* ray->delta_dist_y;
	}
	else
	{
		ray->step_y = -1;
		ray->side_dist_y = (player->pos_y - ray->map_y) * ray->delta_dist_y;
	}
}

/*
** DDA: siradaki hucre sinirina daha yakin olan eksende bir adim ilerler,
** o hucre duvar ('1'), kapali kapi ('D') ya da acik kapi ('o') ise durur.
*/
static void	dda_loop(t_ray *ray, t_config *cfg)
{
	while (1)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (cfg->map[ray->map_y][ray->map_x] == '1'
			|| cfg->map[ray->map_y][ray->map_x] == 'D'
			|| cfg->map[ray->map_y][ray->map_x] == 'o')
		{
			ray->hit_char = cfg->map[ray->map_y][ray->map_x];
			break ;
		}
	}
}

/*
** wall_x: carpma noktasinin duvar boyunca [0,1) araligindaki konumu.
** side==0 ise dikey duvara carpilmistir, konum oyuncunun y'sinden ve
** kat edilen mesafeden (perp_wall_dist * ray_dir_y) turetilir; side==1
** icin ayni mantik x ekseninde uygulanir. floor() ile tam sayi kismi
** atilir, geriye sadece hucre icindeki kesirli konum kalir.
*/
static void	calc_wall_x(t_ray *ray, t_player *player)
{
	if (ray->side == 0)
		ray->perp_wall_dist = ray->side_dist_x - ray->delta_dist_x;
	else
		ray->perp_wall_dist = ray->side_dist_y - ray->delta_dist_y;
	if (ray->side == 0)
		ray->wall_x = player->pos_y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		ray->wall_x = player->pos_x + ray->perp_wall_dist * ray->ray_dir_x;
	ray->wall_x -= floor(ray->wall_x);
}

void	cast_ray(t_player *player, t_config *cfg, double camera_x, t_ray *ray)
{
	init_ray_dir(ray, player, camera_x);
	init_ray_step(ray, player);
	dda_loop(ray, cfg);
	calc_wall_x(ray, player);
}
