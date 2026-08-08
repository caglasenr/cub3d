/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caglasener <caglasener@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 17:00:00 by caglasener        #+#    #+#             */
/*   Updated: 2026/08/07 17:00:00 by caglasener       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>

int	main(int argc, char **argv)
{
	t_game	game;

	ft_memset(&game, 0, sizeof(t_game));
	game.gc = NULL;
	init_config(&game.cfg);
	if (argc != 2)
		gc_free_all_and_exit(&game.gc, "usage: ./cub3D <scene.cub>", 1);
	if (check_extension(argv[1]) != 0)
		gc_free_all_and_exit(&game.gc,
			"map file must have the .cub extension", 1);
	parse_cub_file(argv[1], &game.cfg, &game.gc);
	init_game(&game);
	mlx_loop(game.mlx);
	return (0);
}
