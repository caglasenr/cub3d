/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caglasener <caglasener@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 17:00:00 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/07 17:00:00 by caglasener       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** mlx, frame->addr'i ekranin tum piksellerini yan yana dizilmis tek
** boyutlu byte dizisi olarak tutar (iki boyutlu degil). addr baslangic
** adresi, her satir line_len byte uzunlugunda, her piksel bpp/8 byte
** yer kaplar. Bir (x,y) pikseline gitmek icin:
** addr + y*line_len + x*(bpp/8)  <=>  baslangic + satir + sutun.
*/
void	put_pixel(t_img *img, int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return ;
	*(int *)(img->addr + y * img->line_len + x * (img->bpp / 8)) = color;
}

int	tex_pixel(t_img *img, int x, int y)
{
	return (*(int *)(img->addr + y * img->line_len + x * (img->bpp / 8)));
}

/*
** texture'in bir sutununu ray->draw_start..draw_end araligina sigdirmak
** icin dikey adim (step = texture_yuksekligi / line_height) hesaplanir
** ve baslangic tex_pos'u *tex_pos'a yazilir.
*/
double	get_tex_step(t_ray *ray, t_img *texture, int face, double *tex_pos)
{
	int		line_height;
	double	step;

	line_height = (int)(WIN_H / ray->perp_wall_dist);
	step = (double)texture[face].height / line_height;
	*tex_pos = (ray->draw_start - (WIN_H / 2 - line_height / 2)) * step;
	return (step);
}

/*
** perp_wall_dist buyudukce (duvardan uzaklastikca) line_height kuculur.
** Oyuncu duvara cok yaklasirsa line_height ekran boyutunu asabilir;
** o zaman draw_start negatif, draw_end ekran disina cikabilir, bu
** yuzden ikisini de ekran sinirlarina klemplemek gerekir.
*/
void	wall_height(t_ray *ray)
{
	int	line_height;

	line_height = (int)(WIN_H / ray->perp_wall_dist);
	ray->draw_start = WIN_H / 2 - line_height / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = WIN_H / 2 + line_height / 2;
	if (ray->draw_end >= WIN_H)
		ray->draw_end = WIN_H - 1;
}

/*
** Isin atmadan once ekrani ikiye boler: ust yari tavan rengi, alt yari
** zemin rengi. Duvar ciziminin ezip gececegi kismin altinda kalan
** gokyuzu/zemin boylece duz renkte kalir.
*/
void	draw_background(t_img *frame, t_config *config)
{
	int	ceiling_color;
	int	floor_color;
	int	color;
	int	x;
	int	y;

	ceiling_color = (config->ceiling[0] << 16) | (config->ceiling[1] << 8)
		| (config->ceiling[2]);
	floor_color = (config->floor[0] << 16) | (config->floor[1] << 8)
		| (config->floor[2]);
	y = 0;
	while (y < frame->height)
	{
		if (y < frame->height / 2)
			color = ceiling_color;
		else
			color = floor_color;
		x = 0;
		while (x < frame->width)
		{
			put_pixel(frame, x, y, color);
			x++;
		}
		y++;
	}
}
