#include "cub3d.h"
/*
mlx te frame->adr ekranın bütün piksellerini yan yana dizilmiş tek boyutlu byte dizisi olarak tutuyoriki boyutlu değil 
img->addr başlangıç adresimiz 
her satırın byte cinsinden uzunluğu img->line_len
yani y tane satır y*img->line_len byte ilerde
satır içinde hangi pikselde olduğumuzu bulmak için de  
öncelikle her piksel img->bpp / 8 byte yer kaplar yani x numaralı piksele gitmek için x*(img->bpp/8) byte ileri gideriz
yani istediğimiz piksele gitmek için önce satırını bulup ona o satırdaki boyutunu ekleyeceğiz 
img->addr + (y*img->line_len) + (x*(img->bpp / 8)) == başlangıç adresi + satır+sütün gibi düşün
*/
void put_pixel(t_img *img, int x, int y, int color)
{
    if (x < 0 || y < 0 || x >= img->width || y >= img->height)
        return ;
    *(int *)(img->addr + y * img->line_len + x * (img->bpp / 8)) = color;
}
/*
mlx e rengi int formatında istiyor
bir satırı boyamadan önce bakıyoruz o satır gökyüzü mü floor mu 
onu döngüye girmeden eksktra başta bir kere hesaplayıp ona göre 
put_pixel ile boyuyoruz
*/
void draw_background(t_img *frame, t_config *config)
{
    int ceiling_color;
    int floor_color;
    int color;
    int x;
    int y;
    ceiling_color = (config->ceiling[0] << 16) | (config->ceiling[1] << 8) | (config->ceiling[2]);
    floor_color = (config->floor[0] << 16) | (config->floor[1] << 8) | (config->floor[2]);
    y = 0;
    while(y < frame->height)
    {
        if(y < frame->height / 2)
            color = ceiling_color;
        else
            color = floor_color;
        x = 0;
        while(x < frame->width)
        {
            put_pixel(frame,x,y,color);
            x++;
        }
        y++;
    }
}

void cast_ray(t_player *player, t_config *cfg, )
{

}
void wall_height(t_ray *ray, int *draw_start, int *draw_end)
{

}
void draw_column(t_img *frame, t_img *texture,t_ray *ray, int x, int *draw_start,int *draw_end)
{

}
void render_frame(t_img *frame, t_player *player, t_config *cfg, t_img *textures)
{
    draw_background(frame, cfg);
    int x = 0;
    while (x < WIN_W)
    {
        camera_x = 2.0 * x / (double)WIN_W - 1.0;
        cast_ray(player, cfg, camera_x, &ray);
        wall_height(&ray, &draw_start, &draw_end);
        draw_column(frame, textures, &ray, x, draw_start, draw_end);
        x++;
    }
}
/*

`src/raycasting/render.c` içindeki `render_frame` ve onun çağırdığı fonksiyonları tek tek açıklıyorum:

## `render_frame(frame, player, cfg, textures)` — ana fonksiyon
Bir karenin (frame) tamamını baştan sona çizmekten sorumlu. Her `mlx_loop_hook` tetiklendiğinde bir kez çağrılacak. İçindeki iş:
1. Önce arka planı doldur (`draw_background`).
2. Ekranın her dikey sütunu (`x = 0..WIN_W-1`) için bir ışın gönder, o ışının çarptığı duvarı bul, yüksekliğini hesapla, texture'la boyayıp o sütunu çiz.

Kendisi hiçbir matematik yapmıyor — sadece doğru sırayla doğru fonksiyonları çağırıyor (orkestrasyon).

## `draw_background(t_img *frame, t_config *cfg)` — henüz yazılmadı
**Amaç:** Işın atmadan önce ekranı ikiye böl: üst yarı tavan rengi (`cfg->ceiling`), alt yarı zemin rengi (`cfg->floor`). Duvar çizimi zaten ekranın ortasına yakın kısmı ezip geçeceği için, geriye kalan gökyüzü/zemin kısmı düz renkte kalıyor.
**Nasıl:** `frame`'in her pikseline `put_pixel` ile `y < WIN_H/2` ise tavan rengi, değilse zemin rengi yaz. RGB değerlerini `(r<<16)|(g<<8)|b` gibi tek int'e paketlemen gerekecek (MLX pikseli öyle bekliyor).

## `cast_ray(t_player *player, t_config *cfg, double camera_x, t_ray *ray)` — bildirilmiş, gövdesi ROADMAP Adım 3
**Amaç:** Tek bir ekran sütununa karşılık gelen ışını haritada DDA algoritmasıyla yürütüp hangi duvar hücresine, hangi yüzden (`side`) çarptığını bulmak.
**Girdi:** `player` (pozisyon/yön/plane), `cfg` (harita), `camera_x` (o sütunun [-1,1] aralığındaki konumu — `render_frame` içinde `2.0*x/WIN_W - 1.0` ile hesaplanıyor).
**Çıktı:** `ray` struct'ını doldurur — `map_x/map_y` (çarpılan hücre), `step_x/step_y`, `side` (0=dikey yüzey/X sınırı, 1=yatay yüzey/Y sınırı), `perp_wall_dist` (dik mesafe — balıkgözü bozulmasını önleyen değer) vb. Mantığı zaten `dda_test.c`'de grafiksiz test ettin.

## `wall_height(t_ray *ray, int *draw_start, int *draw_end)` — bildirilmiş, gövdesi ROADMAP Adım 4
**Amaç:** `ray->perp_wall_dist`'ten, o sütunda duvarın ekranda kaç piksel yükseklikte görüneceğini ve nereden nereye çizileceğini hesaplamak.
**Mantık:** Uzak duvar → kısa çizgi, yakın duvar → uzun çizgi (`lineHeight = WIN_H / perp_wall_dist`). Sonra bunu ekranın dikey ortasına göre ortalayıp `draw_start`/`draw_end`'i, ekran sınırlarını taşmayacak şekilde klemp'liyor.

## `draw_column(t_img *frame, t_img *textures, t_ray *ray, int x, int draw_start, int draw_end)` — henüz yazılmadı
**Amaç:** Tek bir dikey şeridi gerçekten piksel piksel `frame`'e basmak.
**Yapacakları:**
1. `ray->side` ve step yönüne bakarak hangi texture'ı kullanacağını seç (NO/SO/WE/EA → `textures[0..3]`).
2. Duvar üzerindeki çarpma noktasının yatay konumunu (`wallX`) bulup bunu texture'ın `texX` sütununa çevir.
3. `draw_start`'tan `draw_end`'e kadar dikey döngüde, `texY`'yi adım adım ilerletip (`step = TEX_H / lineHeight`) `textures[face]`'ten pikseli okuyup (`tex_pixel`) `frame`'e yaz (`put_pixel`).

## Özetle akış
```
render_frame
 ├─ draw_background          (henüz yok — zemin/tavan)
 └─ x = 0..WIN_W-1 döngüsü:
     ├─ cast_ray              (DDA — hangi duvara çarptı)
     ├─ wall_height           (o duvar ekranda ne kadar uzun)
     └─ draw_column           (henüz yok — texture'lı çizim)
```

*/