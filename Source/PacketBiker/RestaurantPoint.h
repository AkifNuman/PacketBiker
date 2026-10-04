#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RestaurantPoint.generated.h"

class USphereComponent;

/** Paket alınan nokta. Oyuncu küreye girince MissionSubsystem'e bildirir. */
UCLASS()
class PACKETBIKER_API ARestaurantPoint : public AActor
{
	GENERATED_BODY()

public:
	ARestaurantPoint();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	FText DisplayName;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	UPROPERTY(VisibleAnywhere, Category = "Mission")
	TObjectPtr<USphereComponent> Trigger;

	UFUNCTION()
	void OnTrigger(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, int32 Idx, bool bSweep, const FHitResult& Hit);
};
