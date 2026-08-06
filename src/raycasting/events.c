#include "cub3d.h"
#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>


int close_window(t_game *game)
{
    int i;

    // 1. Texture (Doku) resimlerini bellekten temizle
    i = 0;
    while (i < 4)
    {
        if (game->textures[i].ptr)
            mlx_destroy_image(game->mlx, game->textures[i].ptr);
        i++;
    }

    // 2. Ana ekran (frame) buffer'ını temizle
    if (game->frame.ptr)
        mlx_destroy_image(game->mlx, game->frame.ptr);

    // 3. Pencereyi ve MLX ekran bağlantısını temizle (Linux spesifik)
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);

    // 4. Parser belleklerini temizle ve programı güvenle kapat
    // gc_free_all_and_exit zaten arkada exit() çağırıyor.
    gc_free_all_and_exit(&game->gc, NULL, 0);
    return (0);
}

int key_press(int keycode, t_game *game)
{
    if (keycode == 53) // ESC
        close_window(game);
    if (keycode >= 0 && keycode < 65536)
        game->keys[keycode] = 1; // Tuşa basıldığında o tuşun değerini 1 yap
    if (keycode == 14) // E tuşu
        toggle_door(game);
    return (0);
}

int key_release(int keycode, t_game *game)
{
    if (keycode >= 0 && keycode < 65536)
        game->keys[keycode] = 0; // Tuş bırakıldığında 0 yap
    return (0);
}

void move_player(t_game *game)
{
    double old_dir_x;
    double old_plane_x;

    // --- İLERİ / GERİ (W ve S) ---
    if (game->keys[13]) // W
    {
        if (game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != '1' && game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
            game->player.pos_x += game->player.dir_x * MOVE_SPEED;
        if (game->cfg.map[(int)(game->player.pos_y + game->player.dir_y * MOVE_SPEED)][(int)game->player.pos_x] != '1' && game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
            game->player.pos_y += game->player.dir_y * MOVE_SPEED;
    }
    if (game->keys[1]) // S
    {
        if (game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x - game->player.dir_x * MOVE_SPEED)] != '1' && game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
            game->player.pos_x -= game->player.dir_x * MOVE_SPEED;
        if (game->cfg.map[(int)(game->player.pos_y - game->player.dir_y * MOVE_SPEED)][(int)game->player.pos_x] != '1' && game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
            game->player.pos_y -= game->player.dir_y * MOVE_SPEED;
    }

    // --- SAĞA / SOLA KAYMA (D ve A - Strafing) ---
    if (game->keys[2]) // D tuşu: Sağa kayma
    {
        // Yön vektörüne dik olan (-dir_y, dir_x) vektörünü kullanıyoruz
        if (game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x - game->player.dir_y * MOVE_SPEED)] != '1' && game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
            game->player.pos_x -= game->player.dir_y * MOVE_SPEED;
        if (game->cfg.map[(int)(game->player.pos_y + game->player.dir_x * MOVE_SPEED)][(int)game->player.pos_x] != '1' && game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
            game->player.pos_y += game->player.dir_x * MOVE_SPEED;
    }
    if (game->keys[0]) // A tuşu: Sola kayma
    {
        // Yön vektörüne dik olan (dir_y, -dir_x) vektörünü kullanıyoruz
        if (game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x + game->player.dir_y * MOVE_SPEED)] != '1' && game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
            game->player.pos_x += game->player.dir_y * MOVE_SPEED;
        if (game->cfg.map[(int)(game->player.pos_y - game->player.dir_x * MOVE_SPEED)][(int)game->player.pos_x] != '1' && game->cfg.map[(int)game->player.pos_y][(int)(game->player.pos_x + game->player.dir_x * MOVE_SPEED)] != 'D')
            game->player.pos_y -= game->player.dir_x * MOVE_SPEED;
    }

    // --- KAMERA ROTASYONU (Sağ ve Sol Ok Tuşları) ---
    if (game->keys[124]) // Sağ Ok Tuşu: Sağa dön (Saat yönü)
    {
        // Hem yön (dir) hem de kamera düzlemi (plane) vektörlerini döndürüyoruz[cite: 1]
        old_dir_x = game->player.dir_x;
        game->player.dir_x = game->player.dir_x * cos(ROT_SPEED) - game->player.dir_y * sin(ROT_SPEED);
        game->player.dir_y = old_dir_x * sin(ROT_SPEED) + game->player.dir_y * cos(ROT_SPEED);

        old_plane_x = game->player.plane_x;
        game->player.plane_x = game->player.plane_x * cos(ROT_SPEED) - game->player.plane_y * sin(ROT_SPEED);
        game->player.plane_y = old_plane_x * sin(ROT_SPEED) + game->player.plane_y * cos(ROT_SPEED);
    }
    if (game->keys[123]) // Sol Ok Tuşu: Sola dön (Saat yönünün tersi)
    {
        old_dir_x = game->player.dir_x;
        // Sola dönüş için eksi açı (-ROT_SPEED) kullanılır, trigonometrik denklem buna göre ayarlanır
        game->player.dir_x = game->player.dir_x * cos(-ROT_SPEED) - game->player.dir_y * sin(-ROT_SPEED);
        game->player.dir_y = old_dir_x * sin(-ROT_SPEED) + game->player.dir_y * cos(-ROT_SPEED);

        old_plane_x = game->player.plane_x;
        game->player.plane_x = game->player.plane_x * cos(-ROT_SPEED) - game->player.plane_y * sin(-ROT_SPEED);
        game->player.plane_y = old_plane_x * sin(-ROT_SPEED) + game->player.plane_y * cos(-ROT_SPEED);
    }
}
void toggle_door(t_game *game)
{
    int door_x;
    int door_y;

    door_x = (int)(game->player.pos_x + game->player.dir_x);
    door_y = (int)(game->player.pos_y + game->player.dir_y);
    if (game->cfg.map[door_y][door_x] == 'D')
        game->cfg.map[door_y][door_x] = 'o';
    else if (game->cfg.map[door_y][door_x] == 'o')
        game->cfg.map[door_y][door_x] = 'D';
}