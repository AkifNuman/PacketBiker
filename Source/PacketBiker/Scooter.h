// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "BaseVehicle.h"
#include "Containers/Array.h"
#include "Components/SkeletalMeshComponent.h"
#include "Scooter.generated.h"

/**
 * 
 */

class UCurveFloat;

UCLASS()
class PACKETBIKER_API AScooter : public ABaseVehicle
{
	GENERATED_BODY()

public:
	AScooter();


protected:
	virtual void BeginPlay() override;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ACharacter* Char;

	UFUNCTION(BlueprintCallable, Category = "kategori")
	void GetInVehicle(USkeletalMesh* InputMesh, UClass* AnimInstance, bool PosproccessIsAnime, ACharacter* CharacterRef);

	// Karakterlerin listesi

protected:

private:
	float Speed;
	float SteerDirection;
};
