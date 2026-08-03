#include "cub3d.h"
#include <mlx.h>

int main(int argc, char **argv)
{
    t_game game;

    game.gc = NULL;
    init_config(&game.cfg);

    if (argc != 2)
        gc_free_all_and_exit(&game.gc, "usage: ./cub3D <scene.cub>", 1);
    if (check_extension(argv[1]) != 0)
        gc_free_all_and_exit(&game.gc, "map file must have the .cub extension", 1);

    parse_cub_file(argv[1], &game.cfg, &game.gc);
    
    // Motoru başlat ve döngüye gir
    init_game(&game);
    mlx_loop(game.mlx);

    return (0);
}
