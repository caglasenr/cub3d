--------------GPT------------
# Cub3D Parser Detaylı Açıklaması

# Genel Amaç

Parser'ın görevi `.cub` dosyasını tamamen okuyup oyunun kullanabileceği bir `t_config` yapısı oluşturmaktır.

Örneğin elimizde şöyle bir dosya olsun:

```text
NO textures/north.xpm
SO textures/south.xpm
WE textures/west.xpm
EA textures/east.xpm

F 220,100,0
C 225,30,0

111111111111
100000000001
10N000000001
111111111111
```

Parser bunu okuyarak aşağıdaki bilgileri elde eder.

- North texture yolu
- South texture yolu
- West texture yolu
- East texture yolu
- Floor rengi
- Ceiling rengi
- Harita
- Oyuncunun başlangıç pozisyonu
- Oyuncunun baktığı yön

Bütün bunlar `t_config` içerisine kaydedilir.

---

# Parser Akışı

Program parser çalıştırıldığında aşağıdaki sırayla ilerler.

```
parse_scene()

        │

        ▼

check_extension()

        │

        ▼

read_file()

        │

        ▼

parse_lines()

        │
        ├────────► parse_element_line()
        │               │
        │               ├────► set_texture()
        │               └────► set_color()
        │                               │
        │                               ▼
        │                         parse_rgb()
        │
        ▼

build_map()

        │

        ├────► collect_map()

        ├────► scan_map()

        └────► check_closed()

                        │

                        ▼

                flood fill algoritması

        │

        ▼

check_textures()

        │

        ▼

Parser tamamlandı.
```

---

# parser.c

Bu dosya parser'ın merkezidir.

Bütün parsing işlemini burası yönetir.

---

# init_config()

İlk çalışan fonksiyonlardan biridir.

Görevi bütün config yapısını güvenli hale getirmektir.

Örneğin:

```c
cfg->no_path = NULL;
cfg->so_path = NULL;
cfg->floor[0] = -1;
```

Bunun nedeni daha sonra

```c
if (!cfg->no_path)
```

şeklinde kontrol yapabilmektir.

Henüz texture okunmamışsa pointer NULL olur.

Henüz renk okunmamışsa

```c
-1
```

olarak kalır.

Bu sayede eksik bilgi kolayca anlaşılır.

---

# parse_scene()

Parser'ın giriş noktasıdır.

Sırasıyla aşağıdakileri yapar.

## 1)

Dosya uzantısını kontrol eder.

```
map.cub
```

olmalı.

```
map.txt
```

reddedilir.

---

## 2)

Dosyayı açar.

```c
open(path,O_RDONLY)
```

başarısız olursa hata verir.

---

## 3)

read_file() çağırılır.

Dosyanın bütün satırları belleğe alınır.

Artık elimizde

```c
char **lines;
```

vardır.

Örneğin

```
lines[0]
```

şudur

```
NO texture.xpm
```

```
lines[1]
```

şudur

```
SO texture2.xpm
```

vb.

---

## 4)

parse_lines()

Harita başlamadan önceki bütün elementleri okur.

Yani

```
NO
SO
WE
EA
F
C
```

---

## 5)

build_map()

Haritayı oluşturur.

---

## 6)

Texture dosyalarının gerçekten var olup olmadığını kontrol eder.

---

Başarılıysa parser tamamlanır.

---

# read_file.c

Bu dosyanın görevi

Dosyadaki her satırı belleğe almaktır.

---

## read_file()

İçeride

```c
get_next_line()
```

kullanılır.

Her satır okunur.

```
NO texture.xpm
```

↓

```
SO texture.xpm
```

↓

```
111111111
```

↓

```
100000001
```

...

Bütün satırlar

```c
char **lines
```

içine kaydedilir.

Sonuç

```text
lines
 │
 ├── lines[0]
 ├── lines[1]
 ├── lines[2]
 ├── lines[3]
 └── NULL
```

şeklindedir.

---

## strip_newline()

get_next_line

şunu döndürür.

```
NO texture.xpm\n
```

Parser bunu istemez.

Sonundaki

```
\n
```

silinir.

---

## append_line()

Yeni satırı

```
lines
```

dizisine ekler.

Her seferinde daha büyük bir dizi oluşturur.

---

# parse_elements.c

Bu dosya

Harita başlamadan önceki bilgileri okur.

Örneğin

```
NO texture.xpm
```

```
SO texture.xpm
```

```
F 220,100,0
```

---

## parse_element_line()

Her satıra bakar.

```
NO
```

ile başlıyorsa

```
set_texture()
```

çağrılır.

```
F
```

ile başlıyorsa

```
set_color()
```

çağrılır.

Hiçbiri değilse

artık harita başlamıştır.

---

## set_texture()

Şunları yapar.

- aynı texture ikinci kez yazılmış mı
- path boş mu
- path içinde boşluk var mı

Sonra

```
cfg->no_path
```

gibi alanlara kaydeder.

---

## set_color()

Şunu çağırır.

```
parse_rgb()
```

RGB doğruysa

```
cfg->floor[]
```

içine kaydeder.

---

## check_textures()

Parser bittikten sonra

```
open(path)
```

ile dosya gerçekten var mı diye kontrol edilir.

Yoksa oyun başlamaz.

---

# parse_colors.c

Bu dosya

```
220,100,0
```

gibi RGB değerlerini okur.

---

## parse_rgb()

Şunları kontrol eder.

- kaç sayı var
- hepsi sayı mı
- 0-255 arasında mı

Doğruysa

```
rgb[0]
rgb[1]
rgb[2]
```

olarak kaydedilir.

---

## has_bad_commas()

Şunları yakalar.

Yanlış

```
220,,100
```

Yanlış

```
220,100,
```

Yanlış

```
,220,100
```

Çünkü

```
ft_split()
```

bunları yanlış şekilde kabul edebilir.

---

## parse_channel()

Her rengi tek tek okur.

Örneğin

```
220
```

↓

integer

↓

220

Sonra

```
0<=x<=255
```

kontrol edilir.

---

# parse_map.c

Bu dosya haritanın kendisini işler.

---

## collect_map()

Elementlerden sonraki bütün satırları

```
cfg->map
```

içine kopyalar.

Artık

```text
111111
100001
10N001
111111
```

şeklinde saklanır.

---

## scan_map()

Haritanın her karakterini dolaşır.

Kontrol eder.

```
0
```

```
1
```

```
N
```

```
S
```

```
E
```

```
W
```

```
(space)
```

Bunların dışındaki karakter hata verir.

---

Oyuncu bulunursa

```
cfg->player_x

cfg->player_y

cfg->player_dir
```

kaydedilir.

---

Ayrıca

```
map_w
```

hesaplanır.

En uzun satır bulunur.

Bu flood fill sırasında kullanılacaktır.

---

# validate_map.c

Parser'ın en önemli kısmıdır.

Haritanın kapalı olup olmadığını kontrol eder.

---

# Neden gerekiyor?

Şöyle bir harita düşünelim.

```
1111111
1000001
1000000
1111111
```

Sağ tarafta açıklık vardır.

Oyuncu dışarı çıkabilir.

Parser bunu engellemelidir.

---

# build_grid()

Haritanın bütün satırlarını aynı uzunluğa getirir.

Örneğin

```
111111

1001

111111
```

şuna dönüşür.

```
111111

1001··

111111
```

Boş kalan yerler

```
space
```

ile doldurulur.

Böylece flood fill güvenli çalışabilir.

---

# flood()

Recursive flood fill algoritmasıdır.

Oyuncudan başlar.

```
N
```

↓

komşular

↓

komşular

↓

komşular

Sürekli

```
y+1

y-1

x+1

x-1
```

şeklinde ilerler.

Duvara gelirse durur.

```
1
```

ziyaret edilmiş yere gelirse durur.

```
V
```

Boşluğa ulaşırsa

```
leak = 1
```

olur.

Harita açık demektir.

---

# check_closed()

Grid oluşturur.

Flood fill başlatır.

Eğer

```
leak==1
```

olursa

```
map is not closed
```

hatası verir.

---

# parser_utils.c

Parser boyunca kullanılan yardımcı fonksiyonlardır.

---

## err()

Standart hata çıktısı üretir.

```
Error
invalid texture
```

şeklinde yazdırır.

Her zaman

```
-1
```

döndürür.

Bu sayede

```c
return err("...");
```

şeklinde kullanılabilir.

---

## check_extension()

Dosya

```
.cub
```

ile bitiyor mu kontrol eder.

---

## is_space()

Space veya tab karakteri mi?

---

## is_empty_line()

Satır tamamen boşluklardan mı oluşuyor?

Parser bunu elementlerle harita arasındaki ayrımı yapmak için kullanır.

---

# free_utils.c

Bellek temizleme fonksiyonlarını içerir.

---

## free_split()

```
char **arr
```

içindeki bütün satırları tek tek free eder.

En sonunda diziyi de free eder.

---

## free_config()

Config içinde malloc edilen bütün alanları temizler.

```
texture path

map

vb.
```

Hepsi free edilir.

Pointerlar tekrar

```
NULL
```

yapılır.

Bu, dangling pointer oluşmasını önler.

---

# Parser'ın Genel Veri Akışı

```
.cub Dosyası

      │

      ▼

read_file()

      │

      ▼

char **lines

      │

      ▼

parse_lines()

      │

      ▼

Texture bilgileri
RGB bilgileri

      │

      ▼

collect_map()

      │

      ▼

cfg->map

      │

      ▼

scan_map()

      │

      ▼

Player bulundu

      │

      ▼

check_closed()

      │

      ▼

Flood Fill

      │

      ▼

Harita kapalı mı?

      │

      ▼

check_textures()

      │

      ▼

Parser başarılı.
```

# Özet

Parser'ın temel amacı `.cub` dosyasını güvenli şekilde okuyup doğrulamaktır. Önce dosya okunur ve satırlar belleğe alınır. Ardından texture yolları ve RGB renkleri ayrıştırılır. Sonrasında harita oluşturulur, oyuncunun başlangıç konumu bulunur ve haritanın yalnızca geçerli karakterlerden oluştuğu doğrulanır. En kritik aşamada flood fill algoritması kullanılarak haritanın tamamen duvarlarla çevrili olduğu kontrol edilir. Son olarak texture dosyalarının gerçekten mevcut olduğu doğrulanır. Bu adımların tamamı başarıyla geçerse `t_config` yapısı eksiksiz şekilde hazırlanmış olur ve oyun motoru güvenle çalışmaya başlayabilir.

-----------------------------
NOTEBOOKLM
Harita Doğrulama Süreci (Map Validation)
Harita doğrulama işlemi temel olarak build_map fonksiyonu üzerinden yönetilir. Bu fonksiyon sırasıyla collect_map, scan_map ve check_closed fonksiyonlarını çağırarak haritayı yapılandırır ve doğrular
.
Haritanın Toplanması (collect_map): Haritanın kesintisiz bir blok olup olmadığı kontrol edilir. Harita bölgesi içerisindeki boş bir satır, haritanın tek parça olmadığı anlamına gelir ve bu durum doğrudan kural ihlali sayılarak reddedilir
.
Karakter ve Oyuncu Taraması (scan_map ve scan_row): Harita toplanıp belleğe alındıktan sonra satır satır taranır
. Haritadaki karakterler is_map_char fonksiyonu ile denetlenir; sadece '0', '1', ' ', 'N', 'S', 'E', 'W' karakterlerine izin verilir
. Ayrıca is_player fonksiyonu sayesinde ('N', 'S', 'E', 'W') oyuncunun başlangıç pozisyonu kontrol edilir
.
Flood Fill Algoritması ile Kapalılık Kontrolü (check_closed)
Oynanabilir alanın duvarlarla ('1') çevrili olduğundan emin olmak için Flood Fill algoritması kullanılır. Bu kontrol, oyun motorunda sızıntı (leak) veya açık alan olmasını engeller.
Izgara Oluşturma (build_grid): Harita satırları birbirinden farklı uzunluklarda (girintili çıkıntılı) olabileceği için, öncelikle boşluk karakterleriyle (' ') doldurulmuş dikdörtgen biçimli, güvenli bir harita kopyası oluşturulur. Bu işlem, flood fill algoritmasının her satır ve sütunu sınır dışına taşma (out-of-bounds) hatası almadan güvenle indekslemesini sağlar
.
Özyineli Yayılım (flood): Recursive (özyineli) çalışan algoritma, oyuncunun başlangıç konumundan haritayı doldurmaya başlar
.
Sızıntı (Leak) Kontrolleri:
Sınır Dışına Çıkma: Eğer yayılım harita dışına adım atarsa (y veya x koordinatları 0'dan küçük veya maksimum boyuttan büyük olursa), alanın duvarlarla çevrili olmadığı anlaşılır ve f->leak = 1 bayrağı aktif edilir
.
Boşluğa Denk Gelme: Eğer yayılım bir boşluk (' ') karakterine gelirse, yürünebilir alanın harita dışıyla teması olduğu tespit edilir ve yine haritanın açık olduğu belirlenerek f->leak = 1 şeklinde işaretlenir
.
İlerlemeyi Durdurma: Yayılım sırasındaki hücre halihazırda bir duvar ('1') veya zaten ziyaret edilmiş ('V') bir hücre ise fonksiyon bir işlem yapmadan geri döner
.
İşaretleme ve Devam Etme: Yukarıdaki şartlara takılmayan geçerli hücreler ziyaret edildiğini belirtmek için 'V' olarak değiştirilir ve ardından algoritma dört ana yöne doğru (aşağı y+1, yukarı y-1, sağa x+1, sola x-1) kendini tekrar çağırarak haritayı taramaya devam eder.
----------------------------------------------------------------------------------------- GEMINIAI ---
Bu dosyalar, klasik bir Raycasting motorunun (muhtemelen `cub3D`) konfigürasyon (`.cub`) dosyasını okuyan, ayrıştıran (parsing) ve haritanın kurallara uygun olup olmadığını doğrulayan çok sağlam bir sistem oluşturuyor. Minishell'deki tokenization ve parsing mantığından sonra, buradaki yapı daha çok dosya okuma, veriyi yapılandırma ve algoritma (flood fill) üzerine kurulu.

Aşağıda doğrudan projene entegre edebileceğin, `README.md` veya proje dökümantasyonu olarak kullanabileceğin detaylı markdown formatındaki açıklamayı bulabilirsin:

---

# cub3D Parsing (Ayrıştırma) Modülü Dokümantasyonu

Bu modül, `.cub` uzantılı konfigürasyon dosyasını okuyarak oyunun ihtiyaç duyduğu doku (texture) yollarını, zemin/tavan renklerini ve harita düzenini `t_config` yapısına (struct) aktarır. Aynı zamanda haritanın etrafının duvarlarla kapalı olup olmadığını ve dosya içeriğinin kurallara uygunluğunu denetler.

## 📂 Dosya Dosya Mimari ve İşleyiş

### 1. `parser.c` (Ana Orkestratör)

Tüm parsing işleminin yönetildiği ana dosyadır. İşleyiş `parse_scene` fonksiyonu ile başlar.

* **`init_config`**: Bellekte (Valgrind'de) çöp değerler (garbage values) oluşmasını engellemek için `t_config` yapısındaki tüm işaretçileri `NULL`'a, bayrakları (flag) `0`'a ve renkleri `-1`'e eşitler.
* **`parse_scene`**: İşlemlerin sırasıyla çağrıldığı yerdir.
1. Dosya uzantısı kontrol edilir (`.cub`).
2. Dosya açılır (`open`) ve `read_file` ile tüm satırlar belleğe alınır.
3. `parse_lines` ile haritaya kadar olan kısım (dokular ve renkler) ayrıştırılır.
4. `build_map` ile harita kısmı toplanır ve doğrulanır.
5. Son olarak `check_textures` ile verilen doku dosyalarının gerçekten var olup olmadığı test edilir.



### 2. `read_file.c` (Veri Okuma)

Dosyanın tamamını okuyup iki boyutlu bir karakter dizisine (`char `) çevirir.

* `get_next_line` kullanılarak dosya satır satır okunur.
* **`strip_newline`**: `get_next_line`'dan gelen satır sonundaki `\n` karakterini temizler.
* **`append_line`**: Dinamik bellek yönetimi (malloc) ile her yeni satırı mevcut iki boyutlu diziye ekler. Bu sayede dosya boyutu önceden bilinmese bile tüm dosya güvenle okunur.

### 3. `parse_elements.c` (Doku ve Renk Ayrıştırma)

Harita grid'inden önce gelen yapılandırma verilerini işler (`NO`, `SO`, `WE`, `EA`, `F`, `C`).

* **`parse_element_line`**: Gelen satırın başındaki boşlukları atlar (`skip_spaces`). Hangi tanımlayıcıya ait olduğunu bularak ilgili fonksiyona yönlendirir.
* **`set_texture` / `set_color**`: Çift tanımlama (duplicate) var mı diye kontrol eder. Yoksa veriyi `t_config` içine kaydeder.
* **`check_textures`**: Dokuların yollarını `open(O_RDONLY)` ile açmayı dener. Başarısız olursa geçersiz path hatası fırlatır (Fd sızıntısı olmaması için anında `close` edilir).

### 4. `parse_colors.c` (RGB Validasyonu)

`F 220,100,0` gibi renk satırlarını işler. Edge case'lere (uç durumlara) karşı çok sıkı denetimlere sahiptir.

* **`has_bad_commas`**: `ft_split` fonksiyonu ardışık virgülleri (örn: `255,,0`) tek bir virgül gibi davranarak atlayabileceği için, bunu önceden tespit edip hata döndürür.
* **`all_digits` / `parse_channel**`: Her bir rengin (R, G, B) sadece rakamlardan oluştuğunu, en fazla 3 haneli olduğunu ve `0-255` aralığında olduğunu doğrular.

### 5. `parse_map.c` (Harita Çıkarımı)

Dosyanın elementlerden sonraki kısmı harita olarak kabul edilir.

* **`collect_map`**: Haritanın başladığı satırdan itibaren sonuna kadar okur. Harita içinde "boş satır" varsa hata verir (harita tek parça olmalıdır). Verileri `cfg->map` içerisine kopyalar.
* **`scan_map` & `scan_row**`: Haritadaki karakterleri kontrol eder. Geçersiz karakter varsa (`0, 1, boşluk, N, S, E, W` harici) hata verir. Ayrıca haritada *tam olarak 1 tane* oyuncu başlangıç noktası olduğundan emin olur ve oyuncunun koordinatlarını (`player_x`, `player_y`) kaydeder.

### 6. `validate_map.c` (Duvar Kontrolü / Flood Fill)

Haritanın tamamen duvarlarla (`1`) çevrili olup olmadığını, oyuncunun harita dışına veya boşluklara düşüp düşmeyeceğini kontrol eder.

* **`build_grid`**: Harita satırları genelde farklı uzunluklardadır (jagged array). Güvenli bir algoritma kurabilmek için, haritanın en uzun satırını (`map_w`) baz alarak tüm haritayı bir dikdörtgen haline getirir. Kısa kalan satırların sonunu boşlukla (` `) doldurur.
* **`flood`**: Klasik "Flood Fill" algoritmasıdır. Oyuncunun konumundan başlayarak her yöne (Yukarı, Aşağı, Sağ, Sol) özyinelemeli (recursive) olarak yayılır.
* Eğer bir duvara (`1`) veya zaten ziyaret edilmiş bir yere (`V`) çarparsa durur.
* Eğer ızgara sınırlarının dışına çıkarsa veya bir boşluğa (` `) değerse, `f->leak = 1` yapar (Yani harita deliktir/açıktır).


* **`check_closed`**: Tüm bu yayılma işlemini başlatır ve sonuçta `leak` değişkeni `1` ise "map is not closed by walls" hatası verir.

### 7. `parser_utils.c` & `free_utils.c` (Yardımcı Araçlar)

* **`parser_utils.c`**: Konsola formatlı `Error\n` mesajı yazdıran `err()` fonksiyonunu barındırır. Ayrıca `.cub` uzantı kontrolü ve boşluk tespit fonksiyonlarını içerir.
* **`free_utils.c`**: Bellek sızıntılarını (memory leaks) önlemek için kritik dosyadır. Çift yıldızlı (`char `) dizileri serbest bırakan `free_split` ve program kapanırken ya da hata anında `t_config` içerisindeki tüm ayrılmış bellekleri (`malloc`) sisteme geri veren `free_config` fonksiyonlarını içerir. GDB veya Valgrind ile debug yaparken hayat kurtarır.

## ⚙️ Sistem Nasıl İşliyor? (Execution Flow)

1. Program argüman olarak aldığı `harita.cub` dosyasını okumak üzere tetiklenir.
2. `read_file` tüm dosyayı belleğe (iki boyutlu diziye) çeker.
3. `parse_lines` döngüye girer. Satırları tek tek okur:
* Eğer satır boşsa atlar.
* Eğer satır `NO, SO, WE, EA, F, C`'den biriyle başlıyorsa değerleri struct'a kaydeder.
* Eğer element olmayan bir karaktere (örn: `1` veya `0`) rastlarsa, haritanın başladığını anlar ve döngüyü kırar.


4. Kaldığı satır indeksini (`start`) `build_map`'e paslar.
5. `build_map` haritayı oluşturur, oyuncu konumunu tarar.
6. Harita dikdörtgen bir ızgaraya (`grid`) çevrilir ve oyuncunun konumundan "Flood Fill" başlatılır. Harita kapalıysa test geçer.
7. Son olarak, parse edilen dosya yolları kullanılarak dokuların sistemde var olup olmadığı test edilir.
8. Herhangi bir aşamada hata olursa (`return -1`), o ana kadar ayrılmış tüm bellekler `free_utils` aracılığıyla temizlenir ve ekrana `Error\n[Hata Detayı]` basılarak program güvenli bir şekilde sonlandırılır. Sıfır hata varsa, oyun motoru (raycasting) render aşamasına geçmeye hazırdır.