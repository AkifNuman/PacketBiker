// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Scooter.generated.h"

UCLASS()
class PACKETBIKER_API AScooter : public APawn
{
	GENERATED_BODY()

public:
	AScooter();

protected:

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void LookUp(float Val);
	void LookRight(float Val);

protected:
	UPROPERTY(Category = SkeletalMesh, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "True"))
		class USkeletalMeshComponent* ScooterMesh;

	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "True"))
		class USpringArmComponent* SpringArm;

	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "True"))
		class UCameraComponent* Camera;
};
