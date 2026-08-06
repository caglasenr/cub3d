#include "cub3d.h"
#include <mlx.h>
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

void cast_ray(t_player *player, t_config *cfg, double camera_x, t_ray *ray )
{
    ray->ray_dir_x = player->dir_x + player->plane_x * camera_x;
    ray->ray_dir_y = player->dir_y + player->plane_y * camera_x;

    ray->map_x = (int)player->pos_x;
    ray->map_y = (int)player->pos_y;

    if(ray->ray_dir_x == 0)
        ray->delta_dist_x = 1e30;
    else
        ray->delta_dist_x = fabs(1/ray->ray_dir_x);
    
    if(ray->ray_dir_y == 0)
        ray->delta_dist_y = 1e30;
    else
        ray->delta_dist_y = fabs(1/ray->ray_dir_y);

    if(ray->ray_dir_x > 0)
    {
        ray->step_x = 1;
        ray->side_dist_x = (ray->map_x + 1 - player->pos_x)*ray->delta_dist_x;
    }
    else
    {
        ray->step_x = -1;
        ray->side_dist_x = (player->pos_x - ray->map_x) * ray->delta_dist_x;
    }
    
    if(ray->ray_dir_y > 0)
    {
        ray->step_y = 1;
        ray->side_dist_y = (ray->map_y + 1 - player->pos_y)*ray->delta_dist_y;
    }
    else
    {
        ray->step_y = -1;
        ray->side_dist_y = (player->pos_y - ray->map_y) * ray->delta_dist_y;
    }
    
    while(1)
    {
        if(ray->side_dist_x < ray->side_dist_y)
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
        if(cfg->map[ray->map_y][ray->map_x] == '1' || cfg->map[ray->map_y][ray->map_x] == 'D')
        {
            ray->hit_char = cfg->map[ray->map_y][ray->map_x];
            break;
        }
    }
    
    if(ray->side == 0)
        ray->perp_wall_dist = ray->side_dist_x - ray->delta_dist_x;
    else
        ray->perp_wall_dist = ray->side_dist_y - ray->delta_dist_y;

/*
Diyelim harita hücresi map_x = 4 (yani duvar, x ekseninde 4 ile 5 arasında bir dikey düzlem, side == 0 — dikey duvara çarptık).

Bu durumda duvarın x koordinatı sabit (tam 4 ya da 5, hangi taraftan çarptığına göre) — ama y koordinatı duvarın neresine çarptığımızı söylüyor.
Diyelim çarpma noktası (4.0, 2.73) çıktı — yani duvarın y=2 ile y=3 arasındaki kısmına, 0.73 kadar aşağıdan çarpmışız.

0.73 işte bizim wallX'imiz — texture'ın "0 ile 1 arası, %73'üne denk gelen sütununu" kullanacağız demek.
Oyuncunun konumunu biliyoruz (pos_x, pos_y) ve ışının hangi yöne, ne kadar (perp_wall_dist) gittiğini biliyoruz. Yön × mesafe = kat edilen yol.
Oyuncunun konumuna bu yolu eklersen çarpma noktasına ulaşırsın — 
tıpkı "5 km/saat hızla 2 saat gidersen 10 km ilerlersin, başlangıç noktan + 10 km = vardığın yer" gibi.
*/
    if(ray->side == 0)
        ray->wall_x = player->pos_y + ray->perp_wall_dist * ray->ray_dir_y;
    else
        ray->wall_x = player->pos_x + ray->perp_wall_dist * ray->ray_dir_x;
    ray->wall_x -= floor(ray->wall_x);
}
/*
line_height ile duvarın ekranda kaç piksel yukarda gözükeceğini tutacağız
perp_wall_dist büyüdükçe yani duvardan daha fazla uzaklaştıkça lineheight küçülecek
burda oyuncu aşağı yukarı bakmıyor ufuk çizgisi hep ekranın tam ortasında win_h/2 line_height ın yarısı yukarı yarısı aşağı
oyuncu duvara çok yaklaşırsa perp_walldist çok küçülür ve line height orantısız artabilir ekran boyutundan daha büyük çıkabilir
o zaman drawstart negatif olabilir draw end de ekran boyutunu aşabilir o yüzden sınırlamalıyız
*/
void wall_height(t_ray *ray, int *draw_start, int *draw_end)
{
    int line_height;
    line_height = (int)(WIN_H / ray->perp_wall_dist);
    *draw_start = WIN_H / 2 - line_height/2;
    if(*draw_start < 0)
        *draw_start = 0;
    *draw_end = WIN_H/2 + line_height/2;
    if(*draw_end >= WIN_H)
        *draw_end = WIN_H-1;
}
int tex_pixel(t_img *img, int x, int y)
{
    return (*(int *)(img->addr + y * img->line_len + x * (img->bpp / 8)));
}

/*
1)Oyuncu bir duvara bakıyor. Bu duvar hangi yöne bakan bir duvar (kuzey mi, güney mi)? → cast_ray bunu side ve ışın yönünden (ray_dir_x/y'nin işareti) çıkarıyor.
2)"Bu yön" bilgisinden, 4 resimden (textures[0..3]) hangisini kullanacağımıza karar veriyoruz → işte face bu.
3)O resmin tam neresine bakacağımızı (texX, texY) hesaplıyoruz — çünkü duvarın soluna çarpan ışınla sağına çarpan ışın,resmin farklı yerlerini göstermeli (yoksa tüm duvar aynı renkte tek düze görünür, desensiz olur).
4)O piksel konumundaki rengi okuyoruz (tex_pixel — put_pixel'in tam tersi, yazmak yerine okuyan versiyonu).
5)O rengi ekrana (frame), doğru yere (x, y) put_pixel ile basıyoruz.
Yani özetle: texture = duvara basılacak resim dosyası, face = 4 resimden hangisi, texX/texY = o resmin hangi noktası, renk = o noktada saklı olan RGB değeri.

Hangi yöne yürüyordun, duvara çarptığında?" — cevap o duvarın adı.

Doğuya yürürken çarptın → EA 3
Batıya yürürken çarptın → WE 2
Güneye yürürken çarptın → SO 1
Kuzeye yürürken çarptın → NO 0
text_x  bulduğumuz wallx değerini texturedeki gerçek piksel sütunu numarasına çeviriyor diyelim wallx 0.73(duvar genişliği boyunda yüzde kaçına denk geliyoruz) yani duvarın %73 sağ veya solundayız ama texture piksellerden 
oluşur o yüzden net sütun sayısını bulmalıyız. texture geniliği 64 pikselse %73 e denk gelen sütün 0.73*64 = 46.72 yani 46. sütun

diyelimki texture 64 piksel yüksekliğinde ama line_height duvarın ekranda kapladığı piksel sayısı 600 veya 20 
yani biz 64 satırlık resmi 600 veya 20 satırlık bir alana sıkıştırcağız
step= texture[face].height / line_height
line 600 ise 64/600 = 0.106 ekranda 1 satır gitmek için texture üzerinde 0.106 gidiyoruz aynı şekilde
line 20 ise 64/20 3.2 ekranda 1 satır gittiğimizde resimde 3.2 satır gidiyoruz

tex_pos için şu şekilde draw startı en başta kesmiiş olabiliriz wall_height fonksiyonunda bizim negaitf çıktığında 0 a eşitleriz 
mesela line height 2000 olsun win_h 720 olsun 720/2 -2000/2 = -640 burumda draw_start 0 olur
tex_pos (0-720/2+2000/2)*step 640*step olur
*/
void draw_column(t_img *frame, t_img *texture, t_ray *ray, int x, int draw_start, int draw_end)
{
    int tex_x;
    int face;
    double step;
    int line_height;
    double tex_pos;//başlangıç
    int tex_y;
    int y;
    int color;

    if (ray->hit_char == 'D')
        face = 4;
    else if(ray->side == 0)
    {
        if(ray->ray_dir_x > 0)
            face = 3;
        else
            face = 2;
    }
    else
    {
        if(ray->ray_dir_y > 0)
            face = 1;
        else
            face = 0;
    }
    
    tex_x = (int)(ray->wall_x * texture[face].width);
    
    if(ray->side == 0 && ray->ray_dir_x > 0)
        tex_x = texture[face].width - tex_x -1;
    if(ray->side == 1 && ray->ray_dir_y < 0)
        tex_x = texture[face].width - tex_x -1;
        
    line_height = (int)(WIN_H / ray->perp_wall_dist);
    step = (double)texture[face].height / line_height;
    tex_pos=(draw_start - (WIN_H/2 - line_height/2))*step;
    
    y = draw_start;
    while(y <= draw_end)
    {
        tex_y = (int)tex_pos;
        tex_pos += step;
        color = tex_pixel(&texture[face], tex_x, tex_y);
        put_pixel(frame, x, y, color);
        y++;
    }


}
void render_frame(t_img *frame, t_player *player, t_config *cfg, t_img *textures)
{
    //değiskenleri tanımladım
    int     x;
    double  camera_x;
    t_ray   ray;
    int     draw_start;
    int     draw_end;
    
    draw_background(frame, cfg);
    x = 0;
    while (x < WIN_W)
    {
        camera_x = 2.0 * x / (double)WIN_W - 1.0;
        cast_ray(player, cfg, camera_x, &ray);
        wall_height(&ray, &draw_start, &draw_end);
        draw_column(frame, textures, &ray, x, draw_start, draw_end);
        x++;
    }
}
int render_loop(t_game *game)
{
    // 1. Ekranı çizmeden önce klavyedeki duruma göre oyuncuyu yürüt
    move_player(game);

    // 2. Yeni konuma göre ekranı çiz
    render_frame(&game->frame, &game->player, &game->cfg, game->textures);
    
    mlx_put_image_to_window(game->mlx, game->win, game->frame.ptr, 0, 0);
    return (0);
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