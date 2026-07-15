#include "cub3d.h"
void set_pos(t_config *cfg, t_player *player)
{
    player->pos_x = cfg->player_x + 0.5;
    player->pos_y = cfg->player_y+ 0.5;
}
void set_dir_plane(t_config *cfg, t_player *player)
{
    if(cfg->player_dir == 'N')
    {
        player->dir_x = 0.0;
        player->dir_y = -1.0;
        player->plane_x = 0.66;
        player->plane_y = 0;
    }
    else if(cfg->player_dir == 'S')
    {
        player->dir_x = 0.0;
        player->dir_y = 1.0;
        player->plane_x = -0.66;
        player->plane_y = 0;
    }
    else if(cfg->player_dir == 'E')
    {
        player->dir_x = 1.0;
        player->dir_y = 0.0;
        player->plane_x = 0.0;
        player->plane_y = 0.66;
    }
    else if(cfg->player_dir == 'W')
    {
        player->dir_x = -1.0;
        player->dir_y = 0.0;
        player->plane_x = 0.0;
        player->plane_y = -0.66;
    }
} 
/*
    N
W       E   
    S
İkisi de matematiksel olarak "dik" — (x,y)'ye dik olan iki vektör var: (-y, x) ve (y, -x). İkisi de dot product = 0 verir, birbirlerinin negatifi. Yani "x'i çevir" ile "y'yi çevir" arasında saf geometri açısından hangisinin "doğru" olduğu diye bir şey yok — biri +90° döndürme, öteki -90° döndürme, ikisi de eşit derecede geçerli bir "dik vektör".

Sorun şu: hangisini seçtiğin önemli değil, ama dördünde de AYNI seçimi yapmak zorundasın. Çünkü plane, kameranın "sağ" yönünü temsil ediyor — ekranın x=0 sütunu bir uçta, x=WIN_W-1 diğer uçta, ve bu sweep'in dir'e göre hep aynı elden (sağdan) olması lazım. N için x'i çevirip E için y'yi çevirirsen, oyuncu kuzeye bakarken görüntü doğru ama doğuya dönünce sağ-sol yer değiştirir — yani karakter döndükçe ekran tutarsız şekilde aynalanır, ki bu MLX'te görünce fark edeceğin türden bir bug olurdu.

Biz "swap sonrası x'i çevir" kuralını seçtik çünkü N için zaten verilmiş cevap (0.66,0) buna uyuyor — yani N'i sabit referans alıp diğer üçünü ona tutarlı hale getiriyoruz. "y'yi çevir" kuralını seçseydik de N için ayrı bir başlangıç değeri gerekirdi ve o zaman görüntü sağ-sol ayna olurdu (gerçek doğruluğu ancak MLX'te görünce anlarız — dda_test.c'deki "ayna testi MLX aşamasında" notu tam bunu kastediyor).
Çünkü plane, kameranın "sağ" yönünü temsil ediyor — ekranın x=0 sütunu bir uçta, x=WIN_W-1 diğer uçta, ve bu sweep'in dir'e göre hep aynı elden (sağdan) olması lazım. 
N yönüne bakarken E (sağ)yönü 1,0 plane (0.66,0)
E yönüne bakarken S (alt)yönü 0,1 plane (0,0.66)
S yönüne bakarken W (batı) yönü -1,0 plane (-0.66,0)
W yönüne bakarken N (yukarı)yönü 0,-1 plane (0,-0.66)
plane değişkeni FOV'un genişliğini ve yönünü tanımlayan bir referans vektör. 
*/
void init_player(t_config *cfg, t_player *player)
{
    set_pos(cfg,player);
    set_dir_plane(cfg,player);
}
