#pragma once

#include "CoreMinimal.h"
#include "BaseVehicle.h"
#include "Scooter.generated.h"

class ABaseCharacter;
class UBoxComponent;
class UArrowComponent;
class USkeletalMeshComponent;

/**
 * Oyuncunun sürdüğü motor. Chaos tabanlı.
 * Faz 2: 4 tekerlek düzeni (2 ön, 2 arka, dar iz) korunur; gerçek 2 teker + yan yatma Faz 4.
 */
UCLASS()
class PACKETBIKER_API AScooter : public ABaseVehicle
{
	GENERATED_BODY()

public:
	AScooter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Throttle(float Val);
	void Steer(float Val);

	/** Karakteri motora bindir: mesh ve anim'i motor üstündeki sürücü mesh'ine kopyalar. */
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void GetInVehicle(USkeletalMesh* InputMesh, UClass* AnimInstance, ACharacter* CharacterRef);

	/** Motordan in: ExitLoc'un dünya konumuna karakter spawn edilir, kontrol ona geçer. */
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void OutOfVehicle();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
	TObjectPtr<USkeletalMeshComponent> CharacterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
	TObjectPtr<UArrowComponent> ExitLoc;

	UPROPERTY(BlueprintReadOnly, Category = "Vehicle")
	bool bInVehicle = false;

	/** Motordan inince spawn edilecek karakter sınıfı. */
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle")
	TSubclassOf<APawn> CharacterClass;

	/** Bu hızın altındayken inilebilir (cm/s). */
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle")
	float MaxExitSpeed = 150.f;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<UBoxComponent> InteractBox;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
