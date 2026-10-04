#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PBSaveGame.generated.h"

/** Tek slot kayıt. BP_SaveGame'in yerini alır. */
UCLASS()
class PACKETBIKER_API UPBSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	static const FString SlotName;

	UPROPERTY(BlueprintReadWrite) int32 Money = 0;
	UPROPERTY(BlueprintReadWrite) int32 Day = 1;
	UPROPERTY(BlueprintReadWrite) TArray<FName> OwnedVehicles   { TEXT("Scooter") };
	UPROPERTY(BlueprintReadWrite) TArray<FName> OwnedCharacters { TEXT("HomelessMan") };
	UPROPERTY(BlueprintReadWrite) FName SelectedVehicle   = TEXT("Scooter");
	UPROPERTY(BlueprintReadWrite) FName SelectedCharacter = TEXT("HomelessMan");

	// İstatistik (gün sonu ve telemetri için)
	UPROPERTY(BlueprintReadWrite) int32 TotalDeliveries = 0;
	UPROPERTY(BlueprintReadWrite) int32 TotalCrashes = 0;
	UPROPERTY(BlueprintReadWrite) int32 BestDayEarnings = 0;

	UFUNCTION(BlueprintCallable, Category = "Save") static UPBSaveGame* LoadOrCreate();
	UFUNCTION(BlueprintCallable, Category = "Save") bool Save();
};
