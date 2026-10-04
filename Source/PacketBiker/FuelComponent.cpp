#include "FuelComponent.h"

UFuelComponent::UFuelComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFuelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Fuel <= 0.f) return;
	const float Rate = FMath::Lerp(IdlePerSecond, BurnPerSecond, Throttle) * ConsumptionMultiplier;
	Fuel = FMath::Max(0.f, Fuel - Rate * DeltaTime);
	OnFuelChanged.Broadcast(GetFraction());
	if (Fuel <= 0.f) OnFuelEmpty.Broadcast();
}

void UFuelComponent::Refill(float Amount)
{
	Fuel = FMath::Clamp(Fuel + Amount, 0.f, Capacity);
	OnFuelChanged.Broadcast(GetFraction());
}
