#include "cub3d.h"
#include <mlx.h>


#include "cub3d.h"

static void	load_single_texture(t_game *game, t_img *tex, char *path)
{
	// 1. .xpm dosyasını okuyup MLX image'ına çeviriyoruz
	tex->ptr = mlx_xpm_file_to_image(game->mlx, path, &tex->width, &tex->height);
	
	// 2. Yükleme başarısız olursa (NULL dönerse) hata verip çıkıyoruz
	if (!tex->ptr)
	{
		gc_free_all_and_exit(&game->gc, "Texture loading failed", 1);
	}
	
	// 3. Çizim sırasında pikselleri okuyabilmek için veri adresini alıyoruz[cite: 2]
	tex->addr = mlx_get_data_addr(tex->ptr, &tex->bpp, &tex->line_len, &tex->endian);
}

void	load_textures(t_game *game)
{
	// 4 farklı yön için dokuları sırayla game->textures dizisine yüklüyoruz[cite: 2]
	load_single_texture(game, &game->textures[0], game->cfg.no_path);
	load_single_texture(game, &game->textures[1], game->cfg.so_path);
	load_single_texture(game, &game->textures[2], game->cfg.we_path);
	load_single_texture(game, &game->textures[3], game->cfg.ea_path);
    if (game->cfg.door_path)
        load_single_texture(game, &game->textures[4], game->cfg.door_path);
}

void init_game(t_game *game)
{
    init_player(&game->cfg, &game->player);
    game->mlx = mlx_init();
    if (!game->mlx)
        gc_free_all_and_exit(&game->gc, "MLX initialization failed", 1);

    load_textures(game);
    game->win = mlx_new_window(game->mlx, WIN_W, WIN_H, "cub3D");
    if (!game->win)
        gc_free_all_and_exit(&game->gc, "Window creation failed", 1);
    game->frame.ptr = mlx_new_image(game->mlx, WIN_W, WIN_H); //ekran boyutunca bir frame açıyorher pikseller önce buraya çiziliyo
    if (!game->frame.ptr)
        gc_free_all_and_exit(&game->gc, "Frame image creation failed", 1);
    
    game->frame.addr = mlx_get_data_addr(game->frame.ptr, &game->frame.bpp, 
                                         &game->frame.line_len, &game->frame.endian);
    game->frame.width = WIN_W;
    game->frame.height = WIN_H;
        
    mlx_hook(game->win, 2, 1L<<0, key_press, game);
    mlx_hook(game->win, 17, 0, close_window, game);

    // Tuşların başlangıç değerini 0 yapıyoruz
    ft_memset(game->keys, 0, sizeof(int) * 65536);

    // Mevcut key_press ve close_window hooklarının altına ekle:
    mlx_hook(game->win, 3, 1L<<1, key_release, game); // Tuş bırakmayı dinler
    mlx_loop_hook(game->mlx, render_loop, game);
}