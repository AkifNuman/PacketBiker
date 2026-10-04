#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DaySubsystem.generated.h"

class UPBSaveGame;

USTRUCT(BlueprintType)
struct FDaySummary
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly) int32 Day = 1;
	UPROPERTY(BlueprintReadOnly) int32 Earnings = 0;
	UPROPERTY(BlueprintReadOnly) int32 Deliveries = 0;
	UPROPERTY(BlueprintReadOnly) int32 OnTime = 0;
	UPROPERTY(BlueprintReadOnly) int32 Crashes = 0;
	UPROPERTY(BlueprintReadOnly) int32 BestTip = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDayEnded, const FDaySummary&, Summary);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, Money);

/**
 * Gün sayacı (GDD: 8 dk gerçek zaman, 08:00→24:00), para, gün sonu özeti, kayıt.
 * BP_DayEndigCounter + BP_Counter + BP_LevelDatabase'in yerini alır.
 * Sipariş sıklığını MissionSubsystem'i tetikleyerek yönetir.
 */
UCLASS()
class PACKETBIKER_API UDaySubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day") float DayLengthSeconds = 480.f;

	UFUNCTION(BlueprintCallable, Category = "Day") void StartDay();
	UFUNCTION(BlueprintCallable, Category = "Day") void EndDay();
	UFUNCTION(BlueprintCallable, Category = "Day") void AddMoney(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Day") bool SpendMoney(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Day") int32 GetMoney() const;
	UFUNCTION(BlueprintPure, Category = "Day") int32 GetDay() const;
	UFUNCTION(BlueprintPure, Category = "Day") float GetDayProgress() const { return bRunning ? Elapsed / DayLengthSeconds : 0.f; }
	/** "HH:MM" oyun saati (08:00 → 24:00) */
	UFUNCTION(BlueprintPure, Category = "Day") FText GetClockText() const;
	UFUNCTION(BlueprintPure, Category = "Day") UPBSaveGame* GetSave() const { return Save; }

	UPROPERTY(BlueprintAssignable) FOnDayEnded OnDayEnded;
	UPROPERTY(BlueprintAssignable) FOnMoneyChanged OnMoneyChanged;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UDaySubsystem, STATGROUP_Tickables); }
	virtual bool DoesSupportWorldType(EWorldType::Type T) const override { return T == EWorldType::Game || T == EWorldType::PIE; }

private:
	UFUNCTION() void HandleMissionFinished(const struct FMission& Mission, float Pay);
	float NextOfferInterval() const;

	UPROPERTY() TObjectPtr<UPBSaveGame> Save;
	FDaySummary Today;
	float Elapsed = 0.f;
	float OfferTimer = 0.f;
	bool bRunning = false;
};
