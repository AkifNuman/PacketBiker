#include "Scooter.h"
#include "BaseCharacter.h"
#include "FrontWheel.h"
#include "RearWheel.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

AScooter::AScooter()
{
	// Sürücü mesh'i (karakter motora binince buraya kopyalanır)
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(GetMesh());
	CharacterMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	CharacterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpringArm->TargetArmLength = 375.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 84.5f));

	// Tekerlekler: aynı kemiğe iki teker (dar iz) — Faz 4'te gerçek iki tekere geçilecek
	UChaosWheeledVehicleMovementComponent* Movement = GetChaosMovement();
	if (Movement)
	{
		Movement->WheelSetups.SetNum(4);
		auto SetWheel = [&](int32 Index, TSubclassOf<UChaosVehicleWheel> Class, FName Bone, float YOffset)
		{
			Movement->WheelSetups[Index].WheelClass = Class;
			Movement->WheelSetups[Index].BoneName = Bone;
			Movement->WheelSetups[Index].AdditionalOffset = FVector(0.f, YOffset, 0.f);
		};
		SetWheel(0, UFrontWheel::StaticClass(), TEXT("F_Wheel"), -10.f);
		SetWheel(1, UFrontWheel::StaticClass(), TEXT("F_Wheel"), 10.f);
		SetWheel(2, URearWheel::StaticClass(), TEXT("R_Wheel"), -10.f);
		SetWheel(3, URearWheel::StaticClass(), TEXT("R_Wheel"), 10.f);

		Movement->EngineSetup.MaxRPM = 3250.f;
		Movement->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
		Movement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.f, 400.f);
		Movement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(2140.f, 400.f);
		Movement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.f, 400.f);

		Movement->SteeringSetup.SteeringCurve.GetRichCurve()->Reset();
		Movement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(20.f, 0.9f);
		Movement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(60.f, 0.8f);
		Movement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(120.f, 0.7f);
	}

	// Yaklaşan karakteri algılayan kutu
	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(GetMesh());
	InteractBox->SetRelativeLocation(FVector(0.f, 0.f, 69.f));
	InteractBox->SetBoxExtent(FVector(71.5f, 75.1f, 66.2f));
	InteractBox->SetCollisionObjectType(ECC_WorldDynamic);
	InteractBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractBox->OnComponentBeginOverlap.AddDynamic(this, &AScooter::OnOverlapBegin);
	InteractBox->OnComponentEndOverlap.AddDynamic(this, &AScooter::OnOverlapEnd);

	ExitLoc = CreateDefaultSubobject<UArrowComponent>(TEXT("ExitLoc"));
	ExitLoc->SetupAttachment(GetMesh());
	ExitLoc->SetRelativeLocation(FVector(0.f, -61.f, 23.f));
	ExitLoc->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

void AScooter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Throttle", this, &AScooter::Throttle);
	PlayerInputComponent->BindAxis("Steer", this, &AScooter::Steer);
	PlayerInputComponent->BindAction("OutOfVehicle", IE_Pressed, this, &AScooter::OutOfVehicle);
}

void AScooter::Throttle(float Val)
{
	if (UChaosWheeledVehicleMovementComponent* M = GetChaosMovement())
	{
		// Geri: fren olarak uygula, ileri: gaz
		M->SetThrottleInput(FMath::Max(Val, 0.f));
		M->SetBrakeInput(FMath::Max(-Val, 0.f));
	}
}

void AScooter::Steer(float Val)
{
	if (UChaosWheeledVehicleMovementComponent* M = GetChaosMovement())
	{
		M->SetSteeringInput(Val);
	}
}

void AScooter::OnOverlapBegin(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (ABaseCharacter* Chr = Cast<ABaseCharacter>(OtherActor))
	{
		Chr->InBox = true;
	}
}

void AScooter::OnOverlapEnd(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32)
{
	if (ABaseCharacter* Chr = Cast<ABaseCharacter>(OtherActor))
	{
		Chr->InBox = false;
	}
}

void AScooter::GetInVehicle(USkeletalMesh* InputMesh, UClass* AnimInstance, ACharacter* CharacterRef)
{
	if (!InputMesh || !CharacterRef) return;

	CharacterMesh->SetSkeletalMesh(InputMesh, true);
	if (AnimInstance) CharacterMesh->SetAnimInstanceClass(AnimInstance);
	CharacterMesh->SetVisibility(true);

	// Binen karakterin sınıfını hatırla, inince aynısı spawn olsun
	CharacterClass = CharacterRef->GetClass();
	bInVehicle = true;

	if (AController* C = CharacterRef->GetController())
	{
		C->UnPossess();
		C->Possess(this);
	}
	CharacterRef->Destroy();
}

void AScooter::OutOfVehicle()
{
	if (!bInVehicle || !CharacterClass) return;

	UChaosWheeledVehicleMovementComponent* M = GetChaosMovement();
	if (M && FMath::Abs(M->GetForwardSpeed()) > MaxExitSpeed) return; // hareket ederken inilmez

	UWorld* World = GetWorld();
	AController* C = GetController();
	if (!World || !C) return;

	// Dünya transformu + yere oturt
	FTransform SpawnTM = ExitLoc->GetComponentTransform();
	SpawnTM.SetScale3D(FVector::OneVector);
	FHitResult Hit;
	const FVector Start = SpawnTM.GetLocation() + FVector(0, 0, 50.f);
	const FVector End = Start - FVector(0, 0, 300.f);
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ScooterExit), false, this);
	if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		SpawnTM.SetLocation(Hit.ImpactPoint + FVector(0, 0, 90.f));
	}

	FActorSpawnParameters SP;
	SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APawn* ExitChar = World->SpawnActor<APawn>(CharacterClass, SpawnTM, SP);
	if (!ExitChar) return;

	if (M)
	{
		M->SetThrottleInput(0.f);
		M->SetBrakeInput(1.f);
		M->SetHandbrakeInput(true);
	}
	CharacterMesh->SetVisibility(false);
	bInVehicle = false;

	C->UnPossess();
	C->Possess(ExitChar);
}
