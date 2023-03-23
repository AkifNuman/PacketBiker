// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "BaseVehicle.h"
#include "BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
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
	void OutOfVehicle();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* MoveMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SteerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ACharacter* Char;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool InVehicle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UArrowComponent* ExitLoc;

	UFUNCTION(BlueprintCallable, Category = "Category")
		void GetInVehicle(USkeletalMesh* InputMesh, UClass* AnimInstance, bool PosproccessIsAnime, ACharacter* CharacterRef);

	//UFUNCTION(BlueprintCallable, Category = "Category")
	//void OutOfVehicle();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
		UBoxComponent* CollisionBox;

private:
	float Speed;
	float SteerDirection;

	UPROPERTY(EditAnywhere)
		TSubclassOf<APawn> Chartype;

	UFUNCTION(BlueprintCallable)
		void Spawn();
};
