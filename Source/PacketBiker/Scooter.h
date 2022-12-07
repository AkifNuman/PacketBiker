// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "BaseVehicle.h"
#include "Scooter.generated.h"

/**
 * 
 */
UCLASS()
class PACKETBIKER_API AScooter : public ABaseVehicle
{
	GENERATED_BODY()

public:
	AScooter();
	virtual void Tick(float DeltaTime) override;

protected:

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Throttle(float Val);
	void Steer(float Val);

	void SteerLeftPress();
	void SteerLeftRel();
	void SteerRightPress();
	void SteerRightRel();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* MoveMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SteerMesh;
protected:

private:
	float Speed;
	float SteerDirection;
};
