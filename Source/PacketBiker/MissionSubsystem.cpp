#include "MissionSubsystem.h"
#include "RestaurantPoint.h"
#include "DeliveryPoint.h"
#include "Kismet/GameplayStatics.h"

void UMissionSubsystem::RegisterRestaurant(ARestaurantPoint* P) { if (P) Restaurants.AddUnique(P); }
void UMissionSubsystem::RegisterDelivery(ADeliveryPoint* P)     { if (P) Deliveries.AddUnique(P); }
void UMissionSubsystem::Unregister(AActor* P)
{
	Restaurants.RemoveAll([P](const TWeakObjectPtr<ARestaurantPoint>& R) { return R.Get() == P; });
	Deliveries.RemoveAll([P](const TWeakObjectPtr<ADeliveryPoint>& D) { return D.Get() == P; });
}

bool UMissionSubsystem::HasActiveMission() const
{
	return Current.State == EMissionState::ToPickup || Current.State == EMissionState::ToDeliver;
}

const FOrderTypeRow* UMissionSubsystem::FindRow(EPackageType Type) const
{
	if (!OrderTypes) return nullptr;
	for (const auto& Pair : OrderTypes->GetRowMap())
	{
		const FOrderTypeRow* Row = reinterpret_cast<const FOrderTypeRow*>(Pair.Value);
		if (Row && Row->Type == Type) return Row;
	}
	return nullptr;
}

bool UMissionSubsystem::OfferMission()
{
	if (Current.State != EMissionState::None && Current.State != EMissionState::Completed && Current.State != EMissionState::Failed)
		return false;
	if (Restaurants.IsEmpty() || Deliveries.IsEmpty()) return false;

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!Player) return false;

	// Oyuncuya yakın bir restoran, ondan uzak bir hedef
	ARestaurantPoint* R = nullptr;
	float Best = TNumericLimits<float>::Max();
	for (const auto& W : Restaurants)
	{
		if (ARestaurantPoint* P = W.Get())
		{
			const float D = FVector::DistSquared(P->GetActorLocation(), Player->GetActorLocation());
			if (D < Best) { Best = D; R = P; }
		}
	}
	ADeliveryPoint* T = nullptr;
	{
		TArray<ADeliveryPoint*> Valid;
		for (const auto& W : Deliveries) if (ADeliveryPoint* P = W.Get()) Valid.Add(P);
		if (Valid.IsEmpty() || !R) return false;
		T = Valid[FMath::RandRange(0, Valid.Num() - 1)];
	}

	FMission M;
	M.State = EMissionState::Offered;
	M.PackageType = FMath::RandBool() ? EPackageType::HotFood : EPackageType::Fragile;
	M.Restaurant = R;
	M.Target = T;
	M.DistanceMeters = FVector::Dist(R->GetActorLocation(), T->GetActorLocation()) / 100.f;
	M.BasePay = 5.f + (M.DistanceMeters / 100.f) * 2.f; // GDD

	const FOrderTypeRow* Row = FindRow(M.PackageType);
	const float Speed = Row ? Row->SpeedMps : 8.f;
	const float Grace = Row ? Row->GraceSeconds : 10.f;
	M.TimeLimit = M.DistanceMeters / Speed + Grace;
	M.TimeRemaining = M.TimeLimit;

	Current = M;
	OnMissionChanged.Broadcast(Current);
	return true;
}

void UMissionSubsystem::AcceptMission()
{
	if (Current.State != EMissionState::Offered) return;
	Current.State = EMissionState::ToPickup;
	OnMissionChanged.Broadcast(Current);
}

void UMissionSubsystem::DeclineMission()
{
	if (Current.State != EMissionState::Offered) return;
	Current = FMission();
	OnMissionChanged.Broadcast(Current);
}

void UMissionSubsystem::NotifyArrived(AActor* Point)
{
	if (Current.State == EMissionState::ToPickup && Point == Current.Restaurant.Get())
	{
		Current.State = EMissionState::ToDeliver; // süre teslimde başlar
		Current.TimeRemaining = Current.TimeLimit;
		OnMissionChanged.Broadcast(Current);
	}
	else if (Current.State == EMissionState::ToDeliver && Point == Current.Target.Get())
	{
		Finish(true);
	}
}

void UMissionSubsystem::NotifyCrash()
{
	if (HasActiveMission()) { Current.Crashes++; OnMissionChanged.Broadcast(Current); }
}

void UMissionSubsystem::Tick(float DeltaTime)
{
	if (Current.State != EMissionState::ToDeliver) return;
	Current.TimeRemaining -= DeltaTime;
	if (Current.TimeRemaining < -Current.TimeLimit) // 2× süre geçti → iptal
	{
		Finish(false);
	}
}

float UMissionSubsystem::ComputePay(bool bOnTime) const
{
	float Pay = Current.BasePay * (bOnTime ? 1.f : 0.5f);
	if (Current.Crashes == 0) Pay *= 1.2f; // bahşiş
	else if (const FOrderTypeRow* Row = FindRow(Current.PackageType)) Pay *= Row->CrashPayMultiplier;
	return FMath::RoundToFloat(Pay);
}

void UMissionSubsystem::Finish(bool bSuccess)
{
	const float Pay = bSuccess ? ComputePay(Current.TimeRemaining >= 0.f) : 0.f;
	Current.State = bSuccess ? EMissionState::Completed : EMissionState::Failed;
	OnMissionFinished.Broadcast(Current, Pay);
	OnMissionChanged.Broadcast(Current);
}
