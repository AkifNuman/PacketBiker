#include "DeliveryPoint.h"
#include "MissionSubsystem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ADeliveryPoint::ADeliveryPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	SetRootComponent(Trigger);
	Trigger->InitSphereRadius(300.f);
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADeliveryPoint::OnTrigger);
}

void ADeliveryPoint::BeginPlay()
{
	Super::BeginPlay();
	if (UMissionSubsystem* S = GetWorld()->GetSubsystem<UMissionSubsystem>()) S->RegisterDelivery(this);
}

void ADeliveryPoint::EndPlay(const EEndPlayReason::Type Reason)
{
	if (UWorld* W = GetWorld()) if (UMissionSubsystem* S = W->GetSubsystem<UMissionSubsystem>()) S->Unregister(this);
	Super::EndPlay(Reason);
}

void ADeliveryPoint::OnTrigger(UPrimitiveComponent*, AActor* Other, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (Other != UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) return;
	if (UMissionSubsystem* S = GetWorld()->GetSubsystem<UMissionSubsystem>()) S->NotifyArrived(this);
}
