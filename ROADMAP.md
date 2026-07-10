# cub3D — Raycaster Yol Haritası (parser sonrası)

Parser **bitti** (`src/parser/`). Bu doküman, parser'ın ürettiği `t_config`
üzerine raycaster'ı **sıfırdan kendiniz yazmanız** için adım adım kılavuzdur.

Referans: Lode Vandevenne — *Raycasting* (https://lodev.org/cgtutor/raycasting.html).
Kodu **anlayarak** yazın; defansta her satırı açıklayabilmelisiniz.

---

## 0. Ortak veri (zaten hazır — `include/cub3d.h`)

Parser `t_config`'i doldurur:
- `no_path/so_path/we_path/ea_path` — 4 duvar texture yolu
- `floor[3]`, `ceiling[3]` — R,G,B (0–255)
- `map` (char**), `map_h`, `map_w`
- `player_x`, `player_y`, `player_dir` ('N'/'S'/'E'/'W')

`t_img` de header'da hazır (frame buffer + texture'lar için).

Raycaster için `t_config`'i saran bir `t_game` ekleyin:
```c
typedef struct s_game {
    void      *mlx;
    void      *win;
    t_img      frame;      // ekran buffer'ı
    t_img      tex[4];     // 0=NO 1=SO 2=WE 3=EA
    t_config   cfg;        // parser çıktısı
    double     pos_x, pos_y;      // oyuncu konumu (hücre + 0.5)
    double     dir_x, dir_y;      // bakış vektörü
    double     plane_x, plane_y;  // kamera düzlemi (FOV)
    int        keys[NUM_KEYS];    // basılı tuşlar (pürüzsüz hareket)
}   t_game;
```

---

## 1. MLX kurulumu & pencere  *(Kişi A)*

1. `minilibx-linux/` klasörünü proje köküne ekleyin, içinde `make` çalıştırın.
2. Kök `Makefile`'daki **MLX bölümünü açın** (yorumdaki 3 satır + `$(NAME)`
   kuralına `$(MLX)` bağımlılığı ve link).
3. Akış:
   - `mlx = mlx_init();`
   - `win = mlx_new_window(mlx, WIN_W, WIN_H, "cub3D");`
   - Frame buffer: `mlx_new_image` + `mlx_get_data_addr`
     (buffer'a çiz → `mlx_put_image_to_window`; subject bunu öneriyor).
4. Piksel yazıcı:
```c
void put_pixel(t_img *img, int x, int y, int color) {
    if (x < 0 || y < 0 || x >= img->width || y >= img->height) return;
    *(int *)(img->addr + y * img->line_len + x * (img->bpp / 8)) = color;
}
```

## 2. Oyuncu vektörleri  *(Kişi A)*

`pos = (player_x + 0.5, player_y + 0.5)`. Yöne göre dir/plane (FOV≈66°):

| Yön | dir (x,y) | plane (x,y) |
|-----|-----------|-------------|
| N   | (0, -1)   | (0.66, 0)   |
| S   | (0, 1)    | (-0.66, 0)  |
| E   | (1, 0)    | (0, 0.66)   |
| W   | (-1, 0)   | (0, -0.66)  |

## 3. DDA — her ekran sütunu için ışın  *(Kişi B)*

```c
for (x = 0; x < WIN_W; x++) {
    cameraX = 2.0 * x / WIN_W - 1;          // [-1, 1]
    rayDirX = dir_x + plane_x * cameraX;
    rayDirY = dir_y + plane_y * cameraX;
    mapX = (int)pos_x;  mapY = (int)pos_y;
    deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
    deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
    // stepX/Y (+1/-1) ve sideDistX/Y başlangıcını rayDir işaretine göre kur
    // DDA: küçük sideDist yönünde ilerle, mapX/Y güncelle, side=0(x)/1(y) yaz,
    //      map[mapY][mapX] == '1' olana kadar
}
```

## 4. Duvar yüksekliği  *(Kişi B)*

```c
if (side == 0) perpWallDist = sideDistX - deltaDistX;
else           perpWallDist = sideDistY - deltaDistY;
lineHeight = (int)(WIN_H / perpWallDist);
drawStart  = -lineHeight / 2 + WIN_H / 2;  if (drawStart < 0) drawStart = 0;
drawEnd    =  lineHeight / 2 + WIN_H / 2;  if (drawEnd >= WIN_H) drawEnd = WIN_H - 1;
```

## 5. Texture seçimi + dikey şerit  *(Kişi B)*

Hangi yüz:
- `side==0 && rayDirX > 0` → **EA (3)**, `< 0` → **WE (2)**
- `side==1 && rayDirY > 0` → **SO (1)**, `< 0` → **NO (0)**

```c
if (side == 0) wallX = pos_y + perpWallDist * rayDirY;
else           wallX = pos_x + perpWallDist * rayDirX;
wallX -= floor(wallX);
texX = (int)(wallX * TEX_W);
// (yön/side'a göre texX'i aynala: doğu/güney duvarlarda TEX_W-1-texX)
step   = (double)TEX_H / lineHeight;
texPos = (drawStart - WIN_H / 2 + lineHeight / 2) * step;
for (y = drawStart; y <= drawEnd; y++) {
    texY = (int)texPos & (TEX_H - 1);
    texPos += step;
    color = tex_pixel(&tex[face], texX, texY);
    put_pixel(&frame, x, y, color);
}
```
Şeridin **üstü** tavan (`cfg.ceiling`), **altı** zemin (`cfg.floor`) rengiyle
doldurulur (ayrı bir dikey döngü ya da tam ekran arka plan).

## 6. Texture yükleme  *(Kişi A)*

```c
tex[0].ptr = mlx_xpm_file_to_image(mlx, cfg.no_path,
                                   &tex[0].width, &tex[0].height);
tex[0].addr = mlx_get_data_addr(tex[0].ptr, &tex[0].bpp,
                                &tex[0].line_len, &tex[0].endian);
// SO/WE/EA için tekrarla. Biri NULL dönerse Error + temiz çıkış.
```
`tex_pixel(img, x, y)` = `put_pixel`'in okuma sürümü.

## 7. Input & hareket  *(Kişi A)*

- Linux: `mlx_hook(win, KeyPress=2, 1L<<0, on_press, g)` ve
  `KeyRelease=3, 1L<<1, on_release, g)` → `keys[]` dizisini güncelle.
- `mlx_loop_hook(mlx, render, g)` içinde:
  - `keys[W/S]` → `pos += ±dir * MOVE_SPEED` (duvar kontrollü)
  - `keys[A/D]` → `pos += ±plane_dik * MOVE_SPEED` (yana kayma)
  - `keys[LEFT/RIGHT]` → dir & plane'i rotasyon matrisiyle döndür:
    ```
    newDirX = dirX*cos(a) - dirY*sin(a);
    newDirY = dirX*sin(a) + dirY*cos(a);   // plane için aynısı
    ```
  - sonra sahneyi çiz, `mlx_put_image_to_window`.
- **ESC** (keycode 65307) ve pencere `X` (`mlx_hook DestroyNotify=17`) →
  temiz çıkış.

## 8. Temiz çıkış & leak  *(Kişi A + B)*

- 4 texture image + frame image → `mlx_destroy_image`
- `mlx_destroy_window`, `mlx_destroy_display(mlx)` + `free(mlx)` (Linux)
- `free_config(&cfg)` (zaten var), `t_game` içindeki her şey
- `valgrind --leak-check=full ./cub3D maps/valid.cub` → **0 leak**.

---

## İş bölümü özeti

| Kişi A (MLX katmanı)            | Kişi B (Render matematiği)          |
|--------------------------------|-------------------------------------|
| Adım 1 pencere/buffer/put_pixel| Adım 3 DDA                          |
| Adım 2 oyuncu vektörleri       | Adım 4 duvar yüksekliği             |
| Adım 6 texture yükleme         | Adım 5 texture mapping + şerit      |
| Adım 7 input/hareket/dönüş     | Adım 5 zemin/tavan                  |
| Adım 8 temiz çıkış             | Adım 8 leak kontrolü                |

Ortak: `include/cub3d.h` (`t_game` ekleyin), `Makefile` (MLX bölümü).

## Sabitler önerisi (`cub3d.h`)
```c
# define WIN_W 1280
# define WIN_H 720
# define TEX_W 64
# define TEX_H 64
# define MOVE_SPEED 0.05
# define ROT_SPEED  0.03
```

## Kontrol listesi
- [ ] Pencere açılıyor, ESC ve `X` ile temiz kapanıyor
- [ ] 4 yön farklı texture gösteriyor
- [ ] Zemin/tavan iki farklı renk
- [ ] WASD hareket + ←/→ dönüş pürüzsüz
- [ ] Başka pencereye geçince/minimize edince bozulmuyor
- [ ] `valgrind` temiz, norminette temiz
- [ ] README.md (subject Bölüm VI zorunlu)
