#include "BaseVehicle.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"

ABaseVehicle::ABaseVehicle()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = false; // motor yan yatarken kamera yatmasın
	SpringArm->bInheritYaw = true;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 27.7f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	UChaosWheeledVehicleMovementComponent* Movement = GetChaosMovement();
	if (!Movement) return;

	// Motor
	Movement->EngineSetup.MaxRPM = 5700.f;
	Movement->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Movement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.f, 400.f);
	Movement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.f, 500.f);
	Movement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.f, 400.f);

	// Direksiyon: hız arttıkça açı azalır
	Movement->SteeringSetup.SteeringCurve.GetRichCurve()->Reset();
	Movement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(0.f, 1.f);
	Movement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(40.f, 0.7f);
	Movement->SteeringSetup.SteeringCurve.GetRichCurve()->AddKey(120.f, 0.6f);

	// Diferansiyel ve şanzıman
	Movement->DifferentialSetup.DifferentialType = EVehicleDifferential::RearWheelDrive;
	Movement->TransmissionSetup.bUseAutomaticGears = true;
	Movement->TransmissionSetup.GearChangeTime = 0.15f;
}

UChaosWheeledVehicleMovementComponent* ABaseVehicle::GetChaosMovement() const
{
	return Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
}

void ABaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Eski axis eşlemeleri Faz 8'de Enhanced Input'a taşınacak.
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseVehicle::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ABaseVehicle::LookRight);
}

void ABaseVehicle::LookUp(float Val)
{
	if (!FMath::IsNearlyZero(Val)) AddControllerPitchInput(Val);
}

void ABaseVehicle::LookRight(float Val)
{
	if (!FMath::IsNearlyZero(Val)) AddControllerYawInput(Val);
}
