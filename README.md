# PacketBiker

Low-poly arcade kurye oyunu. Scooter'ınla restorandan paket al, süre dolmadan müşteriye yetiştir, günün sonunda kazandığınla daha iyi motor ve tuhaf karakterler al.

**Motor:** Unreal Engine 5.4 (Chaos Vehicles) · **Hedef:** Android (birincil), Windows
**Durum:** ürünleştirme, bkz. [docs/ROADMAP.md](docs/ROADMAP.md) · Tasarım: [docs/GDD.md](docs/GDD.md)

## Açmak için
1. Git LFS kur: `git lfs install`
2. Klonla: `git clone git@github.com:AkifNuman/PacketBiker.git && cd PacketBiker && git lfs pull`
3. UE 5.4 ile `PacketBiker.uproject`'i aç. İlk açılışta "Rebuild modules" de.
4. Oyun haritası: `Content/Map/Map.umap` (Faz 7'de `L_City` olacak). Menü: `Content/Map/GameStart.umap`.

## Kontroller (geçici, Faz 8'de Enhanced Input)
W/S gaz-fren · A/D direksiyon · E motordan in/bin · fare kamera

## Klasör yapısı
```
Source/PacketBiker/    C++: araç tabanı, scooter, karakter, restoran noktası
Content/Map/           oyun haritaları
Content/Mission/       sipariş, telefon, hedef marker BP'leri
Content/Level_Database sayaçlar, gün sonu, save game
Content/Vehicles/      motorlar
Content/Characters/    oynanabilir karakterler ve yayalar
Content/TrafficSystem/ trafik AI
Content/Polygon*/      Synty asset paketleri (lisanslı, yeniden dağıtma)
docs/                  ROADMAP, GDD
```

## Katkı
`product/*` branch'lerinde çalış, `master`'a PR ile gir. Büyük binary'ler LFS'te; `.uasset`/`.umap` eklerken `git lfs ls-files` ile kontrol et.
