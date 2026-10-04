#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FuelComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFuelChanged, float, Fraction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFuelEmpty);

/** Motora takılır. GDD: depo 100, gazda 1/sn, boşta 0.1/sn. */
UCLASS(ClassGroup = (PacketBiker), meta = (BlueprintSpawnableComponent))
class PACKETBIKER_API UFuelComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFuelComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel") float Capacity = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel") float BurnPerSecond = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel") float IdlePerSecond = 0.1f;
	/** Robot karakteri gibi pasif yetenekler için çarpan. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel") float ConsumptionMultiplier = 1.f;

	UFUNCTION(BlueprintCallable, Category = "Fuel") void Refill(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Fuel") void SetThrottle(float Throttle01) { Throttle = FMath::Clamp(Throttle01, 0.f, 1.f); }
	UFUNCTION(BlueprintPure, Category = "Fuel") float GetFuel() const { return Fuel; }
	UFUNCTION(BlueprintPure, Category = "Fuel") float GetFraction() const { return Capacity > 0 ? Fuel / Capacity : 0.f; }
	UFUNCTION(BlueprintPure, Category = "Fuel") bool IsEmpty() const { return Fuel <= 0.f; }

	UPROPERTY(BlueprintAssignable) FOnFuelChanged OnFuelChanged;
	UPROPERTY(BlueprintAssignable) FOnFuelEmpty OnFuelEmpty;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Fuel = 100.f;
	float Throttle = 0.f;
};
