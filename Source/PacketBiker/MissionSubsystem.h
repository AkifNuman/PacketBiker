#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MissionTypes.h"
#include "MissionSubsystem.generated.h"

class ARestaurantPoint;
class ADeliveryPoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionChanged, const FMission&, Mission);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionFinished, const FMission&, Mission, float, Pay);

/**
 * Sipariş üretir, aktif siparişi takip eder, süreyi sayar, ödemeyi hesaplar.
 * Haritadaki ARestaurantPoint / ADeliveryPoint aktörleri BeginPlay'de kaydolur.
 * UI (telefon, HUD) delegeleri dinler. Tek aktif sipariş (GDD MVP).
 */
UCLASS()
class PACKETBIKER_API UMissionSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	// --- kayıt ---
	void RegisterRestaurant(ARestaurantPoint* P);
	void RegisterDelivery(ADeliveryPoint* P);
	void Unregister(AActor* P);

	// --- akış ---
	/** Yeni sipariş teklif et (telefon çalar). Aktif sipariş varsa false. */
	UFUNCTION(BlueprintCallable, Category = "Mission") bool OfferMission();
	UFUNCTION(BlueprintCallable, Category = "Mission") void AcceptMission();
	UFUNCTION(BlueprintCallable, Category = "Mission") void DeclineMission();
	/** Oyuncu restorana/hedefe ulaştığında nokta aktörü çağırır. */
	void NotifyArrived(AActor* Point);
	/** Oyuncu kaza yaptı. */
	UFUNCTION(BlueprintCallable, Category = "Mission") void NotifyCrash();

	UFUNCTION(BlueprintPure, Category = "Mission") const FMission& GetMission() const { return Current; }
	UFUNCTION(BlueprintPure, Category = "Mission") bool HasActiveMission() const;

	UPROPERTY(BlueprintAssignable) FOnMissionChanged OnMissionChanged;
	UPROPERTY(BlueprintAssignable) FOnMissionFinished OnMissionFinished;

	/** DT_OrderTypes. Proje ayarından ya da GameMode'dan atanır. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	TObjectPtr<UDataTable> OrderTypes;

	// UTickableWorldSubsystem
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UMissionSubsystem, STATGROUP_Tickables); }
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override { return WorldType == EWorldType::Game || WorldType == EWorldType::PIE; }

private:
	float ComputePay(bool bOnTime) const;
	void Finish(bool bSuccess);
	const FOrderTypeRow* FindRow(EPackageType Type) const;

	UPROPERTY() TArray<TWeakObjectPtr<ARestaurantPoint>> Restaurants;
	UPROPERTY() TArray<TWeakObjectPtr<ADeliveryPoint>> Deliveries;
	FMission Current;
};
