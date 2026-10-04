# PacketBiker → Product Yol Haritası

> Repo: github.com/AkifNuman/PacketBiker · UE 4.27 · son commit 27 Nis 2023 · 63 commit · ~1 GB
> Hazırlayan: tiohub/claude · 2 Eki 2026
> Her faz bitince `[ ]` → `[x]` yap, altına tarih ve not düş. Sırayla ilerle, faz atlama.

## Projenin felsefesi (bozulmayacak)

- **Low-poly kurye oyunu**: Synty Polygon şehrinde scooter/motorla restorandan paket al, müşteriye yetiştir.
- **Gün döngüsü + ekonomi**: gün sonu sayacı, kazanılan dolar, yakıt derdi, yeni motor/karakter satın alma.
- **Karakter kimliği**: Grandma, HomelessMan, Robot, Wizard, Samurai, GamerGirl... her birinin yeteneği (dash vb.) var.
- **Araçtan in/bin**: oyuncu motordan inip yürüyebiliyor, şehirde trafik ve yayalar yaşıyor.
- **Arcade his, sim değil**: cel shader, fish-eye, dash çizgileri. Eğlence > gerçekçilik.
- **Hedef platform**: Mobil (Android) birinci, PC ikinci. Ayarlarda zaten Mobile hedefli.

## Motor kararı: Unreal'da kal, UE5'e taşı

Unity'ye geçiş önerilmiyor. Gerekçe:
- Blueprint mantığının tamamı (mission, fuel, save, minimap, traffic, 13 karakter BP, anim BP'ler) sıfırdan yazılır. 1 GB içeriğin yeniden import'u, materyal ve cel shader'ın yeniden yapımı gerekir. Kazanç yok, 2-3 ay kayıp var.
- Asıl sorun UE değil, UE **4.27 + PhysX** olması. PhysX UE5'te kaldırıldı; `.uproject` zaten `ChaosVehiclesPlugin`'i açmış ve `DefaultEngine.ini`'de Lumen/VSM anahtarları var, yani proje bir ara UE5'te açılmış ama kod PhysX'te kalmış.
- Doğru yol: UE 5.4'e taşı, araçları Chaos'a geçir, mevcut içeriği koru.

Tek istisna: Mobilde performans UE5 ile tutmazsa (Faz 7 sonunda ölçülecek) Unity'ye geçiş yeniden değerlendirilir. O zamana kadar Unity gündemde değil.

## Tespit edilen sorunlar (özet)

| Alan | Sorun |
|---|---|
| Repo | `.uasset`/`.umap` için LFS yok, 1 GB düz git. 7 Polygon paketinin demo haritaları ve kullanılmayan paketler (Samurai, FantasyRivals, Scifi'nin çoğu) repoda. |
| Motor | 4.27 + PhysX; UE5 config sızıntıları. `HoloLens` config'i gereksiz. |
| Araç | Motosiklet 4 tekerlekli araç olarak hack'lenmiş (iki teker ±10 offset ile kopyalanmış). Yan yatma yok. `OutOfVehicle` karakteri relative transform ile spawn ediyor (dünya koordinatı değil), yanlış yere düşer. `Char`/`Chartype` null kontrolü yok. |
| Kod | `Deneme.h` ölü sınıf. `RestaurantPoint` boş iskelet, asıl mission mantığı BP'de. Debug mesajları ("COKME BABI", "YAT ASSAAAA") prod'da. `BaseMesh` iki kez atanıyor. GameMode boş. |
| Haritalar | `Map`, `Level`, `TestLevel`, `GameStart` dağınık; hangisi gerçek oyun belli değil. |
| Mission | Sipariş üretimi, süre, ödeme BP'de ve tek seferlik. Tekrar oynanabilir döngü yok. |
| UI | Menü ekran görüntüsü PNG'lerden oluşmuş, placeholder. Ayarlar boş. |
| Build | Packaging ayarı yok, CI yok, README tek satır. Hiç release yok. |

## 10 Faz

### Faz 1 · Kurtarma ve envanter  `[ ]`
Amaç: proje temiz bir makinede açılsın, derlensin, ne var ne yok bilinsin.
- [ ] UE 4.27 ile projeyi aç, derle, `Map.umap`'i oyna. Çalışan/çalışmayan özellik listesi çıkar (video kaydı al).
- [x] Git LFS kur: `*.uasset *.umap *.png *.fbx *.wav` LFS'e. `.gitignore`'a `Binaries/ Intermediate/ Saved/ DerivedDataCache/ .vs/` (zaten var, kontrol et).
- [x] Kullanılmayan paketleri ve tüm `*/Maps/Demonstration*`, `*/Maps/Overview*` haritalarını sil. Hedef repo boyutu < 400 MB.
- [x] `Deneme.h/.cpp`, `HoloLens` config, `Res_Point` sil. Debug mesajlarını kaldır.
- [x] README: nasıl açılır, hangi harita gerçek, kontroller. `docs/` klasörü aç, bu dosyayı oraya koy.
- [ ] Kabul: temiz klon + 4.27 ile 15 dakikada açılıp oynanabiliyor.

### Faz 2 · UE5 ve Chaos Vehicles'a geçiş  `[ ]`
Amaç: modern motor, PhysX bağımlılığı sıfır.
- [ ] UE 5.4 ile aç, migration log'unu kaydet.
- [x] `ABaseVehicle`/`AScooter` → `AWheeledVehiclePawn` + `UChaosWheeledVehicleMovementComponent`. `UFrontWheel`/`URearWheel` → `UChaosVehicleWheel`.
- [x] `PhysXVehicles` plugin ve `.uproject` bağımlılığını kaldır. `Build.cs`'e `ChaosVehicles`, `EnhancedInput` ekle.
- [x] Config temizliği: tek `DefaultEngine.ini`, mobil hedef ayarları bilinçli seçilsin (Lumen kapalı, mobile forward shading).
- [ ] Tüm BP'ler hatasız derlensin (Blueprint compile-all).
- [ ] Kabul: UE 5.4'te paketlenmiş Windows build'i oynanıyor.

### Faz 3 · Oyun döngüsü tasarımı (GDD)  `[ ]`
Amaç: "oyun ne?" sorusunun tek sayfalık cevabı. Kod yazmadan önce.
- [x] `docs/GDD.md`: çekirdek döngü = telefon çalar → sipariş kabul → restorana git → paketi al → süre dolmadan teslim et → para + puan → gün biter → dükkan → yeni gün.
- [x] Bir "gün"ün süresi, sipariş sıklığı, ödeme formülü (mesafe × hız bonusu), yakıt tüketimi, başarısızlık koşulları.
- [x] Progresyon: hangi motor/karakter kaça, hangi yetenek ne yapar. 13 karakteri 6'ya indir (en iyi yeteneklere sahip olanlar).
- [x] MVP kapsamı: 1 şehir, 3 motor, 4 karakter, 10 restoran, sonsuz gün. Fazlası kesilir.
- [ ] Kabul: GDD'yi okuyan biri oyunu anlatabiliyor; ikiniz imzaladınız.

### Faz 4 · Araç hissi  `[ ]`
Amaç: motora binmek tek başına eğlenceli olsun. Oyunun kalbi.
- [ ] Gerçek iki tekerlek: Chaos'ta 2 wheel + yan yatma (lean) animasyonu hız/direksiyona bağlı. 4 tekerlek hack'i gitsin.
- [ ] Scooter (yavaş, dayanıklı), Sport (hızlı, kaygan), Base (orta) olmak üzere 3 ayrı data asset ile ayar.
- [~] Bin/in: `OutOfVehicle` dünya transformu (kod yazıldı, test editörde) + yere raycast ile spawn, araç durmadan inilemez, tek `ACharacter` possess/unpossess, karakter mesh'i swap. Null kontrolleri.
- [ ] Kamera: hızla açılan FOV, dönüşte takip, çarpışmada sarsıntı.
- [ ] Çarpışma: belirli hızda düşme (crash anim zaten var), 2 sn sonra kalkma, paket hasarı.
- [ ] Kabul: 10 kişilik playtest, "sürüş keyifli" ≥ 7/10.

### Faz 5 · Mission sistemi (C++)  `[ ]`
Amaç: siparişler sonsuz ve veri güdümlü.
- [x] `UMissionSubsystem` (GameInstance subsystem): sipariş üret, aktif siparişi tut, süre say, tamamla/başarısız.
- [x] `ARestaurantPoint` ve `ADeliveryPoint` gerçek aktörler; haritaya yerleştirilir, subsystem'e kaydolur.
- [ ] `DT_Restaurants`, `DT_OrderTypes` DataTable'ları: paket tipi (sıcak yemek: süre kısa, kırılgan: çarpışma ceza).
- [~] Telefon UI (`WB_Notif`) → subsystem delegelerine bağlanacak; → sipariş kabul/ret, HUD'da hedef mesafe + kalan süre, minimap'te marker.
- [x] Ödeme formülü GDD'den.
- [ ] Kabul: 30 dakika kesintisiz sipariş döngüsü, crash yok.

### Faz 6 · Ekonomi, ilerleme, kayıt  `[ ]`
- [x] `UPBSaveGame` (C++): para, gün, sahip olunan motorlar/karakterler, seçili set. `BP_SaveGame`'den taşı.
- [~] Gün sistemi: `BP_DayEndigCounter` → `UDaySubsystem` (C++ hazır, BP geçişi editörde), gün sonu özet ekranı (kazanç, teslimat, kaza).
- [ ] Dükkan: `WB_MenuBuyandPlaywith`'i gerçek satın alma ekranına çevir.
- [ ] Yetenekler: dash (var), + 2 yeni (ör. zaman yavaşlatma, yakıt tasarrufu). `UGameplayAbility` değil, basit component yeterli.
- [~] Yakıt: `UFuelComponent` hazır; `WB_FuelBar` + benzinlik aktörü, yakıt bitince itme mekaniği.
- [ ] Kabul: oyunu kapat-aç, her şey yerinde; 3 gün oynayıp motor alınabiliyor.

### Faz 7 · Şehir, trafik, performans  `[ ]`
- [ ] Tek üretim haritası: `Map`/`Level`/`TestLevel` birleştir → `L_City`. Diğerleri sil. `GameStart` → `L_Menu`.
- [ ] Trafik sistemi stabilizasyonu: spawner limiti, ışıklar, oyuncuyla çarpışma davranışı, hayalet araç temizliği.
- [ ] Yayalar: `BP_AiCharacter` + basit kaçınma. Çarpınca düşsün, ceza.
- [ ] Performans: HISM ile instancing, LOD'lar, HLOD, mobile shader bütçesi. Hedef: orta Android'de 60 fps, PC'de 120.
- [ ] Yükleme: tek harita + streaming yoksa level streaming değerlendir.
- [ ] Kabul: Android cihazda 20 dk oyun, fps ≥ 50, bellek < 1.5 GB. **Burada tutmazsa motor kararı gözden geçirilir.**

### Faz 8 · UI/UX, input, yerelleştirme  `[ ]`
- [ ] Enhanced Input: klavye, gamepad, dokunmatik (sanal joystick + gaz/fren). Mevcut `Steer/Throttle` axis'leri taşı.
- [ ] Menü: ekran görüntüsü PNG'ler yerine gerçek widget'lar. Ana menü, ayarlar (ses, grafik, dil), karakter/motor seçimi.
- [ ] HUD: hız, yakıt, para, süre, minimap tek tutarlı stil (cel shader'a uygun).
- [ ] Yerelleştirme: TR + EN, string table.
- [ ] Tutorial: ilk gün rehberli 3 sipariş.
- [ ] Kabul: oyunu hiç bilmeyen biri 2 dk içinde ilk teslimatı yapıyor.

### Faz 9 · Cila ve kalite  `[ ]`
- [ ] Ses: motor sesi (RPM'e bağlı), şehir ambiyansı (`CitySoundDemo` var), UI sesleri, müzik.
- [ ] VFX: dash çizgileri, fish-eye, lastik izi (`BP_TrackTire`), egzoz, çarpışma.
- [ ] Animasyon geçişleri, kalkma animasyonu, idle çeşitleri.
- [ ] Bug bash: 2 günlük test, tüm crash'ler kapatılır. Crash reporter açık.
- [ ] Telemetri (basit): gün sayısı, ortalama teslimat, kaza sayısı → analiz.
- [ ] Kabul: 1 saatlik oturumda 0 crash, 0 soft-lock.

### Faz 10 · Ürünleştirme ve yayın  `[ ]`
- [ ] Packaging: Shipping config, Android (AAB, 64-bit, imza), Windows. İkon, splash, sürüm numarası.
- [ ] CI: GitHub Actions self-hosted runner (akif/b2) ile her tag'de build. `tiohub` ile ajanları buna bağla.
- [ ] Mağaza: Google Play iç test → kapalı test → üretim. itch.io / Steam sayfası (PC).
- [ ] Pazarlama: 30 sn trailer, 6 ekran görüntüsü, açıklama TR/EN, sosyal medya hesabı.
- [ ] Hukuk: Synty lisansları (kullanılan paketler için geçerli), gizlilik politikası, yaş sınıfı.
- [ ] Yayın sonrası: 1.0.1 için geri bildirim kanalı, haftalık bakım planı.
- [ ] Kabul: mağazada indirilebilir sürüm.

## İlerleme kaydı

| Tarih | Faz | Not |
|---|---|---|
| 2026-10-02 | — | Yol haritası oluşturuldu. |
| 2026-10-05 | 1 | Repo temizliği: PolygonShops + demo haritalar silindi (~400 MB), ölü kod, LFS attributes, README. Branch `product/faz1-kurtarma`. |
| 2026-10-05 | 3 | GDD yazıldı. İmza bekliyor (Akif + khonvoum). |
| 2026-10-05 | 2 | Chaos portu yazıldı, **derlenmedi** (Mac'te UE yok). İlk UE 5.4 açılışı akif'te yapılacak. |
| 2026-10-05 | 5,6 | Mission/Day/Save/Fuel C++ iskeleti yazıldı, derlenmedi. BP bağlantıları editörde. |

## Editör gerektiren sıradaki işler (akif / Windows + UE 5.4)

1. `product/faz1-kurtarma` branch'ini çek, UE 5.4 ile aç, "Rebuild modules". Derleme hatalarını düzelt (Chaos API adları).
2. `BP_BaseMotorcycle`/`BP_BaseVehicle`'ın parent'ını yeni `AScooter`'a bağla, teker kemik adlarını (`F_Wheel`, `R_Wheel`) doğrula.
3. Blueprint compile-all; `BP_SaveGame`, `BP_DayEndigCounter`, `BP_Counter`, `BP_RestaurantPoint`, `BP_TargetPoint` mantığını yeni subsystem'lere taşı.
4. `DT_OrderTypes` DataTable'ını `FOrderTypeRow` ile oluştur (HotFood: 8 m/s +10 s; Fragile: 6 m/s +15 s, kaza ×0.5).
5. `Map.umap` içinde 10 `ARestaurantPoint`, 40 `ADeliveryPoint` yerleştir. Oyna, Faz 1 videosunu kaydet.
6. Git LFS: GitHub ücretsiz kota 1 GB. Mevcut 850 MB içeriği LFS'e taşımak kotayı bitirir. Seçenekler: GitHub Data Pack (5$/ay) **veya** b2 sunucusunda self-hosted LFS (`lfs-test-server`/`giftless`). Karar verilince `git lfs migrate import --everything` çalıştırılır.

