#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeliveryPoint.generated.h"

class USphereComponent;

/** Teslim noktası (ev, ofis). Oyuncu küreye girince MissionSubsystem'e bildirir. */
UCLASS()
class PACKETBIKER_API ADeliveryPoint : public AActor
{
	GENERATED_BODY()

public:
	ADeliveryPoint();

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
