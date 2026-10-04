#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "BaseVehicle.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UChaosWheeledVehicleMovementComponent;

/** Tüm motorların ortak tabanı: kamera, bakış, Chaos araç ayarları. */
UCLASS(Abstract)
class PACKETBIKER_API ABaseVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ABaseVehicle();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void LookUp(float Val);
	void LookRight(float Val);

	/** Chaos hareket bileşenine kısayol. */
	UChaosWheeledVehicleMovementComponent* GetChaosMovement() const;

protected:
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;
};
