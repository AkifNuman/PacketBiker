#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MissionTypes.generated.h"

UENUM(BlueprintType)
enum class EPackageType : uint8
{
	HotFood   UMETA(DisplayName = "Sıcak Yemek"),   // süre kısa
	Fragile   UMETA(DisplayName = "Kırılgan")       // kaza cezalı
};

UENUM(BlueprintType)
enum class EMissionState : uint8
{
	None,
	Offered,     // telefon çalıyor
	ToPickup,    // restorana gidiliyor
	ToDeliver,   // müşteriye gidiliyor
	Completed,
	Failed
};

/** DT_OrderTypes satırı: paket tipine göre süre ve ceza parametreleri. GDD "Ödeme formülü". */
USTRUCT(BlueprintType)
struct FOrderTypeRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) EPackageType Type = EPackageType::HotFood;
	/** Süre = mesafe / Speed + Grace */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float SpeedMps = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float GraceSeconds = 10.f;
	/** Kaza yendiğinde ödeme çarpanı (1 = ceza yok) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float CrashPayMultiplier = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
};

/** Aktif sipariş. */
USTRUCT(BlueprintType)
struct FMission
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) EMissionState State = EMissionState::None;
	UPROPERTY(BlueprintReadOnly) EPackageType PackageType = EPackageType::HotFood;
	UPROPERTY(BlueprintReadOnly) TWeakObjectPtr<AActor> Restaurant;
	UPROPERTY(BlueprintReadOnly) TWeakObjectPtr<AActor> Target;
	UPROPERTY(BlueprintReadOnly) float DistanceMeters = 0.f;
	UPROPERTY(BlueprintReadOnly) float BasePay = 0.f;
	UPROPERTY(BlueprintReadOnly) float TimeLimit = 0.f;
	UPROPERTY(BlueprintReadOnly) float TimeRemaining = 0.f;
	UPROPERTY(BlueprintReadOnly) int32 Crashes = 0;
};
