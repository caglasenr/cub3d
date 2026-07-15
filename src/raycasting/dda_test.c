/*
** dda_test.c — Aşama 0: grafiksiz DDA test düzeneği
**
** Amaç: MLX'e dokunmadan raycasting matematiğini doğrulamak.
** Derle:  cc -Wall -Wextra -Werror dda_test.c -lm -o dda_test
** Hedef çıktı: her iki test ışını için perpWallDist = 1.5000
**
** TODO'ları sırayla doldur. Her TODO, konuştuğumuz bir adıma karşılık
** geliyor; beklenen ara değerler yorumlarda yazılı.
*/

#include <stdio.h>
#include <math.h>

#define MAP_W 12
#define MAP_H 7

/* .cub dosyandaki harita — N kaldırıldı (satır 2, sütun 4 artık '0') */
static const char *g_map[MAP_H] = {
	"111111111111",
	"100000000001",
	"100000000001",
	"100000000001",
	"100000001001",
	"100000001001",
	"111111111111",
};

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;
typedef struct s_ray
{
    int map_x;
    int map_y;
    int step_x;
    int step_y;
    int side;
    double ray_dir_x;
    double ray_dir_y;
    double delta_dist_x;
    double delta_dist_y;
    double side_dist_x;
    double side_dist_y;
    double perp_wall_dist

}t_ray;

static void	init_player(t_player *p)
{
	p->pos_x = 4.5;	/* sütun 4 + 0.5: karenin ortası */
	p->pos_y = 2.5;	/* satır 2 + 0.5 */
	/* TODO A (Adım 3): 'N' için dir. Kuzey = y'nin AZALDIĞI yön. */
	p->dir_x = 0.0;
	p->dir_y = -1.0;	/* ??? */
	/* TODO B (Adım 4): dir'e dik, uzunluk 0.66.
	** Tarif: bileşenlerin yerini değiştir, birinin işaretini çevir,
	** 0.66 ile çarp. Ayna testi MLX aşamasında yapılacak. */
	p->plane_x = 0.66;	/* ??? */
	p->plane_y = 0.0;
}

/*
** Tek ışın için DDA. Dönüş: perpWallDist.
** side: son geçilen sınır 0 = X-sınırı (dikey çizgi), 1 = Y-sınırı.
** DİKKAT: harita erişimi HEP g_map[map_y][map_x] — y önce!
*/
static double	cast_ray(t_player *p, double ray_dir_x, double ray_dir_y,
			int *side)
{
	/* TODO 1: map_x, map_y → oyuncunun karesi (floor, int'e ata).
	** Beklenen: (4, 2) */
    int map_x = (int)p->pos_x;
    int map_y = (int)p->pos_y;
    //printf("map:%d %d \n",map_x,map_y);

	/* TODO 2: delta_dist_x/y = birim fiyatlar.
	** Bileşen 0 ise 1e30 ata (0'a bölme!). Değilse fabs(1.0 / bileşen).
	** Test 1'de beklenen: delta_x = 1e30, delta_y = 1.0 */
    double delta_dist_x;
    double delta_dist_y;

    if(ray_dir_x == 0)
        delta_dist_x = 1e30;
    else
        delta_dist_x = fabs(1/ray_dir_x);

    if(ray_dir_y == 0)
        delta_dist_y = 1e30;
    else
        delta_dist_y = fabs(1/ray_dir_y);
    //printf("delta:%e %f\n",delta_dist_x,delta_dist_y);
	/* TODO 3: step_x/y ve İLK side_dist'ler — dört dallı if.
	** ray_dir_x < 0 : step_x = -1, boşluk = pos_x - map_x
	** ray_dir_x > 0 : step_x = +1, boşluk = map_x + 1.0 - pos_x
	** side_dist = boşluk × birim fiyat. Aynısı y için.
	** Test 1'de beklenen: side_dist_y = 0.5 × 1.0 = 0.5 */
    int step_x;
    double side_dist_x;
    if(ray_dir_x > 0)
    {
        step_x = 1;
        side_dist_x = (map_x + 1 - p->pos_x)*delta_dist_x;
    }
    else
    {
        step_x = -1;
        side_dist_x = (p->pos_x - map_x)*delta_dist_x;
    }
    int step_y;
    double side_dist_y;
    if(ray_dir_y > 0)
    {
        step_y = 1;
        side_dist_y = fabs(map_y + 1 - p->pos_y)*delta_dist_y;
    }
    else
    {
        step_y = -1;
        side_dist_y = (p->pos_y - map_y)*delta_dist_y;
    }
    printf("side: (%e, %f) step: (%d, %d)\n", side_dist_x, side_dist_y, step_x, step_y);
	/* TODO 4: döngü — duvar bulana kadar:
	**   küçük side_dist hangisiyse: ona delta'sını ekle,
	**   o eksenin map'ini step kadar kaydır, *side'ı işaretle.
	**   Sonra g_map[map_y][map_x] == '1' mi bak.
	** Her turda printf ile iz bırak:
	**   printf("adim: kare=(%d,%d) sdX=%.3f sdY=%.3f\n", ...);
	** Test 1'de beklenen iz: (4,1) boş → (4,0) DUVAR */
    int	hit;

    hit = 0;
    while (!hit)
    {
	    if (side_dist_x < side_dist_y)
	    {
		    side_dist_x += delta_dist_x;
		    map_x += step_x;
		    *side = 0;
	    }
	    else
	    {
            side_dist_y +=delta_dist_y;
            map_y += step_y;
            *side = 1;
	    }
	    if (g_map[map_y][map_x] == '1')
		    hit = 1;
	    printf("adim: kare=(%d,%d)\n", map_x, map_y);
    }
	/* TODO 5: perpWallDist — son eklemeyi geri al:
	** side == 0 ise side_dist_x - delta_dist_x, değilse y'ninki. */
    double perpWallDist;
    if(side == 0)
        perpWallDist = side_dist_x -delta_dist_x;
    else
        perpWallDist = side_dist_y - delta_dist_y;
	(void)p; (void)ray_dir_x; (void)ray_dir_y; (void)side;
	return (perpWallDist);
}

int	main(void)
{
	t_player	p;
	int			side;
	double		dist;
	double		rd_x;
	double		rd_y;

	init_player(&p);
	printf("pos=(%.2f, %.2f) dir=(%.2f, %.2f) plane=(%.2f, %.2f)\n",
		p.pos_x, p.pos_y, p.dir_x, p.dir_y, p.plane_x, p.plane_y);
	printf("beklenen: pos=(4.50, 2.50) dir=(0.00, -1.00) plane=(0.66, 0.00)\n\n");

	/* Test 1: orta sütun (cameraX = 0) → rayDir = dir */
	dist = cast_ray(&p, p.dir_x, p.dir_y, &side);
	printf("Test 1: perpWallDist = %.4f, side = %d  (beklenen: 1.5000, 1)\n\n",
		dist, side);

	/* Test 2: cameraX = 0.5 → rayDir = dir + plane*0.5
	** Bu ışın çapraz gider ama AYNI düz duvara çarpar.
	** perpWallDist yine 1.5 çıkmalı — dik mesafe olduğu için!
	** (Balıkgözü olmamasının kanıtı: düz duvar = sabit dik mesafe.) */
	rd_x = p.dir_x + p.plane_x * 0.5;
	rd_y = p.dir_y + p.plane_y * 0.5;
	dist = cast_ray(&p, rd_x, rd_y, &side);
	printf("Test 2: perpWallDist = %.4f, side = %d  (beklenen: 1.5000, 1)\n",
		dist, side);
	return (0);
}
