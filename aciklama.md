# render.c düzeltmeleri

## Context

Kullanıcı `render.c`'yi kendi makinesinde (local) düzenledi ve dosyayı bu
oturuma yükledi (`474f2ef3-render.c`). `put_pixel` ve `draw_background`
doğru ve çalışır durumda — mantığı önceki turlarda birlikte kurduğumuz
şekilde doğru uygulanmış. Ama dosyaya eklenen stub fonksiyonlar
(`cast_ray`, `draw_column`) ve `render_frame`'in güncellenmiş hâli birkaç
derleme hatası içeriyor, ayrıca dosyanın sonuna önceki turda verdiğim
açıklama metni C yorumu olarak yapıştırılmış kalmış. Amaç: bu dosyayı
gerçek `src/raycasting/render.c`'ye taşırken bu sorunları düzeltmek.

## Bulunan sorunlar

1. **`cast_ray` imzası eksik/hatalı** (satır 50):
   ```c
   void cast_ray(t_player *player, t_config *cfg, )
   ```
   Sondaki virgülden sonra parametre yok — `cub3d.h`'deki bildirimle
   (`double camera_x, t_ray *ray` eksik) uyuşmuyor, bu haliyle derlenmez.

2. **`draw_column` imzası header ile çelişiyor** (satır 58):
   ```c
   void draw_column(t_img *frame, t_img *texture, t_ray *ray, int x,
       int *draw_start, int *draw_end)   // pointer
   ```
   `cub3d.h`'de ise `int draw_start, int draw_end` (value) olarak
   bildirilmiş, ve `render_frame` içindeki çağrı da value olarak
   (`draw_column(frame, textures, &ray, x, draw_start, draw_end)`)
   yapılıyor. Pointer/value uyuşmazlığı → "conflicting types" derleme
   hatası verir.

3. **`render_frame` içinde kullanılan değişkenler tanımlanmamış** (satır
   62-74): `camera_x`, `ray`, `draw_start`, `draw_end` hiç deklare
   edilmeden kullanılıyor (orijinal `render.c`'de bunlar fonksiyon
   başında tanımlıydı, bu revizyonda düşmüş). Ayrıca `int x = 0;` blok
   içinde ileride tanımlanmış — 42 norm'a göre tüm deklarasyonlar bloğun
   başında, atamasız olmalı.

4. **Dosya sonuna yapıştırılmış uzun yorum bloğu** (satır 75-116): bu,
   önceki turda sohbette verdiğim `render_frame` açıklama metni —
   kaynak dosyaya ait değil, kaldırılmalı.

5. (Bilgi amaçlı, blokan değil) `cast_ray`/`wall_height`/`draw_column`
   şu an boş gövdeli; içleri dolduğunda parametreler kullanılacağı için
   şimdilik `-Wextra -Werror` altında "unused parameter" uyarısı
   verebilirler — bu, gerçek mantık yazılınca kendiliğinden çözülür,
   şimdi ekstra bir şey yapmaya gerek yok.

## Planlanan değişiklik

`src/raycasting/render.c`'yi güncelle:
- `put_pixel` ve `draw_background`'ı kullanıcının yazdığı haliyle olduğu
  gibi koru (doğru).
- `cast_ray`, `wall_height`, `draw_column` bildirimlerini `cub3d.h` ile
  birebir eşleşecek şekilde düzelt (gövdeleri boş kalabilir — henüz
  yazılmadılar, sıradaki adım bu).
- `render_frame`'i eski (doğru) haline döndür: `camera_x`, `ray`,
  `draw_start`, `draw_end` fonksiyon başında deklare edilsin.
- Dosya sonundaki açıklama yorum bloğunu kaldır.
- Değişiklikten sonra `cc -Wall -Wextra -Werror -c` ile derleme testi
  yap (linkte `cast_ray`/`wall_height`/`draw_column` gövdesiz kalacağı
  için link hatası beklenir, bu normal — sadece `.o` üretimi/derleme
  aşamasını doğrula).

## Doğrulama

- `cc -Wall -Wextra -Werror -Iinclude -Ilibft -Iget_next_line -c
  src/raycasting/render.c -o /tmp/render_test.o` → hatasız derlenmeli.
- Kullanıcıya değişiklikleri satır satır göster, onayını al.