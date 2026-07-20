/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csener <csener@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:26:59 by csener            #+#    #+#             */
/*   Updated: 2025/06/20 16:35:19 by csener           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

/*
** GEÇİCİ test main'i — parser'ı izole doğrular.
** Raycaster'a başlayınca bu dosyanın içeriğini MLX kurulumu + çizim döngüsüyle
** değiştirin. (Kişi A: MLX katmanı, Kişi B: raycasting matematiği.)
*/

static void	print_rgb(char *label, int *c)
{
	printf("%s %d,%d,%d\n", label, c[0], c[1], c[2]);
}

static void	print_summary(t_config *cfg)
{
	int	i;

	printf("NO: %s\n", cfg->no_path);
	printf("SO: %s\n", cfg->so_path);
	printf("WE: %s\n", cfg->we_path);
	printf("EA: %s\n", cfg->ea_path);
	if (cfg->door_path)
		printf("DO: %s\n", cfg->door_path);
	if (cfg->sprite_path)
		printf("SP: %s\n", cfg->sprite_path);
	print_rgb("Floor:  ", cfg->floor);
	print_rgb("Ceiling:", cfg->ceiling);
	printf("Player '%c' at %d,%d\n", cfg->player_dir,
		cfg->player_x, cfg->player_y);
	printf("Map %dx%d:\n", cfg->map_w, cfg->map_h);
	i = -1;
	while (++i < cfg->map_h)
		printf("%s\n", cfg->map[i]);
}

int	main(int argc, char **argv)
{
	t_config	cfg;
	t_gc		*gc;

	gc = NULL;
	if (argc != 2)
		gc_free_all_and_exit(&gc, "usage: ./cub3D <scene.cub>", 1);
	if (check_extension(argv[1]) != 0)
		gc_free_all_and_exit(&gc, "map file must have the .cub extension", 1);
	init_config(&cfg);
	parse_cub_file(argv[1], &cfg, &gc);
	printf("Parsing OK\n");
	print_summary(&cfg);
	gc_free_all_and_exit(&gc, NULL, 0);
	return (0);
}
