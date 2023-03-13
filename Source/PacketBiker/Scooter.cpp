 // Fill out your copyright notice in the Description page of Project Settings.


#include "Scooter.h"
#include "FrontWheel.h"
#include "RearWheel.h"
#include "Components/TimelineComponent.h"


static const FName NAME_SteerInput("Steer");
static const FName NAME_ThrottleInput("Throttle");

AScooter::AScooter()
{
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mannequin(TEXT("/Game/Vehicles/Motorcycles/Scooter/Meshes/Scooter"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("/Game/Vehicles/Motorcycles/Scooter/Scooter_AnimBP"));
	GetMesh()->SetSkeletalMesh(Mannequin.Object);
	GetMesh()->SetAnimInstanceClass(AnimBP.Object->GeneratedClass);

	// Character Mesh Setup
	Character = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character"));
	Character->SetupAttachment(GetMesh());
	Character->SetRelativeLocation(FVector(0.000000f, -0.000135f, -1.465975f));
	Character->SetRelativeRotation(FRotator(0.000000f, -90.000183f, 0.000000f));

	// MoveMesh Setup
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Move_Mesh(TEXT("SkeletalMesh'/Game/Vehicles/Motorcycles/Scooter/Meshes/Scooter.Scooter'"));
	MoveMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Move Mesh"));
	MoveMesh->SetupAttachment(GetMesh());
	MoveMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Move_Mesh.Succeeded())
	{
		MoveMesh->SetSkeletalMesh(Move_Mesh.Object);
		MoveMesh->SetAnimInstanceClass(AnimBP.Object->GeneratedClass);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Steer_Mesh(TEXT("/Game/Vehicles/Motorcycles/Scooter/Meshes/SteerSkel"));
	SteerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Steer"));
	SteerMesh->SetupAttachment(MoveMesh);
	SteerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SteerMesh->SetRelativeLocation(FVector(47.021297f, 0.000046f, 100.295670f));
	if (Steer_Mesh.Succeeded())
	{
		SteerMesh->SetSkeletalMesh(Steer_Mesh.Object);
	}

	SpringArm->TargetArmLength = 375.f;

	//Wheel Setup Area
	check(Vehicle4W->WheelSetups.Num() == 4);
	
	Vehicle4W->bDeprecatedSpringOffsetMode = true;

	Vehicle4W->WheelSetups[0].WheelClass = UFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("F_Wheel");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -10.0f, 0.f);
	Vehicle4W->WheelSetups[0].bDisableSteering = false;

	Vehicle4W->WheelSetups[1].WheelClass = UFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("F_Wheel");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 10.0f, 0.f);
	Vehicle4W->WheelSetups[1].bDisableSteering = false;

	Vehicle4W->WheelSetups[2].WheelClass = URearWheel::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("R_Wheel");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.0f, -10.0f, 0.f);
	Vehicle4W->WheelSetups[2].bDisableSteering = true;

	Vehicle4W->WheelSetups[3].WheelClass = URearWheel::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("R_Wheel");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.0f, 10.0f, 0.f);
	Vehicle4W->WheelSetups[3].bDisableSteering = true;
	
	//Scooter Engine Edit Area
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(2138.715820f, 400.f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5729.577637f, 400.f);

	Vehicle4W->EngineSetup.MaxRPM = 3257.77417;

	//Scooter Steer Edit Area
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(20.0f, 0.9000f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(60.0f, 0.8f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.7f);
	

}


void AScooter::BeginPlay()
{
	Super::BeginPlay();

}

void AScooter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(NAME_ThrottleInput, this, &AScooter::Throttle);
	PlayerInputComponent->BindAxis(NAME_SteerInput, this, &AScooter::Steer);

	PlayerInputComponent->BindAction("SteerLeft", IE_Pressed, this, &AScooter::SteerLeftPress);
	PlayerInputComponent->BindAction("SteerLeft", IE_Released, this, &AScooter::SteerLeftRel);

	PlayerInputComponent->BindAction("SteerRight", IE_Pressed, this, &AScooter::SteerRightPress);
	PlayerInputComponent->BindAction("SteerRight", IE_Released, this, &AScooter::SteerRightRel);
}

void AScooter::Throttle(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
	Speed = GetVehicleMovement()->GetForwardSpeed();
}

void AScooter::Steer(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void AScooter::SteerLeftPress()
{
	SteerDirection = -50.f;
}

void AScooter::SteerLeftRel()
{
	SteerDirection = 0.f;
}

void AScooter::SteerRightPress()
{
	SteerDirection = 50.f;
}

void AScooter::SteerRightRel()
{
	SteerDirection = 0.f;
}

void AScooter::GetInVehicle(USkeletalMesh* InputMesh, UClass* AnimInstance, bool PosproccessIsAnime, ACharacter* CharacterRef)
{

}

