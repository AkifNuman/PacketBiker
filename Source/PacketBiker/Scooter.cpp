

#include "Scooter.h"

AScooter::AScooter()
{
	ScooterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	ScooterMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(ScooterMesh);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bInheritPitch = false; SpringArm->bInheritRoll = false; 	SpringArm->bInheritYaw = true;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 27.677864f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

void AScooter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &AScooter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AScooter::LookRight);
}

void AScooter::LookUp(float Val)
{
	if (Val != 0.f)
	{
		AddControllerPitchInput(Val);
	}
}

void AScooter::LookRight(float Val)
{
	if (Val != 0.f)
	{
		AddControllerYawInput(Val);
	}
}
