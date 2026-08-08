/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caglasener <caglasener@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 17:00:00 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/07 17:00:00 by caglasener       ###   ########.fr       */
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

/*
** N yonu icin plane_x=0.66 referans alinir; diger uc yon ondan turetilir.
** (x,y)'ye dik iki vektor vardir: (-y,x) ve (y,-x); ikisi de dot=0 verir,
** biri +90 derece oteki -90 derece dondurme. Hangisi secilirse secilsin
** dordunde de AYNI kural kullanilmali, cunku plane kameranin "sag" yonunu
** temsil eder (ekranin x=0 sutunu bir ucta, x=WIN_W-1 diger ucta, bu sweep
** dir'e gore hep ayni elden olmali). N icin x cevirip E icin y cevirirsek,
** oyuncu dondukce goruntu sag-sol tutarsiz sekilde aynalanir (MLX'te
** goruntulenince fark edilecek bir bug).
**
** N bakista E(sag) yonu (1,0), plane (0.66,0)
** E bakista S(alt) yonu (0,1), plane (0,0.66)
** S bakista W(bati) yonu (-1,0), plane (-0.66,0)
** W bakista N(yukari) yonu (0,-1), plane (0,-0.66)
** plane, kameranin FOV genisligini ve yonunu tanimlayan referans vektordur.
*/
void	init_player(t_config *cfg, t_player *player)
{
	set_pos(cfg, player);
	set_dir_plane(cfg, player);
}
