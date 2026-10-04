# PacketBiker · Game Design Document (MVP)

> Tek sayfa. Burada olmayan özellik MVP'de yok. Değişiklik PR ile.

## Tek cümle
Low-poly bir şehirde scooter'ınla restorandan paket alıp süre dolmadan müşteriye yetiştirdiğin, her günün sonunda kazandığın parayla daha iyi motor ve tuhaf karakterler aldığın arcade kurye oyunu.

## Çekirdek döngü (60-90 sn)
1. **Telefon çalar** → ekranda sipariş kartı: restoran, hedef mesafe, ödeme, süre. Kabul / Ret (ret cezasız, 3 ret üst üste → o gün sipariş sıklığı düşer).
2. **Restorana git** → mavi marker. Varınca 1 sn "paket alındı" animasyonu.
3. **Teslim et** → yeşil marker, geri sayım başlar. Süre dolmadan varırsan tam ödeme; dolarsa yarı ödeme; 2× süre geçerse sipariş iptal, ödeme yok.
4. **Para + puan** → HUD'da artar. Gün sayacı ilerler.
5. **Gün biter** (gerçek zamanlı 8 dk) → özet ekranı → **Dükkan** → yeni gün.

## Gün
- Süre: 8 dakika gerçek zaman. Gün içinde saat 08:00 → 24:00 akar (1 oyun saati = 30 sn).
- Sipariş sıklığı: ilk 2 dk her 40 sn, orta 4 dk her 25 sn, son 2 dk her 20 sn. Aynı anda en fazla 1 aktif sipariş (MVP).
- Gün sonu: toplam kazanç, teslimat sayısı, zamanında oranı, kaza sayısı, en iyi bahşiş.

## Ödeme formülü
`ödeme = 5 + mesafe_m / 100 × 2` dolar. Zamanında: ×1.0, geç: ×0.5. Hiç kaza yoksa +%20 bahşiş. "Kırılgan" paket kaza yediyse ödeme ×0.5.

Paket tipleri (MVP'de 2): **Sıcak yemek** (süre kısa: mesafe/8 m/s + 10 sn), **Kırılgan** (süre normal: mesafe/6 m/s + 15 sn, kaza cezalı).

## Yakıt
- Depo 100 birim. Tüketim: gaz basılıyken 1 birim/sn, boşta 0.1.
- Benzinlik: haritada 3 adet, 1 birim = 0.2 $. Yakıt bitince motor itilir (yürüme hızı), telefon çalmaz.
- Yakıt oyuncuyu "yola çıkmadan planla" dedirtmeli ama cezalandırmamalı: gün başında depo her zaman dolu.

## Araçlar (3)
| Motor | Hız | Hızlanma | Tutuş | Depo | Fiyat |
|---|---|---|---|---|---|
| Scooter | 60 km/h | düşük | yüksek | 100 | başlangıç |
| Base | 90 | orta | orta | 80 | 400 $ |
| Sport | 130 | yüksek | düşük | 60 | 1200 $ |

Kaza: 40 km/h üstü duvara/araca çarpma = düşme (crash anim), 2 sn yerde, kalk, motor 3 m geride. Paket "kırılgan"sa hasar.

## Karakterler (4, MVP) ve yetenekler
| Karakter | Yetenek | Süre / cooldown |
|---|---|---|
| HomelessMan | **Dash**: 2 sn hız ×1.6 | 2 sn / 15 sn |
| Grandma | **Sabır**: teslim süresi +%25 (pasif) | — |
| Robot | **Pil**: yakıt tüketimi ×0.6 (pasif) | — |
| GamerGirl | **Slow-mo**: 3 sn zaman ×0.5, dönüşlerde | 3 sn / 25 sn |

Fiyat: HomelessMan başlangıç, Grandma 300 $, Robot 600 $, GamerGirl 900 $.
Diğer 9 karakter (Wizard, Samurai, Demon, ...) MVP sonrası, içerik güncellemesi olarak.

## Şehir
- Tek harita `L_City`, ~1 km², Polygon City + Town + Shops karışımı. 10 restoran, 40 teslim noktası (ev/ofis), 3 benzinlik.
- Trafik: 20-30 araç, ışıklı kavşaklar. Çarpma = kaza. Polis yok (MVP).
- Yayalar: 30-40, kaldırımda, çarpınca düşer, -10 $ ceza, bağırır.

## Kontroller
- **Dokunmatik**: sol alt gaz/fren kaydırıcı, sağ alt direksiyon, sağ üst yetenek, sol üst telefon.
- **Gamepad**: RT gaz, LT fren, sol stick direksiyon, A yetenek, Y telefon, B in/bin.
- **Klavye**: W/S gaz-fren, A/D direksiyon, Shift yetenek, Tab telefon, E in/bin.

## Platform ve bütçe
- Birincil: Android (orta cihaz, 60 fps). İkincil: Windows.
- Oturum: 1 gün = 8 dk. Hedef oturum 2-3 gün.

## MVP'de YOK
Çoklu sipariş, polis, gece/gündüz ışık değişimi, hava, çok oyunculu, görevler/hikaye, hız cezası, günlük görevler, reklam/IAP (yayından sonra).

## Başarı ölçütü
Playtest'te 10 kişiden 7'si ikinci günü kendiliğinden oynuyor.
