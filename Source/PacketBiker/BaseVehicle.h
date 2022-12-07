// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "WheeledVehicle.h"
#include "BaseVehicle.generated.h"

/**
 * 
 */
UCLASS()
class PACKETBIKER_API ABaseVehicle : public AWheeledVehicle
{
	GENERATED_BODY()
	
public:

	ABaseVehicle();

protected:

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void LookUp(float Val);
	void LookRight(float Val);

protected:

	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "True"))
		class USpringArmComponent* SpringArm;

	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "True"))
		class UCameraComponent* Camera;

private:
};
