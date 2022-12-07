// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVehicle.h"

ABaseVehicle::ABaseVehicle()
{
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->TargetArmLength = 300.f;

	SpringArm->bInheritPitch = true; SpringArm->bInheritRoll = true; SpringArm->bInheritYaw = true; SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 27.677864f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);


	// Adjust the tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;

	// Torque Setup
	Vehicle4W->MaxEngineRPM = 5700.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.f, 500.f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.f, 400.f);

	// Adjust the steering
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_FrontDrive;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.45f;

	// Automatic gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

}

void ABaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &ABaseVehicle::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ABaseVehicle::LookRight);
}

void ABaseVehicle::LookUp(float Val)
{
	if (Val != 0.f)
	{
		AddControllerPitchInput(Val);
	}
}

void ABaseVehicle::LookRight(float Val)
{
	if (Val != 0.f)
	{
		AddControllerYawInput(Val);
	}
}
